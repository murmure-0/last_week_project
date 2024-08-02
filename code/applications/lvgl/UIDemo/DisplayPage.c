#include "DisplayPrivate.h"

/*实例化页面调度器*/
PageManager_t page;

/*页面导入*/
#define PAGE_IMPORT(name)\
do{\
    extern void PageRegister_##name(PageManager_t* page_p, uint8_t pageID);\
    PageRegister_##name(&page, PAGE_##name);\
}while(0)

/**
 * 页面手势事件回调函数
 * @param obj 手势事件发生的对象
 * @param event 事件类型，此处专门处理手势事件
 * 当检测到页面上的手势事件时，此回调函数被调用。
 * 它通过获取当前活跃的输入设备的手势方向，并将该信息传输给页面管理器，
 * 以处理相应的页面导航操作。
 */
static void page_gestute_event_cb(lv_obj_t * obj, lv_event_t event)
{
    /* 判断事件类型是否为手势事件 */
    if(event.code == LV_EVENT_GESTURE)
    {
        /* 获取当前手势的方向 */
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
        /* 将页面、对象和手势方向信息传输给页面管理器进行处理 */
        PM_EventTransmit(&page, obj, dir);
    }
}

#define PIN_KEY_LEFT        GET_PIN(C, 0)     // PC0:  KEY0         --> KEY
#define PIN_KEY_DOWN        GET_PIN(C, 1)      // PC1 :  KEY1         --> KEY
#define PIN_KEY_RIGHT       GET_PIN(C, 4)      // PC4 :  KEY2         --> KEY
#define PIN_KEY_UP        	GET_PIN(C, 5)     // PC5:  WK_UP        --> KEY
uint8_t globel_key_val;

#define BUZZER_PIN      GET_PIN(B, 0)
static rt_thread_t tid = RT_NULL;
static struct rt_semaphore beep_sem_lock = {0};
rt_err_t Beeper_Thread_init(void);

/*	PAGE_KEY_NONE,
	PAGE_KEY_LEFT,
	PAGE_KEY_RIGHT,
	PAGE_KEY_UP,
	PAGE_KEY_DOWN,*/

static uint8_t Key_Read(void)
{
	if(rt_pin_read(PIN_KEY_LEFT) == PIN_LOW)
	{
		return PAGE_KEY_LEFT;
	}
	if(rt_pin_read(PIN_KEY_RIGHT) == PIN_LOW)
	{
		return PAGE_KEY_RIGHT;
	}
	if(rt_pin_read(PIN_KEY_UP) == PIN_LOW)
	{
		return PAGE_KEY_UP;
	}
	if(rt_pin_read(PIN_KEY_DOWN) == PIN_LOW)
	{
		return PAGE_KEY_DOWN;
	}
	return PAGE_KEY_NONE;
}

/**
 * @brief 键盘扫描线程入口函数
 * 
 * 该函数周期性地读取键盘状态，处理按键事件。
 * 它区分了按键按下和持续按压的情况，并在适当的时候触发按键事件。
 */
static void KeyScan_ThreadEntry(void)
{
    /* 上一次检测到的键值，初始化为无键按下状态 */
    static uint8_t last_key = PAGE_KEY_NONE;
    /* 上一次按键事件的时间戳 */
    static uint32_t last_tick = 0;
    {
        /* 读取当前键值 */
        uint8_t key = Key_Read();
        /* 如果有键被按下 */
        if (key != PAGE_KEY_NONE)
        {
            /* 延迟50毫秒，以滤除短暂的抖动 */
            rt_thread_mdelay(50);
            /* 如果键值仍然被按下，并且与上一次按下键值不同 */
            if (key == Key_Read() && last_key != key)
            {
                /* 输出按键信息到控制台 */
                rt_kprintf("%d pressed!", key);
                /* 更新最后一次按键的时间戳 */
                last_tick = rt_tick_get();
                /* 设置全局键值变量，并发送按键事件 */
                globel_key_val = key;
                lv_event_send(AppWindow_GetObj(page.NowPage), LV_EVENT_KEY, &globel_key_val);
                /* 发送蜂鸣器信号，以提示按键事件 */
                rt_sem_release(&beep_sem_lock);
                /* 更新上一次按键的键值 */
                last_key = key;
            }
            /* 如果键持续被按下超过1秒 */
            else if(last_key == key && rt_tick_get() - last_tick > 200)
            {
                /* 更新全局键值变量，并发送按键事件 */
                globel_key_val = key;
                lv_event_send(AppWindow_GetObj(page.NowPage), LV_EVENT_KEY, &globel_key_val);
                /* 更新上一次按键的键值 */
                last_key = key;
            }
        }
        /* 如果没有键被按下，但之前有键被按下过 */
        else if(last_key != PAGE_KEY_NONE)
        {
            /* 将上一次按键状态重置为无键按下 */
            last_key = PAGE_KEY_NONE;
        }
    }
}

