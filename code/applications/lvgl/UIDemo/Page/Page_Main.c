#include "../DisplayPrivate.h"

// #include "aht10.h"
PAGE_EXPORT(Main);

LV_IMG_DECLARE(IMG_RTTHREAD);
// LV_IMG_DECLARE(IMG_Power);
// LV_IMG_DECLARE(IMG_CHNSecond);



static lv_obj_t* contBatt;


static lv_obj_t* contDate;
static lv_obj_t* labelDate;
static lv_obj_t* labelWeek;



// static aht10_device_t aht10_dev;


static lv_style_t* cont_style;/* 定义一个静态局部变量，用于存储容器样式指针 */

/**
 * 初始化容器的样式
 * 
 * 该函数负责初始化一个 lv_style_t 类型的静态变量 style，并设置其相关属性，
 * 最后将该样式指针赋值给全局变量 cont_style，供后续使用。
 * 
 * @note 由于该函数内部使用了静态变量 style，因此该函数只应在初始化时调用一次。
 */
static void ContStyle_Setup(void)
{
    /* 初始化样式对象 */
    static lv_style_t style;
    lv_style_init(&style);
    
    /* 设置容器背景颜色为黑色 */
    lv_style_set_bg_color(&style, LV_COLOR_BLACK);
    
    /* 设置容器边框宽度为2像素 */
    lv_style_set_border_width(&style, 2);
    
    /* 设置容器圆角半径为0，即矩形形状 */
    lv_style_set_radius(&style, 0);
    
    /* 设置容器边框颜色为白色 */
    lv_style_set_border_color(&style, LV_COLOR_WHITE);
    
    /* 设置容器文本颜色为白色 */
    lv_style_set_text_color(&style, LV_COLOR_WHITE);
    
    /* 移除容器内部边框宽度，聚焦时除外 */
    lv_style_set_border_width(&style, 0);
    
    /* 设置容器内边距为0，使内容紧贴边框 */
    lv_style_set_pad_all(&style, 0);
    
    /* 将初始化好的样式指针赋值给全局变量，供其他地方使用 */
    cont_style = &style;
}
/**
 * 创建一个显示日期和星期的容器
 * @param par 父对象，用于将新创建的容器添加为子对象
 */
static void Data_Week_Create(lv_obj_t* par)
{
    /* 创建一个容器对象 */
    lv_obj_t* cont = lv_obj_create(par);
    /* 为容器应用样式 */
    lv_obj_add_style(cont, cont_style, LV_PART_MAIN);
    /* 设置容器的大小 */
    lv_obj_set_size(cont, 220, 40);
    /* 将容器对齐到父对象的底部中间位置 */
    lv_obj_align_to(cont, contBatt, LV_ALIGN_TOP_MID, 5, 10);
    /* 设置容器边框宽度 */
    lv_obj_set_style_border_width(cont, 2, LV_PART_MAIN);
    /* 清除容器的可滚动标志 */
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    /* 保存容器对象的引用 */
    contDate = cont;

    /* 在容器中创建一个标签对象用于显示日期 */
    lv_obj_t* label = lv_label_create(cont);
    /* 设置标签的字体样式 */
    lv_obj_set_style_text_font(label, &Font_RexBold_28, LV_PART_MAIN);
    /* 设置标签的初始文本为"00.00.00" */
    lv_label_set_text(label, "00.00.00");
    /* 将标签对齐到容器的左侧中间位置 */
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 10, 0);
    /* 清除标签的可滚动标志 */
    lv_obj_clear_flag(label, LV_OBJ_FLAG_SCROLLABLE);

    /* 在容器中创建一个标签对象用于显示星期 */
    lv_obj_t* label2 = lv_label_create(cont);
    /* 设置标签的字体样式 */
    lv_obj_set_style_text_font(label2, &Font_RexBold_28, LV_PART_MAIN);
    /* 设置标签的初始文本为"SUM" */
    lv_label_set_text(label2, "SUM");
    /* 将标签对齐到容器的右侧中间位置 */
    lv_obj_align(label2, LV_ALIGN_RIGHT_MID, -10, 0);
    /* 清除标签的可滚动标志 */
    lv_obj_clear_flag(label2, LV_OBJ_FLAG_SCROLLABLE);

    /* 保存标签对象的引用 */
    labelDate = label;
}

