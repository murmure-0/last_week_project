
#include "../DisplayPrivate.h"

PAGE_EXPORT(MainMenu);

static lv_obj_t* contApps;
static lv_anim_timeline_t* anim_timeline = NULL;

LV_IMG_DECLARE(IMG_SDCard);
// LV_IMG_DECLARE(IMG_RGBLed);
// LV_IMG_DECLARE(IMG_WiFi);
// LV_IMG_DECLARE(IMG_EnvMonitor);
// LV_IMG_DECLARE(IMG_Attitude);
// LV_IMG_DECLARE(IMG_Setting);
typedef struct
{
    const char* src_img;
    const char* name;
    const uint8_t pageID;
    int bg_color;
    lv_obj_t* obj;
} AppICON_TypeDef;

#define APP_DEF(address, name, color) {address, #name, PAGE_##name, color, NULL}
#define APP_ICON_SIZE 90
#define APP_ICON_ANIM_TIME 100
#define MENU_ROW 2
#define MENU_COL 3
#define APP_ICON_GAP 20

static uint8_t ICON_NowSelIndex = 0;


static AppICON_TypeDef AppICON_Grp[] =
{
    APP_DEF("E:/sdcard/IMG/HIT_PLANT/icon_plant.bin", HitPlant, 0x000000),
	APP_DEF("E:/sdcard/IMG/HIT_PLANT/icon_block.bin", HitPlant, 0x000000),
    // APP_DEF(RGBLed, 0x0400A5),
	// APP_DEF(EnvMonitor, 0xFF7FC6),
	// APP_DEF(Attitude, 0x4EB9AB),
	// APP_DEF(&Setting, Setting , 0x0040DD),
};

static lv_style_t* cont_style;
static void ContStyle_Setup(void)
{
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_bg_color(&style, LV_COLOR_BLACK);
    lv_style_set_border_width(&style, 2);
    lv_style_set_radius(&style, 0);
    lv_style_set_border_color(&style, LV_COLOR_WHITE);
    lv_style_set_text_color(&style, LV_COLOR_WHITE);
    lv_style_set_border_width(&style, 0);
    lv_style_set_pad_all(&style, 0);
    cont_style = &style;
}

/**
 * 实现图标点击时的动画效果
 * 
 * @param img 图像对象指针
 * @param ispress 按下状态，用于确定是否执行点击动画
 * 
 * 当图标被点击时，会执行一个缩放动画，以提供用户交互的视觉反馈
 * 动画通过调整图标的缩放级别来实现，点击时缩小，释放时恢复原状
 * 动画时长和过渡效果都经过精心设定，以确保动画的流畅性和一致性
 */
static void AppClickAnim(lv_obj_t* img, bool ispress)
{
    // 初始化动画对象
    lv_anim_t a;
    lv_anim_init(&a);
    // 设置动画对象的作用目标
    lv_anim_set_var(&a, img);
    // 设置动画执行的回调函数，用于在动画每一帧更新图像的缩放值
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_img_set_zoom);
    // 根据当前是否按下状态，设置动画的起始和结束值
    lv_anim_set_values(&a, lv_img_get_zoom(img), ispress ? 190 : LV_IMG_ZOOM_NONE);
    // 设置动画的持续时间
    lv_anim_set_time(&a, APP_ICON_ANIM_TIME);

    // 设置动画的过渡效果为缓入缓出，使得动画更加自然
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);

    // 启动动画
    lv_anim_start(&a);
}

/**
 * @brief 图标事件处理函数
 * 
 * 该函数用于处理图标对象上的事件，如按下、释放和点击等。
 * 根据不同的事件代码，执行相应的动画效果或页面跳转操作。
 * 
 * @param e 事件对象，包含事件相关的所有信息
 */
