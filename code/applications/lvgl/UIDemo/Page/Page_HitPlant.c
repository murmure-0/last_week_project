#include "../DisplayPrivate.h"
// #include "aht10.h"
#include "stdlib.h"
PAGE_EXPORT(HitPlant);
LV_IMG_DECLARE(IMG_ENEMY);
LV_IMG_DECLARE(IMG_PLANT);
#define TITLE_FONT (lv_font_montserrat_20)
#define TITLE_FONT (lv_font_montserrat_32)
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
//用户得分
static int score = 0;

#define BULLET_COUNT 5
#define ENEMY_COUNT 3



static BulletState Bullet[BULLET_COUNT];
static EnemyState Enemy[ENEMY_COUNT];

static lv_obj_t* fly_plant;

//定时器句柄
static lv_timer_t *Time1_Handle = 0;
static lv_timer_t *Time2_Handle = 0;
static lv_timer_t *Time3_Handle = 0;

static lv_obj_t* blood1;
static lv_obj_t* blood2;
static lv_obj_t* blood3;
static lv_obj_t* score_label;
static uint8_t blood_num = 3;
// static lv_timer_t *t1 = 0;
// static lv_timer_t *t2 = 0;
// static lv_timer_t *t3 = 0;

static void Bullet_Move(int step);
static void fly_plant_init(lv_obj_t* par);
static void Timer1_CallBack(lv_timer_t *t);
static void Timer2_CallBack(lv_timer_t *t);
static void Timer3_CallBack(lv_timer_t *t);
static void Bullet_Clear(void);
static void Bullet_Create(void);
static void Bullet_Init(BulletState *z, int nofz);
static void Enemy_move(void);
static void Enemy_create(void);
static void Enemy_Clear(void);
static void blood_init(lv_obj_t* par);
static void blood_update(void);
static void score_update(void);
static void score_label_init(lv_obj_t* par);
static void Game_over(void);
static void Setup()
{
    score = 0;
    blood_num = 3;
    /* 将应用窗口移至前台，使其成为活动窗口 */
    lv_obj_move_foreground(appWindow);
    // ContStyle_Setup();
    Bullet_Init(Bullet, BULLET_COUNT);
    fly_plant_init(appWindow);
    blood_init(appWindow);
    score_label_init(appWindow);
    rt_kprintf("fly start");
    PageDelay(100);
    Time1_Handle = lv_timer_create(Timer1_CallBack, 200, 0);
    Time2_Handle = lv_timer_create(Timer2_CallBack, 1500, 0);
    Time3_Handle = lv_timer_create(Timer3_CallBack, 2500, 0);
}

static void Timer1_CallBack(lv_timer_t *t)
{
    // 自动向左移动游戏角色，用于实现动态移动效果
    Bullet_Move(8);
    Enemy_move();
    blood_update();
    score_update();
    // 检测并处理敌人是否被攻击，用于游戏中的战斗逻辑处理
    // Enemy_was_attacked();
    
    // 检测并处理BOSS是否被攻击，用于游戏中的战斗逻辑处理
    // boss_was_attacked();
}
static void Timer2_CallBack(lv_timer_t *t)
{
    // 自动向左移动游戏角色，用于实现动态移动效果
    Bullet_Create();
    // Bullet_Clear();
    // Enemy_Clear();
    // 检测并处理敌人是否被攻击，用于游戏中的战斗逻辑处理
    // Enemy_was_attacked();
    
    // 检测并处理BOSS是否被攻击，用于游戏中的战斗逻辑处理
    // boss_was_attacked();
}
static void Timer3_CallBack(lv_timer_t *t)
{
    Enemy_create();
}
/**
 * 创建子弹对象
 * 
 * 此函数遍历现有的子弹数组，寻找未激活（即可用的）子弹，然后将其激活并设置初始位置
 * 子弹被激活后，会根据某个飞行物（fly1）的位置来确定子弹的初始位置，并将子弹显示在屏幕上
 * 
 * 注意：函数提前返回，只要找到并激活一个子弹，就不会继续寻找数组中剩余的子弹
 */

