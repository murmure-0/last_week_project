#include "../DisplayPrivate.h"
// #include "aht10.h"
PAGE_EXPORT(HitPlant);

#define BULLET_COUNT 10
#define ENEMY_COUNT 10
typedef struct
{
	lv_obj_t *obj;
	int alive;
	int x;
	int y;
} BulletState;
typedef struct
{
	lv_obj_t *obj;
	int alive;
	int move_x;
	int move_y;
	int x;
	int y;
} EnemyState;



static BulletState Bullet[BULLET_COUNT];
static EnemyState Enemy[ENEMY_COUNT];

static lv_obj_t* fly_plant;

//定时器句柄
static lv_timer_t *Time1_Handle = 0;
static lv_timer_t *Time2_Handle = 0;

// static lv_timer_t *t1 = 0;
// static lv_timer_t *t2 = 0;
// static lv_timer_t *t3 = 0;

static void Bullet_Move(int step);
static void fly_plant_init(lv_obj_t* par);
static void Timer1_CallBack(lv_timer_t *t);
static void Timer2_CallBack(lv_timer_t *t);
static void Bullet_Clear(void);
static void Bullet_Create(void);
static void Bullet_Init(BulletState *z, int nofz);
static void Setup()
{
    /* 将应用窗口移至前台，使其成为活动窗口 */
    lv_obj_move_foreground(appWindow);
    // ContStyle_Setup();
    Bullet_Init(Bullet, BULLET_COUNT);
    fly_plant_init(appWindow);



    
    rt_kprintf("fly start");
    PageDelay(100);
    Time1_Handle = lv_timer_create(Timer1_CallBack, 20, 0);
    Time2_Handle = lv_timer_create(Timer2_CallBack, 2000, 0);
}

static void Timer1_CallBack(lv_timer_t *t)
{
    // 自动向左移动游戏角色，用于实现动态移动效果
    Bullet_Move(2);
    
    // 检测并处理敌人是否被攻击，用于游戏中的战斗逻辑处理
    // enemy_was_attacked();
    
    // 检测并处理BOSS是否被攻击，用于游戏中的战斗逻辑处理
    // boss_was_attacked();
}
static void Timer2_CallBack(lv_timer_t *t)
{
    // 自动向左移动游戏角色，用于实现动态移动效果
    Bullet_Create();
    Bullet_Clear();
    // 检测并处理敌人是否被攻击，用于游戏中的战斗逻辑处理
    // enemy_was_attacked();
    
    // 检测并处理BOSS是否被攻击，用于游戏中的战斗逻辑处理
    // boss_was_attacked();
}

/**
 * 创建子弹对象
 * 
 * 此函数遍历现有的子弹数组，寻找未激活（即可用的）子弹，然后将其激活并设置初始位置
 * 子弹被激活后，会根据某个飞行物（fly1）的位置来确定子弹的初始位置，并将子弹显示在屏幕上
 * 
 * 注意：函数提前返回，只要找到并激活一个子弹，就不会继续寻找数组中剩余的子弹
 */
