#include "../DisplayPrivate.h"
// #include "aht10.h"
PAGE_EXPORT(Dial);

LV_IMG_DECLARE(IMG_Heart);
LV_IMG_DECLARE(IMG_Power);
LV_IMG_DECLARE(IMG_CHNSecond);

/**
 * @brief 时间效果标签结构体
 * 
 * 此结构体用于存储与时间效果相关的标签对象及其动画属性。
 * 
 * @var lv_obj_t *label_1
 * 第一个标签对象指针。
 * 
 * @var lv_obj_t *label_2
 * 第二个标签对象指针。
 * 
 * @var lv_anim_t anim_now
 * 当前正在播放的动画。
 * 
 * @var lv_anim_t anim_next
 * 下一个将要播放的动画。
 * 
 * @var lv_coord_t y_offset
 * Y 轴偏移量。
 * 
 * @var uint8_t value_last
 * 上一次的数值。
 * 
 * @var lv_coord_t x_offset
 * X 轴偏移量。
 */
typedef struct{
    lv_obj_t * label_1;
    lv_obj_t * label_2;
    lv_anim_t anim_now;
    lv_anim_t anim_next; 
    lv_coord_t y_offset;
    uint8_t value_last;
    lv_coord_t x_offset;
}lv_label_time_effect_t;


static lv_obj_t* contBatt;
static lv_obj_t* ledBattGrp[10];

static lv_obj_t* contDate;
static lv_obj_t* labelDate;
static lv_obj_t* labelWeek;

static lv_obj_t* contTime;
static lv_label_time_effect_t labelTimeEffect[4];

static lv_obj_t* imgPower;
static lv_obj_t* ledSecGrp[2];

static lv_obj_t* contHeartRate;
static lv_obj_t* labelHeartRate;

static lv_obj_t* contSteps;
static lv_obj_t* labelSteps;

static lv_obj_t* imgCHN;

static Clock_Value_t Clock;

static lv_label_time_effect_t labelTimeEffect[4];
static lv_obj_t* ledSecGrp[2];
static lv_obj_t* Cont;
static lv_obj_t* contTime;
static lv_obj_t* labelDate;
static lv_obj_t* labelWeather;
static lv_anim_timeline_t* anim_timeline = NULL;

static rt_mutex_t dynamic_mutex = RT_NULL;
static rt_thread_t dial_thread = RT_NULL;
// static aht10_device_t aht10_dev;

static void lv_label_time_effect_check_value(lv_label_time_effect_t * effect, uint8_t value);
static void lv_label_time_effect_init(
    lv_label_time_effect_t* effect,
    lv_obj_t* cont,
    lv_obj_t* label_copy,
    uint16_t anim_time,
    lv_coord_t x_offset
);
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
 * 根据电池使用百分比更新电池LED显示
 * 
 * 该函数通过将使用百分比映射到LED数组的索引来控制LED的点亮状态。
 * 使用的LED数量取决于电池LED组的大小。
 * 
 * @param usage 电池的使用百分比，范围为0到100。
 */
static void ContBatt_UpdateBattUsage(uint8_t usage)
{
    /* 将电池使用百分比映射到LED数组的索引上，确定最多有多少个LED应该被点亮 */
    int8_t maxIndexTarget = __Map(usage, 0, 100, 0, __Sizeof(ledBattGrp));

    /* 遍历LED数组，根据映射的结果点亮或熄灭LED */
    for (int i = 0; i < __Sizeof(ledBattGrp); i++)
    {
        lv_obj_t* led = ledBattGrp[i];

        /* 如果当前索引小于等于目标索引，则点亮LED，否则熄灭LED */
        (i < maxIndexTarget) ? lv_led_on(led) : lv_led_off(led);
    }
}

/**
 * @brief 动画回调函数，用于更新电池使用情况
 * 
 * 此函数作为一个回调函数被动画库调用，其目的是更新电池的使用百分比。
 * 它不直接对外公开，而是通过动画库的机制被内部调用。
 * 
 * @param obj 回调函数的自定义对象参数，本例中未使用。
 * @param usage 表示电池的使用百分比，作为一个整数传递。
 */
static void ContBatt_AnimCallback(void* obj, int16_t usage)
{
    // 调用ContBatt_UpdateBattUsage函数来更新电池使用百分比
    ContBatt_UpdateBattUsage(usage);
}

