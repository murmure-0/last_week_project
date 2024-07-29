#include <ui_main.h>
#include <lvgl.h>
// #include "rtthread.h"
// static void button_click_cb(lv_event_t * e);
void ui_init(void)
{
    rt_kprintf("ui_init");
    // // 创建一个按钮
    lv_obj_t * log_out_btn = lv_btn_create(lv_scr_act());
    lv_obj_set_pos(log_out_btn, 10, 10); // 设置按钮的位置
    lv_obj_set_size(log_out_btn, 100, 50); // 设置按钮的尺寸
    // lv_obj_add_event_cb(btn, button_click_cb, LV_EVENT_CLICKED, NULL); // 添加点击事件处理程序

    // // 为按钮添加标签
    lv_obj_t * label = lv_label_create(log_out_btn);
    lv_label_set_text(label, "Click me!");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    // lv_obj_align(label, LV_ALIGN_CENTER); // 居中文本
}
// 按钮点击事件处理程序
// static void button_click_cb(lv_event_t * e)
// {
    // lv_event_code_t code = lv_event_get_code(e);
    // lv_obj_t * obj = lv_event_get_target(e);

    // if (code == LV_EVENT_CLICKED)
    // {
    //     // 当按钮被点击时
    //     lv_obj_t * label = lv_label_create(obj, NULL);
    //     lv_label_set_text(label, "Clicked!");
    //     lv_obj_align(label, obj, LV_ALIGN_IN_BOTTOM_MID, 0, 5); // 将文本居中显示在按钮下方
    // }
// }