static void Btns_Init(void)
{
	globel_key_val = 0;
	rt_pin_mode(PIN_KEY_LEFT, PIN_MODE_INPUT_PULLUP);
	rt_pin_mode(PIN_KEY_RIGHT, PIN_MODE_INPUT_PULLUP);
	rt_pin_mode(PIN_KEY_DOWN, PIN_MODE_INPUT_PULLUP);
	rt_pin_mode(PIN_KEY_UP, PIN_MODE_INPUT_PULLUP);
}

/**
  * @brief  页面初始化
  * @param  无
  * @retval 无
  */
/**
 * 初始化显示页面模块。
 * 该函数负责初始化页面管理器，导入所有页面，并设置当前页面为拨号页面。
 * 同时初始化按钮和蜂鸣器线程，并设置屏幕触摸事件回调函数。
 */
void DisplayPage_Init()
{
    // 初始化页面管理器，传入页面数组、最大页面数量和页面堆栈大小
    PM_PageManagerInit(&page, PAGE_MAX, 10);
    
    // 导入所有可用的页面
    PAGE_IMPORT(MainMenu);        // 主菜单页面
    // PAGE_IMPORT(SDCard);          // SD卡管理页面
    PAGE_IMPORT(Dial);            // 拨号页面
    PAGE_IMPORT(Main);
    PAGE_IMPORT(HitPlant);
    // PAGE_IMPORT(SysInfo);         // 系统信息页面
    // PAGE_IMPORT(Attitude);        // 姿态显示页面
    // PAGE_IMPORT(BackLight);       // 背光设置页面
    // PAGE_IMPORT(HWTest);          // 硬件测试页面
    // PAGE_IMPORT(RGBLed);          // RGBLED设置页面
    // PAGE_IMPORT(Setting);         // 设置页面
    // PAGE_IMPORT(TimeSet);         // 时间设置页面
    // PAGE_IMPORT(EnvMonitor);      // 环境监测页面
    // PAGE_IMPORT(WiFi);            // WiFi设置页面
    
    // 注释掉的代码表示原本计划推入LED和按钮页面，但目前未使用
    // page.Push(PAGE_LEDAndBtn);
    
    // 将拨号页面推入页面堆栈，设置为当前显示的页面
    PM_Push(&page, PAGE_Main);
    
    // 初始化按钮模块
    Btns_Init();
    
    // 初始化蜂鸣器线程
    Beeper_Thread_init();
    
    // 为当前屏幕添加事件回调函数，处理页面手势事件
    lv_obj_add_event_cb(lv_scr_act(), (lv_event_cb_t)page_gestute_event_cb, LV_EVENT_ALL, NULL);
}

void Page_Loop_running(void)
{
	KeyScan_ThreadEntry();
	lv_task_handler();
	rt_thread_mdelay(LV_DISP_DEF_REFR_PERIOD);
}

/**
  * @brief  显示更新
  * @param  无
  * @retval 无
  */
void Display_Update()
{
    PM_Running(&page);
	KeyScan_ThreadEntry();
    //lv_task_handler();
}

/**
  * @brief  页面阻塞延时，保持lvgl更新
  * @param  无
  * @retval 无
  */
void PageDelay(uint32_t ms)
{
    uint32_t lastTime = lv_tick_get();
    
    while(lv_tick_elaps(lastTime) <= ms)
    {
        lv_task_handler();
		rt_thread_mdelay(LV_DISP_DEF_REFR_PERIOD);
    }
}

void Beeper_thread_entry()
{
    while (RT_TRUE)
    {
        rt_sem_take(&beep_sem_lock, RT_WAITING_FOREVER);
        rt_pin_write(BUZZER_PIN, PIN_HIGH);
        rt_thread_mdelay(100);
        rt_pin_write(BUZZER_PIN, PIN_LOW);
    }
    
}

rt_err_t Beeper_Thread_init()
{
    rt_pin_mode(BUZZER_PIN, PIN_MODE_OUTPUT);

    rt_sem_init(&beep_sem_lock, "beep_sem", 0 , RT_IPC_FLAG_PRIO);

    tid = rt_thread_create("Beeper",
                                    Beeper_thread_entry, RT_NULL,
                                    256,
                                    25,
                                    10);
    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }
    else
    {
        rt_kprintf("create thread Beeper failed");
        return -1;
    }
}
