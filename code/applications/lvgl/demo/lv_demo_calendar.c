#include <lvgl.h>
#include <board.h>
#include <drv_lcd.h>

/**
 * @brief 日历事件处理函数
 * @param e 日历事件指针
 */
static void event_handler(lv_event_t * e)
{
    /* 获取事件代码 */
    lv_event_code_t code = lv_event_get_code(e);
    /* 获取事件当前目标对象 */
    lv_obj_t * obj = lv_event_get_current_target(e);

    /* 当事件代码为值改变时 */
    if(code == LV_EVENT_VALUE_CHANGED) {
        /* 创建一个结构体用于存储选中的日期 */
        lv_calendar_date_t date;
        /* 获取按下（选中）的日期，如果成功则处理日期 */
        if(lv_calendar_get_pressed_date(obj, &date)) {
            /* 打印用户日志，显示点击的日期 */
            LV_LOG_USER("Clicked date: %02d.%02d.%d", date.day, date.month, date.year);
        }
    }
}

/**
 * @brief 初始化日历演示
 */
void lv_demo_calendar(void)
{
    /* 创建一个日历对象 */
    lv_obj_t  * calendar = lv_calendar_create(lv_scr_act());
    /* 设置日历大小为屏幕宽度和高度 */
    lv_obj_set_size(calendar, LCD_W, LCD_H);
    /* 将日历居中显示 */
    lv_obj_align(calendar, LV_ALIGN_CENTER, 0, 0);
    /* 添加事件处理函数，监听所有事件 */
    lv_obj_add_event_cb(calendar, event_handler, LV_EVENT_ALL, NULL);

    /* 设置今天日期为2021年2月23日 */
    lv_calendar_set_today_date(calendar, 2021, 02, 23);
    /* 设置显示的日期为2021年2月 */
    lv_calendar_set_showed_date(calendar, 2021, 02);

    /* 高亮显示几个特定日期 */
    /*Highlight a few days*/
    static lv_calendar_date_t highlighted_days[3];       /*Only its pointer will be saved so should be static*/
    highlighted_days[0].year = 2021;
    highlighted_days[0].month = 02;
    highlighted_days[0].day = 6;

    highlighted_days[1].year = 2021;
    highlighted_days[1].month = 02;
    highlighted_days[1].day = 11;

    highlighted_days[2].year = 2022;
    highlighted_days[2].month = 02;
    highlighted_days[2].day = 22;

    /* 设置高亮显示的日期数组和数量 */
    lv_calendar_set_highlighted_dates(calendar, highlighted_days, 3);

    /* 根据配置创建日历头部下拉框或箭头 */
#if LV_USE_CALENDAR_HEADER_DROPDOWN
    lv_calendar_header_dropdown_create(calendar);
#elif LV_USE_CALENDAR_HEADER_ARROW
    lv_calendar_header_arrow_create(calendar);
#endif
    /* 更新显示日期为2021年10月 */
    lv_calendar_set_showed_date(calendar, 2021, 10);
}