static void blood_update(void)
{
    //根据blood_num来更新血量，是否显示血量爱心
    if (blood_num == 2)
    {
        lv_obj_add_flag(blood1, LV_OBJ_FLAG_HIDDEN);//隐藏对象
    }
    if (blood_num == 1)
    {
        lv_obj_add_flag(blood2, LV_OBJ_FLAG_HIDDEN);//隐藏对象
    }
    if (blood_num == 0)
    {
        lv_obj_add_flag(blood3, LV_OBJ_FLAG_HIDDEN);//隐藏对象
    }
    //  lv_obj_clear_flag(user_obj, LV_OBJ_FLAG_HIDDEN);//取消隐藏

}
static void Bullet_Create(void)
{

    
    // 遍历子弹数组
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        // 检查当前子弹是否未激活
        if (Bullet[i].alive == 0)
        {
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
static void Enemy_Clear(void)
{
    int i;
    for (i = 0; i < ENEMY_COUNT; i++)
    {
        if (Enemy[i].alive == 1)
        {
            if (Enemy[i].x > 240 || Enemy[i].y > 240 || Enemy[i].x < 0)
            {
                lv_obj_del(Enemy[i].obj);
                Enemy[i].alive = 0;
            }
        }
    }
}
static void Enemy_move(void)
{
    int i;
    // 遍历所有敌机
    for (i = 0; i < ENEMY_COUNT; i++)
    {
        // 检查敌机是否存活
        if (Enemy[i].alive == 1)
        {
            // 更新敌机的x坐标和y坐标，实现移动效果
            Enemy[i].x += Enemy[i].move_x;
            Enemy[i].y += Enemy[i].move_y;
            if (Enemy[i].x > 240 || Enemy[i].y > 240 || Enemy[i].x < 0)
            {
                lv_obj_del(Enemy[i].obj);
                Enemy[i].alive = 0;
            }
            else
            {
                //判断是否和飞机相撞
                if (abs(Enemy[i].x - lv_obj_get_x(fly_plant))<20 && abs(Enemy[i].y - lv_obj_get_y(fly_plant))<20)
                {
                    lv_obj_del(Enemy[i].obj);
                    Enemy[i].alive = 0;
                    blood_num--;
                    if(blood_num == 0)
                    {
                        // lv_timer_del(Time1_Handle);
                        // lv_timer_del(Time2_Handle);
                        // lv_timer_del(Time3_Handle);
                        Game_over();
                        rt_thread_mdelay(1000);
                    }
                    // 删除LVGL对象，避免界面污染
                }
                else
                {
                    // 更新敌机对象在界面上的位置
                    lv_obj_set_pos(Enemy[i].obj, Enemy[i].x, Enemy[i].y);
                }
                
            }
        }
    }
}
//游戏失败会弹出一个窗口
static void Game_over(void)
{
    lv_obj_t *obj = lv_obj_create(lv_scr_act());
    lv_obj_set_size(obj, 200, 40);
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN);
    //居中
    lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);
    //样式设置成圆角
    lv_obj_set_style_radius(obj, 10, LV_PART_MAIN);
    //创建一个label失败
    lv_obj_t *label = lv_label_create(obj);
    lv_label_set_text(label, "Game Over");
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_32, LV_PART_MAIN);
    //标签居中
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    //停止三个定时器
    lv_timer_del(Time1_Handle);
    lv_timer_del(Time2_Handle);
    lv_timer_del(Time3_Handle);
}
static void Enemy_create(void)
{
    

    int i;
    // 遍历所有敌机，寻找未激活（alive=0）的敌机进行初始化
    for (i = 0; i < ENEMY_COUNT; i++)
    {
        // 检查敌机是否未激活
        if (Enemy[i].alive == 0)
        {
            // 激活敌机
                Enemy[i].alive = 1;
                
                // 初始化敌机位置，x坐标为-50，表示从屏幕外开始移动
                Enemy[i].y = -20;
                
                // 随机生成敌机的y坐标
                Enemy[i].x = (rand()%50)+70;
                
                // 随机生成敌机的横向移动速度
                Enemy[i].move_x = rand() % 3 -1;
                
                // 随机生成敌机的纵向移动速度
                Enemy[i].move_y = rand() % 8 +1;
                
                // 在游戏界面中创建敌机图像对象
                Enemy[i].obj = lv_img_create(appWindow);
                
                // 设置敌机图像的源文件
                // lv_img_set_src(Enemy[i].obj, "E:/sdcard/IMG/HIT_PLANT/Enemy1.bin");
                lv_img_set_src(Enemy[i].obj, &IMG_ENEMY);
                
                // 设置敌机图像的旋转角度
                // lv_img_set_angle(Enemy[i].obj, 900);
                
                // 设置敌机图像的位置
                lv_obj_set_pos(Enemy[i].obj, Enemy[i].x, Enemy[i].y);
                
                // 完成一个敌机的创建后，直接返回
            return;
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
/**
 * @brief 执行boom动画的xcb步骤
 * 
 * 该函数通过调整图像缩放比例和透明度来实现动画效果。在动画的每一帧，
 * 它根据帧编号（v）调整图像的缩放和透明度，从而实现动态的视觉效果。
 * 
 * @param var 指向lv_obj_t类型的指针，实际为动画对象的指针
 * @param v 帧编号，用于计算当前帧的缩放和透明度
 */
void boom_anim_exec_xcb(void *var, int32_t v)
{
    // 将传入的void指针转换为lv_obj_t指针，以便操作具体的动画对象
    lv_obj_t *xxx = (lv_obj_t *)var;
    
    // // 设置图像的缩放比例，根据帧编号动态调整，实现放大或缩小的效果
    // lv_img_set_zoom(xxx, 55 + v * 50);
    
    // // 设置图像的透明度，根据帧编号动态调整，实现透明度的变化
    // lv_obj_set_style_img_opa(xxx, 300 - v * 50, 0);
    if(v == 1)
    {
        lv_img_set_src(xxx, "E:/sdcard/IMG/HIT_PLANT/enemy1_down1.bin");
    }
    else if(v == 2)
    {
        lv_img_set_src(xxx, "E:/sdcard/IMG/HIT_PLANT/enemy1_down2.bin");
    }
    else if(v == 3)
    {
        lv_img_set_src(xxx, "E:/sdcard/IMG/HIT_PLANT/enemy1_down3.bin");
    }
    else if(v == 4)
    {
        lv_img_set_src(xxx, "E:/sdcard/IMG/HIT_PLANT/enemy1_down4.bin");
    }

    
}
/**
 * 在动画完成后删除对象
 * 
 * 此回调函数的目的是在动画效果完成后，删除动画所关联的对象这在某些情况下是有必要的，
 * 比如在动画结束后不再需要该对象，或者需要立即释放资源以供其他用途使用
 * 
 * @param a 指向正在进行动画的对象的动画描述结构体动画完成后，此函数会使用该动画对象作为参数来执行删除操作
 */
void boom_anim_delect_cb(lv_anim_t *a)
{
    // 从动画结构体中获取正在动画的对象，并将其转换为lv_obj_t类型
    lv_obj_t *xxx = (lv_obj_t *)a->var;
    
    // 删除动画对象从屏幕上删除对象并释放其关联的资源
    lv_obj_del(xxx);
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
            if (Bullet[i].y < 0)
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
            else
            {
                uint8_t flag = 0;
                //检测移动后的位置是否有敌机
                // 遍历所有敌人
                for (int j = 0; j < ENEMY_COUNT; j++)
                {
                    // 检查敌人是否存活
                    if (Enemy[j].alive == 1)
                    {
                        // 检查子弹和敌人的相对位置，判断是否发生碰撞
                        if (abs(Bullet[i].x - Enemy[j].x) < 10 && abs(Bullet[i].y - Enemy[j].y)  < 10)
                        {
                            score += 200;
                            flag = 1;
                            // 删除子弹对象
                            lv_obj_del(Bullet[i].obj);
                            // 标记子弹为非存活状态
                            Bullet[i].alive = 0;

                            // 删除敌人对象
                            lv_obj_del(Enemy[j].obj);
                            // 标记敌人为非存活状态
                            Enemy[j].alive = 0;
                            // 找到碰撞对象后跳出循环
                            
                            // 创建爆炸图像对象
                            lv_obj_t *boomx = lv_img_create(appWindow);
                            // // 设置爆炸图像源
                            // lv_img_set_src(boomx, &boom);
                            // // 设置爆炸图像位置
                            lv_obj_set_pos(boomx, Enemy[j].x, Enemy[j].y);


                            // 初始化动画对象
                            lv_anim_t a;
                            lv_anim_init(&a);

                            // 设置动画变量为爆炸图像对象
                            lv_anim_set_var(&a, boomx);
                            // 设置动画执行回调
                            lv_anim_set_exec_cb(&a, boom_anim_exec_xcb);
                            // 设置动画持续时间
                            lv_anim_set_time(&a, 1000);
                            // 设置动画值范围
                            lv_anim_set_values(&a, 1, 4);
                            // 设置动画完成回调
                            lv_anim_set_ready_cb(&a, boom_anim_delect_cb);
                            // 启动动画
                            lv_anim_start(&a);
                            // 找到碰撞对象后跳出循环

                            break;
                        }
                    }
                }
                if(!flag)
                {
                    // 设置子弹对象在界面上的新位置
                    lv_obj_set_pos(Bullet[i].obj, Bullet[i].x+12, Bullet[i].y-15);
                    // 重新计算子弹对象的布局位置
                    lv_obj_refr_pos(Bullet[i].obj);
                }
                
            }
            
		}
	}
}
static void fly_plant_init(lv_obj_t* par)
{
    fly_plant = lv_img_create(par);
    // lv_img_set_src(fly_plant, "E:/sdcard/IMG/HIT_PLANT/fly_plant.bin");
    lv_img_set_src(fly_plant, &IMG_PLANT);
    lv_obj_align_to(fly_plant, par, LV_ALIGN_BOTTOM_MID, 0, 0);
}


static void blood_init(lv_obj_t* par)
{
    /* 创建一个容器对象 */
    lv_obj_t* cont = lv_obj_create(par);
    /* 设置容器的大小 */
    lv_obj_set_size(cont, 100, 30);
    /* 将容器对齐到父对象的底部中间位置 */
    lv_obj_align_to(cont, par, LV_ALIGN_TOP_RIGHT, 0, 0);
    /* 设置容器边框宽度 */
    lv_obj_set_style_border_width(cont, 0, LV_PART_MAIN);
    /* 清除容器的可滚动标志 */
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    //设置背景透明
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, LV_PART_MAIN);

    blood1 = lv_img_create(cont);
    lv_img_set_src(blood1, "E:/sdcard/IMG/HIT_PLANT/blood.bin");
    lv_obj_align_to(blood1, cont, LV_ALIGN_LEFT_MID, 0, 0);

     blood2 = lv_img_create(cont);
    lv_img_set_src(blood2, "E:/sdcard/IMG/HIT_PLANT/blood.bin");
    lv_obj_align_to(blood2, cont, LV_ALIGN_CENTER, 0, 0);

     blood3 = lv_img_create(cont);
    lv_img_set_src(blood3, "E:/sdcard/IMG/HIT_PLANT/blood.bin");
    lv_obj_align_to(blood3, cont, LV_ALIGN_RIGHT_MID, 0, 0);
}