/**
 * 创建一个包含电池状态图标的容器
 * @param par 父对象，用于指定新创建对象的父对象
 */
static void ContBatt_Create(lv_obj_t* par)
{
    // 创建一个容器对象
    lv_obj_t* cont = lv_obj_create(par);
    // 为容器添加样式
    lv_obj_add_style(cont, cont_style, LV_PART_MAIN);
    // 设置容器的大小
    lv_obj_set_size(cont, 222, 20);
    // 将容器定位在顶部中间位置
    lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 5);
    // 移除容器的可滚动标志
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    // 将容器对象赋值给全局变量contBatt
    contBatt = cont;

    // 在容器中创建一个图像对象，表示电源图标
    lv_obj_t* img = lv_img_create(cont);
    lv_img_set_src(img, &IMG_Power);
    lv_obj_align(img, LV_ALIGN_LEFT_MID, 0, 0);
    // 将图像对象赋值给全局变量imgPower
    imgPower = img;

    // 计算LED灯的宽度和高度
    const lv_coord_t led_w = (lv_obj_get_width(contBatt) - lv_obj_get_width(img)) / 10 - 2;
    const lv_coord_t led_h = lv_obj_get_height(contBatt);

    // 创建一系列的LED灯对象，用于表示电池电量
    for (int i = 0; i < __Sizeof(ledBattGrp); i++)
    {
        // 创建一个LED灯对象
        lv_obj_t* led = lv_led_create(contBatt);
        // 设置LED灯的大小
        lv_obj_set_size(led, 14, 20);
        // 设置LED灯的样式，去除边框和阴影
        lv_obj_set_style_radius(led, 0, LV_PART_MAIN);
        lv_obj_set_style_border_width(led, 0, LV_PART_MAIN);
        lv_obj_set_style_shadow_width(led, 0, LV_PART_MAIN);
        // 设置LED灯的颜色为红色
        lv_led_set_color(led, LV_COLOR_RED);
        // 根据LED灯的索引位置，将其定位在容器中适当的位置
        if(i==0)
            lv_obj_align(led, LV_ALIGN_RIGHT_MID, 0, 0);
        else
            lv_obj_align_to(led, ledBattGrp[i - 1], LV_ALIGN_OUT_LEFT_MID, -2, 0);
        // 将LED灯设置为关闭状态
        lv_led_off(led);
        // 将LED灯对象添加到全局数组ledBattGrp中
        ledBattGrp[i] = led;
    }
}

/**
 * 创建一个容器，用于显示一周中的某一天
 * @param par 父对象，通常是屏幕或另一个容器
 */
static void ContWeek_Create(lv_obj_t* par)
{
    /* 创建一个容器对象 */
    lv_obj_t* cont = lv_obj_create(par);
    /* 为容器应用样式 */
    lv_obj_add_style(cont, cont_style, LV_PART_MAIN);
    /* 设置容器大小为父对象宽度的一半，高度与父对象相同 */
    lv_obj_set_size(cont, lv_obj_get_width(par) / 2, lv_obj_get_height(par));
    /* 将容器对齐到父对象的右侧中间位置 */
    lv_obj_align(cont, LV_ALIGN_RIGHT_MID, 0, 0);
    /* 设置容器的背景颜色为白色 */
    lv_obj_set_style_bg_color(cont, LV_COLOR_WHITE, LV_PART_MAIN);
    /* 禁止容器的滚动功能 */
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);

    /* 在容器中创建一个标签对象，用于显示星期名称 */
    lv_obj_t* label = lv_label_create(cont);
    /* 为标签设置字体样式 */
    lv_obj_set_style_text_font(label, &Font_RexBold_28, LV_PART_MAIN);
    /* 为标签设置文本颜色 */
    lv_obj_set_style_text_color(label, LV_COLOR_BLACK, LV_PART_MAIN);
    /* 设置标签的文本为“SUN” */
    lv_label_set_text(label, "SUN");
    /* 将标签居中对齐在容器中 */
    lv_obj_align(label,LV_ALIGN_CENTER, 0, 0);
    /* 禁止标签的滚动功能 */
    lv_obj_clear_flag(label, LV_OBJ_FLAG_SCROLLABLE);
    /* 保存标签对象的引用，供后续使用 */
    labelWeek = label;
}

/**
 * 创建一个显示日期的容器
 * @param par 父对象，用于将新创建的容器添加为子对象
 */