static void AppICON_EventHandler(lv_event_t* e)
{
    // 获取事件对应的目标对象
    lv_obj_t* obj = lv_event_get_current_target(e);
    // 获取目标对象的第一个子对象，通常用于显示图标图像
    lv_obj_t* img = lv_obj_get_child(obj, 0);

    // 获取事件代码，用于判断触发了哪种事件
    lv_event_code_t code = lv_event_get_code(e);
    // 获取图标对象的用户数据，通常用于存储自定义的数据结构
    AppICON_TypeDef* appIcon = (AppICON_TypeDef*)lv_obj_get_user_data(obj);

    // 根据不同的事件代码执行相应的操作
    if(code == LV_EVENT_PRESSED)
    {
        // 当图标被按下时，启动点击动画
        AppClickAnim(img, true);
    }
    else if(code == LV_EVENT_RELEASED || code == LV_EVENT_PRESS_LOST)
    {
        // 当图标被释放或按下状态丢失时，停止点击动画
        AppClickAnim(img, false);
    }
    else if(code == LV_EVENT_SHORT_CLICKED)
    {
        // 获取图标关联的页面ID
        uint8_t pageID = appIcon->pageID;

        // 如果页面ID为无效值，则不执行任何操作
        if (pageID == PAGE_NONE)
            return;

        // 根据页面ID执行相应的页面跳转操作
        if (pageID == PAGE_Setting)
        {
            // 跳转到设置页面
            PM_Push(Page, PAGE_Setting);
        }
        else if (pageID == PAGE_SDCard)
        {
            // 跳转到SD卡页面
            PM_Push(Page, PAGE_SDCard);
        }
//        else if (pageID == PAGE_WiFi)
//        {
//            // 跳转到WiFi页面
//            PM_Push(Page, PAGE_WiFi);
//        }
    }
}

static void AppICON_Create(lv_obj_t* par)
{

    int label_h = 16;

    // 创建一个对象，并设置其参数为LV_ALIGN_TOP_MID，宽为WINDOW_WIDTH，高为AppICON_Grp大小加1乘以(APP_ICON_SIZE加APP_ICON_GAP)
    // 不允许滚动，但允许事件冒泡
    lv_obj_t* obj = lv_obj_create(par); // 创建一个对象
    lv_obj_align(obj, LV_ALIGN_TOP_MID, 0, 0); // 将对象居顶居中对齐
    lv_obj_set_size(obj, WINDOW_WIDTH, (__Sizeof(AppICON_Grp) + 1 )* (APP_ICON_SIZE + APP_ICON_GAP)); // 设置对象的尺寸
    lv_obj_add_style(obj, cont_style, LV_PART_MAIN); // 为对象添加样式
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE); // 清除对象的滚动标志
    lv_obj_add_flag(obj, LV_OBJ_FLAG_EVENT_BUBBLE); // 允许对象的事件冒泡
    contApps = obj;

    for (int i = 0; i < __Sizeof(AppICON_Grp); i++)
    {
        // 创建一个虚拟对象，用于承载图标和相关信息
        lv_obj_t* obj_vir = lv_obj_create(contApps);
        // 移除边框
        lv_obj_set_style_border_width(obj_vir, 0, LV_PART_MAIN);
         // 设置对象的尺寸
        lv_obj_set_size(obj_vir, APP_ICON_SIZE, APP_ICON_SIZE);
        //lv_obj_set_drag_parent(obj_vir, true);
        //lv_obj_add_event_cb(obj_vir, (lv_event_cb_t)AppICON_EventHandler, LV_EVENT_ALL, NULL);
        // 设置用户数据，以便于事件处理时获取到对应的图标信息
        lv_obj_set_user_data(obj_vir, &(AppICON_Grp[i]));
         // 移除滚动标志
        lv_obj_clear_flag(obj_vir, LV_OBJ_FLAG_SCROLLABLE);
        // 定义列数和图标间的间隔
        int col = MENU_COL;
        // 设置图标对象的对齐方式，使其在界面上垂直居中排列
        lv_coord_t interval_pixel = APP_ICON_SIZE + APP_ICON_GAP;
        lv_obj_align(
            obj_vir,
            LV_ALIGN_TOP_MID,
            -40,
            i* interval_pixel
        );
        // 设置背景颜色
        lv_obj_set_style_bg_color(obj_vir, lv_color_hex(AppICON_Grp[i].bg_color), LV_PART_MAIN);
        // 添加事件回调，用于处理图标的点击等交互事件
        lv_obj_add_event_cb(obj_vir, (lv_event_cb_t)AppICON_EventHandler, LV_EVENT_ALL, NULL);

         // 创建图像对象，并设置图像源
        lv_obj_t* img = lv_img_create(obj_vir);
        lv_img_set_src(img, AppICON_Grp[i].src_img);
        // 将图像居中对齐
        lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
        // 创建标签对象，用于显示图标名称
        lv_obj_t* label = lv_label_create(contApps);
         // 设置文本对齐方式
        lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);
        // 设置标签宽度
        lv_obj_set_width(label, 150);
        // 设置标签文本内容
        lv_label_set_text(label, AppICON_Grp[i].name);
        // 设置文本颜色
		lv_obj_set_style_text_color(label, LV_COLOR_WHITE, LV_PART_MAIN);
        // 设置标签相对于图标的位置
        lv_obj_align_to(label, obj_vir, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
        // 设置标签文本字体
		lv_obj_set_style_text_font(label, &lv_font_montserrat_20, LV_PART_MAIN);
        // 设置标签文本的换行模式
		lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
        // 将创建的对象与图标组中的图标关联起来
        AppICON_Grp[i].obj = obj_vir;
    }

}

