// /*
//  * @Author: murnure 2662761173@qq.com
//  * @Date: 2024-07-30 23:26:06
//  * @LastEditors: murnure 2662761173@qq.com
//  * @LastEditTime: 2024-08-02 16:26:45
//  * @FilePath: \code\applications\lvgl\UIDemo\Page\Page_RGBLed.c
//  * @Description: 
//  * 
//  * Copyright (c) 2024 by murmure, All Rights Reserved. 
//  */
#include "../DisplayPrivate.h"
#include "ap3216c.h"
PAGE_EXPORT(alarm);

// #define BUZZER_PIN      GET_PIN(B, 0);
static void alarm_update(void *parameter);
static rt_thread_t alarm_handle = RT_NULL;
static rt_thread_t beep_handle = RT_NULL;
void alarm_thread_init();
void Beep_thread_entry();
rt_err_t Beep_Thread_init();
rt_mailbox_t beep_delay_mb; // 定义一个邮箱变量
static lv_obj_t *alarm_img;
static void alarm_Init(lv_obj_t* par);
static void Setup()
{
    /* 将应用窗口移至前台，使其成为活动窗口 */
    lv_obj_move_foreground(appWindow);
    beep_delay_mb = rt_mb_create("beep_mailbox", 10, RT_IPC_FLAG_FIFO);
    alarm_Init(appWindow);
    alarm_thread_init();
    Beep_Thread_init();
    rt_kprintf("fly start");
    PageDelay(100);

}

void alarm_thread_init()
{
    alarm_handle = rt_thread_create("time_update",
        alarm_update,
        RT_NULL,
        1024,
        10,
        10);
    if (alarm_handle != RT_NULL)
    {
        rt_thread_startup(alarm_handle);
    }
}
static void alarm_update(void *parameter)
{
    ap3216c_device_t dev;
    dev = ap3216c_init("i2c2"); 
    uint16_t proximity;
    while (1)
    {
        // ambient_light = ap3216c_read_ambient_light(dev);
        proximity = ap3216c_read_ps_data(dev);
        rt_kprintf("proximity:%d\n",  proximity);
        if(proximity > 50)
        {   
            rt_mb_send(beep_delay_mb, (rt_uint32_t)(10));
            lv_obj_clear_flag(alarm_img, LV_OBJ_FLAG_HIDDEN);//隐藏对象
        }
        else
        {
            lv_obj_add_flag(alarm_img, LV_OBJ_FLAG_HIDDEN);//隐藏对象
        }
        
        rt_thread_mdelay(1100-proximity);
        // PageDelay(100);
    }
}
rt_err_t Beep_Thread_init()
{
    rt_pin_mode(GET_PIN(B, 0), PIN_MODE_OUTPUT);

    // rt_sem_init(&beep_sem_lock, "beep_sem", 0 , RT_IPC_FLAG_PRIO);

    beep_handle = rt_thread_create("Beeper",
                                    Beep_thread_entry, RT_NULL,
                                    256,
                                    25,
                                    10);
    if (beep_handle != RT_NULL)
    {
        rt_thread_startup(beep_handle);
    }
    else
    {
        rt_kprintf("create thread Beeper failed");
        return -1;
    }
}
void Beep_thread_entry()
{
    int beep_delay_time;
    while (RT_TRUE)
    {
        // rt_sem_take(&beep_sem_lock, RT_WAITING_FOREVER);
        rt_mb_recv(beep_delay_mb, (rt_ubase_t*)&beep_delay_time, RT_WAITING_FOREVER);
        rt_pin_write(GET_PIN(B, 0), PIN_HIGH);
        rt_thread_mdelay(beep_delay_time);
        rt_pin_write(GET_PIN(B, 0), PIN_LOW);
    }
    
}
static void alarm_Init(lv_obj_t* par)
{
    // Create an image object on the parent container
    lv_obj_t *img = lv_img_create(par);
    lv_obj_set_size(img, 100, 100); // Set the size of the image
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0); // Align it to the center of the parent
    // Set the source of the image to the binary data
    lv_img_set_src(img, "E:/sdcard/IMG/alarm.bin");
    //隐藏这个图像控件
     lv_obj_add_flag(img, LV_OBJ_FLAG_HIDDEN);//隐藏对象
    alarm_img = img;

}

static void Exit()
{
    rt_thread_delete(beep_handle);
    rt_thread_delete(alarm_handle);
    lv_obj_clean(appWindow);
    rt_kprintf("fly exit");
}


static void Event(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    
    // 当检测到的事件类型为键盘事件时，执行以下操作
    if(code == LV_EVENT_KEY)
    {
        // 从事件数据中获取按键值
        uint8_t key = *(uint8_t*)lv_event_get_user_data(e);
        // 打印获取的按键值，用于调试
        if(key == PAGE_KEY_RIGHT)
        {
            // PM_Push(Page, PAGE_MainMenu);
            // PM_Push(Page, PAGE_MainMenu);
        }
        
        if(key == PAGE_KEY_LEFT)
        {

            // 向左翻页时，推入硬件测试页面
            PM_Pop(Page);
        }
        if(key == PAGE_KEY_UP)
        {

        }
        if(key == PAGE_KEY_DOWN)
        {
        }


    }

}