static void ContDate_Create(lv_obj_t* par)
{
    /* 创建一个容器对象 */
    lv_obj_t* cont = lv_obj_create(par);
    /* 为容器应用样式 */
    lv_obj_add_style(cont, cont_style, LV_PART_MAIN);
    /* 设置容器的大小 */
    lv_obj_set_size(cont, 222, 32);
    /* 将容器对齐到父对象的底部中间位置 */
    lv_obj_align_to(cont, contBatt, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
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

    /* 保存标签对象的引用 */
    labelDate = label;

    /* 创建并显示一周中的日期容器 */
    ContWeek_Create(contDate);
}

static void LabelDate_Update(int year, int mon, int day, int week)
{
    lv_label_set_text_fmt(labelDate, "%02d.%02d.%02d", year % 100, mon, day);

    const char* week_str[] = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };
    lv_label_set_text(labelWeek, week_str[week]);
}

static void LabelTime_Update(int h,int m)
{
    
    lv_label_time_effect_check_value(&labelTimeEffect[3], m % 10);
    
    lv_label_time_effect_check_value(&labelTimeEffect[2], m / 10);

    lv_label_time_effect_check_value(&labelTimeEffect[1], h % 10);
    
    lv_label_time_effect_check_value(&labelTimeEffect[0], h / 10);

    lv_led_toggle(ledSecGrp[0]);
    lv_led_toggle(ledSecGrp[1]);
}