/**
 * @brief 移动图标焦点
 * 
 * 该函数通过动画效果移动图标焦点。它计算目标位置并使用动画来实现平滑移动。
 * 动画使用缓动函数来实现更自然的运动效果。
 * 
 * @param iconIndex 图标索引，用于计算目标位置
 */
static void ICON_Grp_MoveFouce(uint8_t iconIndex)
{
    // 计算图标焦点的目标位置
    int target_y = - iconIndex * (APP_ICON_SIZE + APP_ICON_GAP) + (WINDOW_HEIGHT - APP_ICON_SIZE)/2;

    // 初始化动画结构体
    lv_anim_t a;
    lv_anim_init(&a);
    // 设置动画变量为图标容器
    lv_anim_set_var(&a, contApps);
    // 设置动画执行回调函数
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
    // 设置动画的起始值和目标值
    lv_anim_set_values(&a, lv_obj_get_y(contApps), target_y);
    // 设置动画持续时间
    lv_anim_set_time(&a, APP_ICON_ANIM_TIME);

    // 设置动画路径回调函数，实现缓动效果
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);

    // 启动动画
    lv_anim_start(&a);
    //PageDelay(APP_ICON_ANIM_TIME);
}

/**
 * 移动图标选择界面
 * 
 * 该函数用于处理图标界面的移动操作。它通过调整当前选择的图标索引来实现移动，
 * 索引的调整量由参数dir指定。调整后，确保索引值在有效范围内（0到AppICON_Grp大小-1）。
 * 
 * 参数:
 * @param dir int8_t 移动方向，正值向右移动，负值向左移动
 * 
 * 返回值:
 * 无
 * 
 * 注意: 该函数使用了静态变量ICON_NowSelIndex来记录当前选择的图标索引
 */
static void ICON_Grp_Move(int8_t dir)
{
    // 调整并更新当前选择的图标索引，确保其在有效范围内
    __ValuePlus(ICON_NowSelIndex, dir, 0, __Sizeof(AppICON_Grp)-1);
    
    // 打印当前选择的图标索引，用于调试
    rt_kprintf("appIcon move to %d\n",ICON_NowSelIndex);
    
    // 调用函数将焦点移动到新的位置
    ICON_Grp_MoveFouce(ICON_NowSelIndex);
}