static void Time_Create(lv_obj_t* par)
{
    /* 创建一个容器对象 */
    lv_obj_t* cont = lv_obj_create(par);
    /* 为容器应用样式 */
    lv_obj_add_style(cont, cont_style, LV_PART_MAIN);
    /* 设置容器的大小 */
    lv_obj_set_size(cont, 140, 80);
    /* 将容器对齐到父对象的底部中间位置 */
    lv_obj_align_to(cont, contBatt, LV_ALIGN_LEFT_MID, 10, -20);
    /* 设置容器边框宽度 */
    lv_obj_set_style_border_width(cont, 3, LV_PART_MAIN);
    /* 清除容器的可滚动标志 */
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    // lv_style_set_border_color
    lv_obj_set_style_border_color(cont, lv_color_make(0xFF, 0xEC, 0xF1), LV_STATE_DEFAULT);
    /* 保存容器对象的引用 */
    // contDate = cont;

    /* 在容器中创建一个标签对象用于显示日期 */
    lv_obj_t* label = lv_label_create(cont);
    /* 设置标签的字体样式 */
    lv_obj_set_style_text_font(label, &Font_RexBold_68, LV_PART_MAIN);
    /* 设置标签的初始文本为"00.00.00" */
    lv_label_set_text(label, "1400");
    lv_obj_set_style_text_color(label, lv_color_make(0xFF, 0xEC, 0xF1), LV_PART_MAIN);
    /* 将标签对齐到容器的左侧中间位置 */
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 10, 0);
    /* 清除标签的可滚动标志 */
    lv_obj_clear_flag(label, LV_OBJ_FLAG_SCROLLABLE);
}
static void LOGO_Create(lv_obj_t* par)
{
    lv_obj_t* img = lv_img_create(par);
    lv_img_set_src(img, &IMG_RTTHREAD);
    lv_obj_align_to(img, par, LV_ALIGN_BOTTOM_LEFT, 10, -15);
}





/**
 * @brief 定义了一个结构体，用于存储动画时间线的信息
 * 
 * 此结构体包含了动画开始时间、目标对象、动画执行回调、动画的起始和结束值、动画持续时间
 * 以及动画路径回调等信息，用于精确控制动画的执行过程。
 */
typedef struct lv_anim_timeline_info
{
    uint32_t start_time; /**< 动画的开始时间，以毫秒为单位 */
    lv_obj_t* obj; /**< 动画作用的对象 */

    lv_anim_exec_xcb_t exec_cb; /**< 动画执行回调函数，用于更新对象的属性 */
    int32_t start; /**< 动画的起始值 */
    int32_t end; /**< 动画的结束值 */
    uint16_t duration; /**< 动画的持续时间，以毫秒为单位 */
    lv_anim_path_cb_t path_cb; /**< 动画路径回调函数，用于计算动画的中间值 */

}lv_anim_timeline_info_t;












/**
 * @brief 初始化界面并设置相关控件
 * 
 * 该函数主要负责应用界面的初始化，包括将应用窗口移至前台、设置容器样式、创建各个控件等。
 * 它还尝试打开RTC设备以获取时间信息，并创建日期显示线程。
 */
static void Setup()
{
    /* 将应用窗口移至前台，使其成为活动窗口 */
    lv_obj_move_foreground(appWindow);
    /* 设置应用的整体样式 */
    ContStyle_Setup();

    Data_Week_Create(appWindow);
    Time_Create(appWindow);
    LOGO_Create(appWindow);
    // GIF_Create(appWindow);
    // ContWeek_Create(appWindow);
    /* 延迟100毫秒，以便界面元素有足够时间渲染 */
    PageDelay(100);


}

static void Exit()
{
	// rt_thread_delete(dial_thread);
	// rt_mutex_delete (dynamic_mutex);
	// aht10_deinit(aht10_dev);
    // PagePlayAnim(true);
    // lv_anim_timeline_del(anim_timeline);
    lv_obj_clean(appWindow);
}


static void Event(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
	if(code == LV_EVENT_KEY)
	{
		uint8_t key = *(uint8_t*)lv_event_get_user_data(e);
		rt_kprintf("lv_indev_get_key %d \n", key);
		if(key == PAGE_KEY_RIGHT)
		{
			PM_Push(Page, PAGE_MainMenu);
		}
		
		if(key == PAGE_KEY_LEFT)
		{
			PM_Push(Page, PAGE_HWTest);
		}
		
	}
    if (code == LV_EVENT_PRESSING)
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
        if (dir == LV_DIR_RIGHT)
        {
            PM_Push(Page, PAGE_MainMenu);
        }
    }
}