static void LabelTime_Create(lv_obj_t* par)
{
    const lv_coord_t x_mod[4] = { -70, -30, 30, 70 };
    for (int i = 0; i < __Sizeof(labelTimeEffect); i++)
    {
        lv_obj_t* label = lv_label_create(par);
        lv_label_set_text(label, "0");
        lv_obj_align(label, LV_ALIGN_CENTER, x_mod[i], 0);
        //lv_obj_set_auto_realign(label, true);

        lv_label_time_effect_init(&labelTimeEffect[i], par, label, 500, x_mod[i]);
    }

    lv_obj_t* led = lv_led_create(par);
    lv_obj_set_size(led, 8, 8);
    //lv_obj_set_hidden(led, true);
    lv_obj_set_style_radius(led, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(led, 0, LV_PART_MAIN);
    lv_led_set_color(led, LV_COLOR_RED);
    lv_obj_set_style_shadow_width(led, 0, LV_PART_MAIN);
    lv_obj_align(led, LV_ALIGN_CENTER, 0, -15);
    ledSecGrp[0] = led;

    led = lv_led_create(par);
    lv_obj_set_size(led, 8, 8);
    //lv_obj_set_hidden(led, true);
    lv_obj_set_style_radius(led, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(led, 0, LV_PART_MAIN);
    lv_led_set_color(led, LV_COLOR_RED);
    lv_obj_set_style_shadow_width(led, 0, LV_PART_MAIN);
    lv_obj_align(led, LV_ALIGN_CENTER, 0, 15);
    ledSecGrp[1] = led;
}

static void ContTime_Create(lv_obj_t* par)
{
    lv_obj_t* cont = lv_obj_create(par);
    lv_obj_add_style(cont, cont_style, LV_PART_MAIN);
    lv_obj_set_size(cont, 222, 93);
    lv_obj_align_to(cont, contDate, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_set_style_border_color(cont, LV_COLOR_RED, LV_PART_MAIN);
    lv_obj_set_style_border_width(cont, 2, LV_PART_MAIN);
    lv_obj_set_style_text_font(cont, &Font_RexBold_89, LV_PART_MAIN);
    lv_obj_set_style_text_color(cont, LV_COLOR_RED, LV_PART_MAIN);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);

    contTime = cont;

    LabelTime_Create(contTime);
}

static void LabelHeartRate_Update()
{
    //lv_label_set_text_fmt(labelHeartRate, "%04.01f", ParticleSensor_GetBeats());
}

static void ContHeartRate_Create(lv_obj_t* par)
{
    lv_obj_t* cont = lv_obj_create(par);
    lv_obj_add_style(cont, cont_style, LV_PART_MAIN);
    lv_obj_set_size(cont, 150, 32);
    lv_obj_align_to(cont, contTime, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_obj_set_style_border_width(cont, 2, LV_PART_MAIN);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    contHeartRate = cont;

    cont = lv_obj_create(contHeartRate);
    lv_obj_add_style(cont, cont_style, LV_PART_MAIN);
    lv_obj_set_size(cont, lv_obj_get_width(contHeartRate) / 3, lv_obj_get_height(contHeartRate));
    lv_obj_align(cont, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_bg_color(cont, LV_COLOR_WHITE, LV_PART_MAIN);
    lv_obj_set_style_text_color(cont, LV_COLOR_BLACK, LV_PART_MAIN);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t* label = lv_label_create(cont);
    lv_obj_set_style_text_font(label, &Font_RexBold_28, LV_PART_MAIN);
    lv_label_set_text(label, "TMP");
    lv_obj_align(label,LV_ALIGN_CENTER, 0, 0);

    label = lv_label_create(contHeartRate);
    lv_obj_set_style_text_font(label, &Font_RexBold_28, LV_PART_MAIN);
    lv_label_set_text(label, "00.0");
    lv_obj_align_to(label, cont, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    labelHeartRate = label;

    cont = lv_obj_create(contHeartRate);
    lv_obj_add_style(cont, cont_style, LV_PART_MAIN);
    lv_obj_set_size(cont, lv_obj_get_width(contHeartRate) / 4, lv_obj_get_height(contHeartRate));
    lv_obj_align(cont, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_set_style_bg_color(cont, LV_COLOR_WHITE, LV_PART_MAIN);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t* img = lv_img_create(cont);
    lv_img_set_src(img, &IMG_Heart);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, img);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_img_set_zoom);
    lv_anim_set_values(&a, LV_IMG_ZOOM_NONE, 160);
    lv_anim_set_time(&a, 300);
    lv_anim_set_playback_time(&a, 300);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_repeat_delay(&a, 800);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_out);

    lv_anim_start(&a);
}

static void LabelSteps_Update()
{
    //lv_label_set_text_fmt(labelSteps, "%05d", IMU_GetSteps());
}

/**
 * 创建一个包含湿度和步数显示的容器
 * @param par 父对象，用于将新创建的容器添加为子对象
 */
static void ContSteps_Create(lv_obj_t* par)
{
    /* 创建一个容器对象，并将其添加为父对象的子对象 */
    lv_obj_t* cont = lv_obj_create(par);
    
    /* 为容器应用样式，并设置其大小和位置 */
    lv_obj_add_style(cont, cont_style, LV_PART_MAIN);
    lv_obj_set_size(cont, 150, 32);
    lv_obj_align_to(cont, contHeartRate, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(cont, 2, LV_PART_MAIN);
    
    /* 全局变量存储此容器对象的引用 */
    contSteps = cont;

    /* 在contSteps容器内部创建另一个容器，用于显示湿度和步数 */
    cont = lv_obj_create(contSteps);
    
    /* 设置内部容器的样式、大小和位置 */
    lv_obj_add_style(cont, cont_style, LV_PART_MAIN);
    lv_obj_set_size(cont, lv_obj_get_width(contSteps) / 3, lv_obj_get_height(contSteps));
    lv_obj_align(cont, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_bg_color(cont, LV_COLOR_WHITE, LV_PART_MAIN);
    lv_obj_set_style_text_color(cont, LV_COLOR_BLACK, LV_PART_MAIN);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);

    /* 创建并配置显示湿度值的标签 */
    lv_obj_t* label = lv_label_create(cont);
    lv_obj_set_style_text_font(label, &Font_RexBold_28, LV_PART_MAIN);
    lv_label_set_text(label, "HUM");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    /* 创建并配置显示步数的标签 */
    label = lv_label_create(contSteps);
    lv_obj_set_style_text_font(label, &Font_RexBold_28, LV_PART_MAIN);
    lv_label_set_text(label, "00000");
    lv_obj_align_to(label, cont, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    
    /* 全局变量存储步数标签对象的引用 */
    labelSteps = label;
}
static void ImgCHN_Create(lv_obj_t* par)
{
    lv_obj_t* img = lv_img_create(par);
    lv_img_set_src(img, &IMG_CHNSecond);
    lv_obj_align_to(img, contTime, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 5);
    imgCHN = img;
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
 * @brief 动画回调函数，用于统一设置对象的不透明度
 * 
 * @param obj 回调函数的参数，此处未使用，可以为空
 * @param opa 设置的对象的不透明度值
 */
static void ObjsOpa_AnimCallback(void* obj, int16_t opa)
{
    /* 设置时间效果标签组中所有标签的不透明度 */
    lv_obj_set_style_opa(labelTimeEffect[0].label_1, opa, LV_PART_MAIN);
    lv_obj_set_style_opa(labelTimeEffect[1].label_1, opa, LV_PART_MAIN);
    lv_obj_set_style_opa(labelTimeEffect[2].label_1, opa, LV_PART_MAIN);
    lv_obj_set_style_opa(labelTimeEffect[3].label_1, opa, LV_PART_MAIN);

    /* 设置时间效果标签组中所有副标签的不透明度 */
    lv_obj_set_style_opa(labelTimeEffect[0].label_2, opa, LV_PART_MAIN);
    lv_obj_set_style_opa(labelTimeEffect[1].label_2, opa, LV_PART_MAIN);
    lv_obj_set_style_opa(labelTimeEffect[2].label_2, opa, LV_PART_MAIN);
    lv_obj_set_style_opa(labelTimeEffect[3].label_2, opa, LV_PART_MAIN);
    
    /* 设置日期、心率和步数标签的不透明度 */
    lv_obj_set_style_opa(labelDate, opa, LV_PART_MAIN);
    lv_obj_set_style_opa(labelHeartRate, opa, LV_PART_MAIN);
    lv_obj_set_style_opa(labelSteps, opa, LV_PART_MAIN);

    /* 设置中国国旗图像的不透明度 */
    lv_obj_set_style_opa(imgCHN, opa, LV_PART_MAIN);
}

/**
 * 创建页面动画，用于逐渐显示各个组件
 * 
 * 通过定义一系列动画信息，创建一个动画时间线，逐步改变组件的宽度或不透明度，
 * 以实现页面元素的动画入场效果。动画信息包括组件、起始宽度/不透明度、结束宽度/不透明度、
 * 动画执行函数、动画路径函数和动画持续时间。
 */
static void PagePlayAnim_Create(void)
{
    /* 定义动画信息数组，包含每个组件的动画细节，如开始时间、目标对象、执行函数等 */
#define ANIM_WIDTH_DEF(start_time, obj) {start_time, obj, (lv_anim_exec_xcb_t)lv_obj_set_width, 0,  lv_obj_get_width(obj), 300, lv_anim_path_ease_out}
    lv_anim_timeline_info_t anim_timeline_info[] = {
        ANIM_WIDTH_DEF(0,   contBatt),
        ANIM_WIDTH_DEF(100, contDate),
        ANIM_WIDTH_DEF(200, contTime),
        ANIM_WIDTH_DEF(300, contHeartRate),
        ANIM_WIDTH_DEF(400, contSteps),

        {400, NULL, (lv_anim_exec_xcb_t)ContBatt_AnimCallback, 0, 100, 400, lv_anim_path_linear},
        {800, NULL, (lv_anim_exec_xcb_t)ObjsOpa_AnimCallback, LV_OPA_TRANSP, LV_OPA_COVER, 500, lv_anim_path_ease_out},
    };

    /* 创建一个动画时间线，用于管理一系列动画 */
    anim_timeline = lv_anim_timeline_create();

    /* 遍历动画信息数组，为每个组件创建并添加动画到时间线 */
    for (int i = 0; i < __Sizeof(anim_timeline_info); i++)
    {
        lv_anim_t a;
        lv_anim_init(&a);
        /* 设置动画变量，即组件对象 */
        lv_anim_set_var(&a, anim_timeline_info[i].obj);
        /* 设置动画的起始和结束值 */
        lv_anim_set_values(&a, anim_timeline_info[i].start, anim_timeline_info[i].end);
        /* 注释掉的代码表示原本有但当前不需要的设置 */
        //lv_anim_set_early_apply(&a, false);
        /* 设置动画的执行函数和路径函数 */
        lv_anim_set_exec_cb(&a, anim_timeline_info[i].exec_cb);
        lv_anim_set_path_cb(&a, anim_timeline_info[i].path_cb);
        /* 设置动画的持续时间 */
        lv_anim_set_time(&a, anim_timeline_info[i].duration);
        /* 将动画添加到时间线，在指定的时间开始执行 */
        lv_anim_timeline_add(anim_timeline, anim_timeline_info[i].start_time, &a);
    }
}

/**
 * 页面动画播放函数
 * 
 * @param playback 指定动画的播放方向。true表示反向播放，false表示正向播放。
 * 
 * 该函数用于启动页面的动画播放。它首先调用一个动画回调函数，然后设置动画时间线的播放方向，
 * 并开始播放动画。最后，根据动画的播放时长，延迟执行后续操作。
 */
static void PagePlayAnim(bool playback)
{
    /* 调用动画回调函数，此处传递的参数为NULL和0 */
    ObjsOpa_AnimCallback(NULL, 0);
    
    /* 设置动画时间线的播放方向 */
    lv_anim_timeline_set_reverse(anim_timeline, playback);
    
    /* 开始播放动画时间线 */
    lv_anim_timeline_start(anim_timeline);
    
    /* 根据动画时间线的播放时长，延迟执行后续操作 */
    PageDelay(lv_anim_timeline_get_playtime(anim_timeline));
}



/****************************************************************************************/


/**
 * 初始化一个时间效果标签
 * @param effect 一个lv_label_time_effect_t类型的结构体，用于存储效果相关数据
 * @param cont 时间标签的容器对象
 * @param label_copy 要显示的时间文本的原始标签对象
 * @param anim_time 动画持续时间，单位为毫秒
 * @param x_offset 标签在容器中的水平偏移量
 */
void lv_label_time_effect_init(
    lv_label_time_effect_t* effect,
    lv_obj_t* cont,
    lv_obj_t* label_copy,
    uint16_t anim_time,
    lv_coord_t x_offset
)
{
    /* 创建一个新的标签对象用于显示时间 */
    lv_obj_t* label = lv_label_create(cont);
    /* 将原始标签的文本复制到新创建的标签上 */
    lv_label_set_text(label, lv_label_get_text(label_copy));

    /* 设置新标签的文本颜色为红色 */
    lv_obj_set_style_text_color(label, LV_COLOR_RED, LV_PART_MAIN);
    /* 设置新标签的字体为特定的字体 */
    lv_obj_set_style_text_font(label, &Font_RexBold_89, LV_PART_MAIN);

    /* 计算新标签的垂直偏移量，使其在容器中居中显示 */
    effect->y_offset = (lv_obj_get_height(cont) + 90) / 2 + 1;
    /* 将新创建的标签与容器中心对齐，并应用水平偏移量 */
    lv_obj_align(label, LV_ALIGN_CENTER, x_offset, -effect->y_offset);
    /* 保存原始标签和新创建的标签的引用 */
    effect->label_1 = label_copy;
    effect->label_2 = label;

    /* 初始化用于动画效果的变量 */
    effect->value_last = 0;

    /* 初始化一个动画对象，用于控制标签的垂直位置 */
    lv_anim_t a;
    lv_anim_init(&a);
    /* 设置动画的执行回调函数，用于更新标签的位置 */
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
    /* 设置动画的持续时间 */
    lv_anim_set_time(&a, anim_time);
    /* 设置动画的延迟时间 */
    lv_anim_set_delay(&a, 0);
    /* 设置动画的速度曲线为缓出 */
    lv_anim_set_path_cb(&a, lv_anim_path_ease_out);

    /* 保存动画对象的引用，用于后续的效果更新 */
    effect->anim_now = a;
    effect->anim_next = a;
}


static void lv_label_time_effect_check_value(lv_label_time_effect_t * effect, uint8_t value)
{
    if(value == effect->value_last)
        return;
    lv_obj_t * next_label;
    lv_obj_t * now_label;

    if(lv_obj_get_y(effect->label_2) > lv_obj_get_y(effect->label_1))
    {
        now_label = effect->label_1;
        next_label = effect->label_2;
        //LV_LOG_USER("effect->label_2 %d < effect->label_1 %d ", lv_obj_get_y(effect->label_2) ,lv_obj_get_y(effect->label_1));
    }
    else 
    {
        now_label = effect->label_2;
        next_label = effect->label_1;
        //LV_LOG_USER("effect->label_2 %d > effect->label_1 %d", lv_obj_get_y(effect->label_2), lv_obj_get_y(effect->label_1));
    }

    lv_label_set_text_fmt(now_label, "%d", effect->value_last);
    lv_label_set_text_fmt(next_label, "%d", value);
    effect->value_last = value;

    //lv_obj_align(next_label, LV_ALIGN_CENTER, 0, -effect->y_offset);

    lv_obj_set_y(next_label, -effect->y_offset);
    //lv_obj_align_to(next_label, now_label, LV_ALIGN_OUT_TOP_MID, 0, -effect->y_offset);

    lv_coord_t y_offset = abs(lv_obj_get_y(now_label) - lv_obj_get_y(next_label));

    lv_anim_t* a;
    a = &(effect->anim_now);
    lv_anim_set_var(a, now_label);
    lv_anim_set_values(a, 0, effect->y_offset);
    lv_anim_start(a);

    a = &(effect->anim_next);
    lv_anim_set_var(a, next_label);
    lv_anim_set_values(a, -effect->y_offset, 0);
    lv_anim_start(a);

    //LV_LOG_USER("y_offset [%d], now_label y [%d],next_label y [%d]",
//        y_offset, lv_obj_get_y(now_label),
//        lv_obj_get_y(next_label));
}
static void dial_update(void *parameter)
{
	struct tm *Time;
	time_t now;
	float humidity, temperature;
	while(1)
	{
		
		// humidity = aht10_read_humidity(aht10_dev);
        humidity = 0;
		lv_label_set_text_fmt(labelSteps, "%d.%d %%", (int)humidity, (int)(humidity * 10) % 10);
		// temperature = aht10_read_temperature(aht10_dev);
        temperature = 0;
		lv_label_set_text_fmt(labelHeartRate, "%d.%d", (int)temperature, (int)(temperature * 10) % 10);

		rt_mutex_take(dynamic_mutex, RT_WAITING_FOREVER);
		// now = time(RT_NULL) + 28800;
		// Time = gmtime(&now);
		// Clock.year = Time->tm_year+1900;
		// Clock.month = Time->tm_mon+1;
		// Clock.date = Time->tm_mday;
		// Clock.week = Time->tm_wday;
		// Clock.hour = Time->tm_hour;
		// Clock.min = Time->tm_min;
		// Clock.sec = Time->tm_sec;
        Clock.year = 0;
        Clock.month = 0;
        Clock.date = 0;
        Clock.week = 0;
        Clock.hour = 0;
        Clock.min = 0;
        Clock.sec = 0;
		LabelDate_Update(Clock.year, Clock.month, Clock.date, Clock.week);
		LabelTime_Update(Clock.hour,Clock.min);
		rt_mutex_release(dynamic_mutex);
		rt_thread_mdelay(1000);
	}
}

static void date_thread_crearte(void)
{
	dynamic_mutex = rt_mutex_create("dmutex", RT_IPC_FLAG_PRIO);
	dial_thread = rt_thread_create("daterun", dial_update, RT_NULL, 2048, 25, 10);
    if (dial_thread != RT_NULL)
    {
        rt_thread_startup(dial_thread);
    }
    else
    {
        rt_kprintf("create weather_update failed!\n");
    }
}

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
    /* 创建电池电量显示容器 */
    ContBatt_Create(appWindow);
    /* 创建日期显示容器 */
    ContDate_Create(appWindow);
    /* 创建时间显示容器 */
    ContTime_Create(appWindow);
    /* 创建心率显示容器 */
    ContHeartRate_Create(appWindow);
    /* 创建步数显示容器 */
    ContSteps_Create(appWindow);
    /* 创建中国国旗图标控件 */
    ImgCHN_Create(appWindow);
    /* 初始化动画页面 */
    PagePlayAnim_Create();
    /* 播放欢迎动画 */
    PagePlayAnim(false);
    /* 延迟100毫秒，以便界面元素有足够时间渲染 */
    PageDelay(100);
    /* 查找并打开RTC设备，用于获取准确的时间信息 */
    rt_device_t device = rt_device_find("rtc");
    /* 如果找不到RTC设备，则输出错误信息 */
    if (!device)
    {
        rt_kprintf("find %s failed!\n", "rtc");
    }
    /* 如果打开RTC设备失败，则输出错误信息 */
    else
    {
        if(rt_device_open(device, 0) != RT_EOK)
            rt_kprintf("open %s failed!\n", "rtc");
    }
    /* 创建日期显示线程 */
    date_thread_crearte();
}

static void Exit()
{
	rt_thread_delete(dial_thread);
	rt_mutex_delete (dynamic_mutex);
	// aht10_deinit(aht10_dev);
    PagePlayAnim(true);
    lv_anim_timeline_del(anim_timeline);
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
			// PM_Push(Page, PAGE_MainMenu);
            PM_Push(Page, PAGE_Main);
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