/**
 * 创建和初始化动画的时间线
 * 该函数负责初始化动画变量，设置动画参数，并创建动画时间线
 * 通过动画时间线来管理动画的执行顺序和时间，实现平滑的过渡效果
 */
static void MainPageAnim_Create()
{
    // 获取应用容器的当前Y坐标，用于计算动画的起始位置
    int32_t cont_rey = lv_obj_get_y(contApps);

    // 将应用容器的Y坐标设置为屏幕高度，准备动画的起始位置
    lv_obj_set_y(contApps, WINDOW_HEIGHT);

    // 初始化动画对象a1
    lv_anim_t a1;
    lv_anim_init(&a1);
    // 设置动画对象a1的作用目标为应用容器
    lv_anim_set_var(&a1, contApps);
    // 设置动画的起始值和结束值，实现从屏幕底部向上滑入的效果
    lv_anim_set_values(&a1, WINDOW_HEIGHT,  ((APP_ICON_SIZE + APP_ICON_GAP) + (WINDOW_HEIGHT - APP_ICON_SIZE)/2));
    // 设置动画在更新期间不提前应用
    lv_anim_set_early_apply(&a1, false);
    // 设置动画的执行回调函数，用于更新目标对象的位置
    lv_anim_set_exec_cb(&a1, (lv_anim_exec_xcb_t)lv_obj_set_y);
    // 设置动画的路径回调函数，实现弹跳效果的动画
    lv_anim_set_path_cb(&a1, lv_anim_path_ease_out);
    // 设置动画的持续时间为300毫秒
    lv_anim_set_time(&a1, 300);

    // 创建动画时间线对象
    anim_timeline = lv_anim_timeline_create();
    // 将动画对象a1添加到时间线中，指定其在时间线中的起始时间为0
    lv_anim_timeline_add(anim_timeline, 0, &a1);
}

/**
  * @brief  页面初始化事件
  * @param  无
  * @retval 无
  */
static void Setup(void)
{
    /*将此页面移到前台*/
    lv_obj_move_foreground(appWindow);
	ContStyle_Setup();
    //ImgAppShadow_Create(appWindow);
    AppICON_Create(appWindow);
//    MainPageAnim_Create();
//    PageAnim_Play(anim_timeline, false);
	lv_obj_set_y(contApps, WINDOW_HEIGHT);
	PageDelay(10);
	ICON_Grp_MoveFouce(ICON_NowSelIndex);
}

/**
  * @brief  页面退出事件
  * @param  无
  * @retval 无
  */
static void Exit(void)
{
//    PageAnim_Play(anim_timeline, true);
//    lv_anim_timeline_del(anim_timeline);
    lv_obj_clean(appWindow);
}

/**
  * @brief  页面事件
  * @param  obj:发生事件的对象
  * @param  event:事件编号
  * @retval 无
  */
static void Event(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
	if(code == LV_EVENT_KEY)
	{
		uint8_t key = *(uint8_t*)lv_event_get_user_data(e);
		rt_kprintf("lv_indev_get_key %d \n", key);
		if(key == PAGE_KEY_LEFT)
		{
            LV_LOG("Page->Pop()\n");
            PM_Pop(Page);
		}
		
		if(key == PAGE_KEY_UP)
		{
            ICON_Grp_Move(-1);
		}
		
		if(key == PAGE_KEY_DOWN)
		{
            ICON_Grp_Move(+1);
		}
		
		if(key == PAGE_KEY_RIGHT)
		{

			PM_Push(Page, AppICON_Grp[ICON_NowSelIndex].pageID);

		}
		
	}
    if(code == LV_EVENT_PRESSING)
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
        if (dir == LV_DIR_RIGHT)
        {
            LV_LOG("Page->Pop()\n");
            PM_Pop(Page);
        }
    }
}