static void score_label_init(lv_obj_t* par)
{
    /* 创建一个容器对象 */
    lv_obj_t* cont = lv_obj_create(par);
    /* 设置容器的大小 */
    lv_obj_set_size(cont, 120, 30);
    /* 将容器对齐到父对象的底部中间位置 */
    lv_obj_align_to(cont, par, LV_ALIGN_TOP_LEFT, 0, 0);
    /* 设置容器边框宽度 */
    lv_obj_set_style_border_width(cont, 0, LV_PART_MAIN);
    /* 清除容器的可滚动标志 */
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    //设置背景透明
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, LV_PART_MAIN);

    score_label = lv_label_create(cont);
    lv_label_set_text(score_label, "0");
    lv_obj_align_to(score_label, cont, LV_ALIGN_LEFT_MID, 0, 0);
    //使用字号为20的字体
    
    lv_obj_set_style_text_font(score_label, &lv_font_montserrat_20, LV_PART_MAIN);
    //设置显示字体为白色
    lv_obj_set_style_text_color(score_label, lv_color_white(), LV_PART_MAIN);

}
static void score_update(void)
{
    lv_label_set_text_fmt(score_label, "%d", score);
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
        rt_err_t uwRet = RT_EOK;
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
            plant_next_x+=8;
            // 向右翻页时，推入主菜单页面
            // PM_Push(Page, PAGE_MainMenu);
            // PM_Push(Page, PAGE_MainMenu);
        }
        
        if(key == PAGE_KEY_LEFT)
        {
            plant_next_x-=8;
            // 向左翻页时，推入硬件测试页面
            // PM_Push(Page, PAGE_Main);
        }
        if(key == PAGE_KEY_UP)
        {
            plant_next_y-=8;
            // rt_kprintf("key up");
        }
        if(key == PAGE_KEY_DOWN)
        {
            plant_next_y+=8;
            // rt_kprintf("key down");
        }
        if(plant_next_y < 0)
        {
            plant_next_y = 0;
        }
        if(plant_next_y >210)
        {
            plant_next_y = 210;
        }
        if(plant_next_x < 0)
        {

            plant_next_x = 0;
        }
        if(plant_next_x >210)
        {
            
            plant_next_x = 210;
        }
        if(blood_num > 0)
        {
            lv_obj_set_pos(fly_plant, plant_next_x, plant_next_y);
        }
        else
        {
            PM_Pop(Page);
        }
        // uwRet = rt_mb_send(op_mb, (rt_uint32_t)&test_str1);
        // if(uwRet == RT_EOK)
        //     rt_kprintf("邮箱信息发送成功！\n\n");
        // else
        //     rt_kprintf("邮箱信息发送失败！\n\n");
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