static void Bullet_Create(void)
{
    // 遍历子弹数组
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        // 检查当前子弹是否未激活
        if (Bullet[i].alive == 0)
        {
            // 激活子弹
            Bullet[i].alive = 1;
            // 更新飞行物的位置
            lv_obj_refr_pos(fly_plant);
            // 设置子弹的初始位置，与飞行物相同
            Bullet[i].x = lv_obj_get_x(fly_plant);
            Bullet[i].y = lv_obj_get_y(fly_plant);

            // 在屏幕上创建子弹按钮对象
            Bullet[i].obj = lv_btn_create(appWindow);
            // 设置子弹按钮对象的位置
            lv_obj_set_pos(Bullet[i].obj, Bullet[i].x+12, Bullet[i].y-15);
            // 更新子弹按钮对象的位置
            lv_obj_refr_pos(Bullet[i].obj);
            // 设置子弹按钮对象的大小
            lv_obj_set_size(Bullet[i].obj, 5, 15);
            
            // 设置子弹按钮对象的样式，移除阴影
            lv_obj_set_style_shadow_width(Bullet[i].obj, 0, LV_PART_MAIN);
            // 找到并激活一个子弹后，提前返回，结束函数
            return;
        }
    }
}
static void Bullet_Clear(void)
{
	int i;
	// 遍历所有子弹
	for (i = 0; i < BULLET_COUNT; i++)
	{
		// 检查子弹是否激活且超出屏幕左边界
		if (Bullet[i].alive == 1 && Bullet[i].y < 0)
		{
			// 删除LVGL对象，避免界面污染
			lv_obj_del(Bullet[i].obj);
			// 重置子弹状态为非激活
			Bullet[i].alive = 0;
			// 清除子弹的LVGL对象指针
			Bullet[i].obj = 0;
			// 重置子弹位置
			Bullet[i].x = 0;
			Bullet[i].y = 0;
		}
	}
}
static void Bullet_Init(BulletState *z, int nofz)
{
	int i;
	// 遍历每一颗子弹，对其进行初始化
	for (i = 0; i < nofz; i++)
	{
		// 设置子弹为非活跃状态
		z->alive = 0;
		// 清除子弹关联的对象
		z->obj = 0;
		// 将子弹的位置设置为原点
		z->x = 0;
		z->y = 0;
		// 指向下一枚子弹
		z++;
	}
}
static void Bullet_Move(int step)
{
	// 遍历子弹库中的每一发子弹
	for (int i = 0; i < BULLET_COUNT; i++)
	{
		// 如果子弹处于存活状态
		if (Bullet[i].alive == 1)
		{
			// 更新子弹的x坐标，实现移动效果
			Bullet[i].y -= step;
            // 设置子弹对象在界面上的新位置
			lv_obj_set_pos(Bullet[i].obj, Bullet[i].x+12, Bullet[i].y-15);
			// 重新计算子弹对象的布局位置
			lv_obj_refr_pos(Bullet[i].obj);
		}
	}
}
static void fly_plant_init(lv_obj_t* par)
{
    fly_plant = lv_img_create(par);
    lv_img_set_src(fly_plant, "E:/sdcard/IMG/HIT_PLANT/fly_plant.bin");
    lv_obj_align_to(fly_plant, par, LV_ALIGN_BOTTOM_MID, 0, 0);
}













static void Exit()
{
    lv_obj_clean(appWindow);
    rt_kprintf("fly exit");
}


static void Event(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    
    // 当检测到的事件类型为键盘事件时，执行以下操作
    if(code == LV_EVENT_KEY)
    {
        int plant_next_x,plant_next_y;
        plant_next_x = lv_obj_get_x(fly_plant);
        plant_next_y = lv_obj_get_y(fly_plant);
        // 从事件数据中获取按键值
        uint8_t key = *(uint8_t*)lv_event_get_user_data(e);
        // 打印获取的按键值，用于调试
        // rt_kprintf("lv_indev_get_key %d \n", key);
        // 根据不同的按键值进行页面切换操作
        if(key == PAGE_KEY_RIGHT)
        {
            plant_next_x+=5;
            // 向右翻页时，推入主菜单页面
            // PM_Push(Page, PAGE_MainMenu);
            // PM_Push(Page, PAGE_MainMenu);
        }
        
        if(key == PAGE_KEY_LEFT)
        {
            plant_next_x-=5;
            // 向左翻页时，推入硬件测试页面
            // PM_Push(Page, PAGE_Main);
        }
        if(key == PAGE_KEY_UP)
        {
            plant_next_y-=5;
            // rt_kprintf("key up");
        }
        if(key == PAGE_KEY_DOWN)
        {
            plant_next_y+=5;
            // rt_kprintf("key down");
        }
        lv_obj_set_pos(fly_plant, plant_next_x, plant_next_y);
    }
    // 当检测到按下事件时
    // if (code == LV_EVENT_PRESSING)
    // {
    //     // 获取当前活动输入设备的触摸方向
    //     lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
    //     // 如果触摸方向为向右
    //     if (dir == LV_DIR_RIGHT)
    //     {
    //         // 推送页面到主菜单页面
    //         PM_Push(Page, PAGE_MainMenu);
    //     }
    // }
}
