
// #include "../DisplayPrivate.h"
// #include <drv_matrix_led.h>

// /*导出页面*/
// PAGE_EXPORT(RGBLed);
// static lv_obj_t* Cont;
// static lv_obj_t* Title;
// static lv_obj_t* ColorCont;
// static lv_obj_t* ColorLabel;
// static lv_style_t* cont_style;
// static lv_anim_timeline_t* anim_timeline = NULL;

// typedef struct {
//     lv_obj_t* slider;
//     lv_obj_t* label;
// }RGBSlider_t;

// typedef struct {
//     uint8_t r;
//     uint8_t g;
//     uint8_t b;
// }RGBColor_t;

// static RGBSlider_t rgbSliders[3];
// static RGBColor_t LedColor;
// static uint8_t isSelect;

// /**
//  * 滑块事件回调函数
//  * @param e 事件指针，包含滑块的相关信息
//  * 
//  * 该函数在滑块的值改变时被调用，更新与滑块关联的标签文本，
//  * 并根据滑块的值调整LED颜色和显示。
//  */
// static void slider_event_cb(lv_event_t* e)
// {
//     /* 获取触发事件的滑块对象 */
//     lv_obj_t* slider = lv_event_get_target(e);
//     /* 获取滑块事件中携带的用户数据，这里是一个标签对象 */
//     lv_obj_t* label = lv_event_get_user_data(e);
//     /* 获取滑块的当前值，并转换为整型 */
//     int val = (int)lv_slider_get_value(slider);
//     /* 更新标签文本，显示滑块的当前值 */
//     lv_label_set_text_fmt(label, "%d%%", val);

//     /* 根据不同的滑块，更新LED颜色的红、绿、蓝分量 */
//     if (slider == rgbSliders[0].slider)
//     {
//         LedColor.r = (uint8_t)lv_slider_get_value(slider) * 255 / 100;
//     }
//     else if (slider == rgbSliders[1].slider)
//     {
//         LedColor.g = (uint8_t)lv_slider_get_value(slider) * 255 / 100;
//     }
//     else if (slider == rgbSliders[2].slider)
//     {
//         LedColor.b = (uint8_t)lv_slider_get_value(slider) * 255 / 100;
//     }

//     /* 更新颜色标签的文本，显示当前LED颜色的RGB值 */
//     lv_label_set_text_fmt(ColorLabel, "%d,%d,%d", LedColor.r, LedColor.g, LedColor.b);

//     /* 根据当前LED颜色，设置颜色容器的背景色 */
//     lv_obj_set_style_bg_color(ColorCont, lv_color_make(LedColor.r, LedColor.g, LedColor.b), LV_PART_MAIN);

//     /* 使用当前LED颜色填充LED矩阵 */
//     led_matrix_fill_rgb(LedColor.r, LedColor.g, LedColor.b);
// }
// /**
//  * 创建滑动条控件
//  * 
//  * @param par 滑动条的父对象
//  */

// void Slider_Create(lv_obj_t* par)
// {
//     /* 创建一个容器对象 */
//     lv_obj_t* cont = lv_obj_create(par);
    
//     /* 添加样式到容器 */
//     lv_obj_add_style(cont, cont_style, LV_PART_MAIN);
    
//     /* 设置容器尺寸 */
//     lv_obj_set_size(cont, WINDOW_WIDTH, 150);
    
//     /* 对齐容器至顶部中间 */
//     lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 0);
    
//     /* 清除容器滚动标志 */
//     lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    
//     /* 设置容器内边距 */
//     lv_obj_set_style_pad_all(cont, 20, LV_PART_MAIN);
    
//     /* 遍历创建RGB滑动条 */
//     for (int i = 0; i < __Sizeof(rgbSliders); i++)
//     {
//         /* 在显示中心创建滑动条 */
//         lv_obj_t* slider = lv_slider_create(cont);
        
//         /* 设置滑动条尺寸 */
//         lv_obj_set_size(slider, 120, 20);
        
//         /* 设置滑动条聚焦状态下的边框宽度和颜色 */
//         lv_obj_set_style_border_width(slider, 2, LV_PART_MAIN | LV_STATE_FOCUSED);
//         lv_obj_set_style_border_color(slider, LV_COLOR_WHITE, LV_PART_MAIN | LV_STATE_FOCUSED);
//         lv_obj_set_style_border_width(slider, 2, LV_PART_INDICATOR | LV_STATE_FOCUSED);
//         lv_obj_set_style_border_color(slider, LV_COLOR_WHITE, LV_PART_INDICATOR | LV_STATE_FOCUSED);
        
//         /* 将滑动条指针存储在rgbSliders数组中 */
//         rgbSliders[i].slider = slider;
        
//         /* 创建滑动条下方的标签 */
//         lv_obj_t* label = lv_label_create(par);
//         lv_label_set_text(label, "0%");
        
//         /* 设置标签字体 */
//         lv_obj_set_style_text_font(label, &lv_font_montserrat_28, LV_PART_MAIN);
        
//         /* 将标签指针存储在rgbSliders数组中 */
//         rgbSliders[i].label = label;
        
//         /* 添加事件回调给滑动条 */
//         lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, label);
        
//         /* 将滑动条添加到应用窗口组 */
//         lv_group_add_obj(appWindow_g, slider);
//     }
    
//     /* 设置RGB滑动条的位置和背景颜色 */
//     lv_obj_align_to(rgbSliders[0].slider, cont, LV_ALIGN_TOP_LEFT, 0, 0);
//     lv_obj_set_style_bg_color(rgbSliders[0].slider, lv_color_make(0xff,0,0) , LV_PART_KNOB);
    
//     lv_obj_align_to(rgbSliders[1].slider, cont, LV_ALIGN_LEFT_MID, 0, 0);
//     lv_obj_set_style_bg_color(rgbSliders[1].slider, lv_color_make(0,0xff,0) , LV_PART_KNOB);
    
//     lv_obj_align_to(rgbSliders[2].slider, cont, LV_ALIGN_BOTTOM_LEFT, 0, 0);
//     lv_obj_set_style_bg_color(rgbSliders[2].slider, lv_color_make(0,0,0xff) , LV_PART_KNOB);
    
//     /* 设置标签相对于滑动条的位置 */
//     for (int i = 0; i < __Sizeof(rgbSliders); i++)
//     {
//         lv_obj_align_to(rgbSliders[i].label, rgbSliders[i].slider, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
//     }
    
//     /* 创建颜色容器 */
//     lv_obj_t* colorCont = lv_obj_create(par);
    
//     /* 设置颜色容器尺寸 */
//     lv_obj_set_size(colorCont, 70, 40);
    
//     /* 设置颜色容器位置 */
//     lv_obj_align_to(colorCont, cont, LV_ALIGN_OUT_BOTTOM_LEFT, 20, 0);
    
//     /* 清除颜色容器滚动标志 */
//     lv_obj_clear_flag(colorCont, LV_OBJ_FLAG_SCROLLABLE);
    
//     /* 将颜色容器指针赋值给全局变量 */
//     ColorCont = colorCont;
    
//     /* 创建颜色标签 */
//     lv_obj_t* colorlabel = lv_label_create(par);
    
//     /* 设置颜色标签文本 */
//     lv_label_set_text(colorlabel, "0,0,0");
    
//     /* 设置颜色标签字体 */
//     lv_obj_set_style_text_font(colorlabel, &lv_font_montserrat_28, LV_PART_MAIN);
    
//     /* 设置颜色标签位置 */
//     lv_obj_align_to(colorlabel, ColorCont, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    
//     /* 将颜色标签指针赋值给全局变量 */
//     ColorLabel = colorlabel;
// }

// /**
//  * @brief 初始化容器样式
//  * 
//  * 本函数用于设置一个静态的LVGL样式对象，该对象用于容器组件的外观样式。
//  * 样式包括背景颜色、边框宽度、圆角半径、边框颜色、文本颜色等属性。
//  * 设置完成后，将该样式对象的地址赋值给全局变量cont_style，以便在创建容器组件时应用该样式。
//  */
// static void ContStyle_Setup(void)
// {
//     /* 初始化一个静态的样式对象 */
//     static lv_style_t style;
//     lv_style_init(&style);

//     /* 设置背景颜色为黑色 */
//     lv_style_set_bg_color(&style, LV_COLOR_BLACK);
//     /* 设置边框宽度为2像素 */
//     lv_style_set_border_width(&style, 2);
//     /* 设置圆角半径为0，即矩形形状 */
//     lv_style_set_radius(&style, 0);
//     /* 设置边框颜色为白色 */
//     lv_style_set_border_color(&style, LV_COLOR_WHITE);
//     /* 设置文本颜色为白色 */
//     lv_style_set_text_color(&style, LV_COLOR_WHITE);
//     /* 移除容器内部的边框宽度，使得边框只出现在外部 */
//     lv_style_set_border_width(&style, 0);
//     /* 设置所有方向的内边距为0，使组件内容紧贴边框 */
//     lv_style_set_pad_all(&style, 0);

//     /* 将样式对象的地址赋值给全局变量cont_style */
//     cont_style = &style;
// }

// /**
//  * @brief 初始化设置页面
//  * 
//  * 本函数负责设置页面的初始化工作，包括将应用窗口移至前台、配置样式、创建容器和标题、
//  * 添加滑块以及创建和启动页面动画。
//  * 
//  * 页面初始化是一个关键步骤，它确保了用户界面的正确布局和动画效果的平滑过渡，提升了用户体验。
//  */
// static void Setup(void)
// {
//     /* 将应用窗口移至前台，确保用户界面的可见性和交互性 */
//     lv_obj_move_foreground(appWindow);
    
//     /* 设置容器的样式，包括颜色、边框等，以符合应用的视觉设计 */
//     ContStyle_Setup();
    
//     /* 创建主容器，并将其附加到应用窗口上，作为页面布局的基础 */
//     Cont = Cont_Create(appWindow);
    
//     /* 创建标题组件，并设置其文本为"Settings"，为页面提供明确的标识 */
//     Title = Title_Create(appWindow,"Settings");
    
//     /* 创建滑块组件，为用户提供交互式设置功能 */
//     Slider_Create(Cont);
    
//     /* 创建页面过渡动画，增强用户界面的动态效果 */
//     anim_timeline = PageAnim_Create(Cont, Title);
    
//     /* 启动页面动画，实现从上一个页面到当前页面的平滑过渡 */
//     PageAnim_Play(anim_timeline, false);
// }
// /**
//  * @brief 程序退出时的清理函数
//  * 
//  * 该函数在程序准备退出时调用，负责执行一系列清理工作，包括播放页面动画、删除动画时间线、清空LED矩阵和清理应用窗口对象。
//  * 这样做的目的是为了保证程序退出时界面效果平滑，同时清理掉所有分配的资源，防止内存泄漏。
//  */
// static void Exit(void)
// {
//     /* 播放页面动画，确保动画效果完整执行 */
//     PageAnim_Play(anim_timeline, true);
    
//     /* 从动画时间线中删除当前所有动画，为程序退出做准备 */
//     lv_anim_timeline_del(anim_timeline);
    
//     /* 清空LED矩阵，关闭所有LED显示 */
//     extern void led_matrix_clear();
//     led_matrix_clear();
    
//     /* 清理应用窗口对象，释放相关资源 */
//     lv_obj_clean(appWindow);
// }

// /**
//  * 处理页面事件
//  * @param e 事件指针，包含事件相关的信息
//  */
// static void Event(lv_event_t* e)
// {
//     /* 获取事件代码 */
//     lv_event_code_t code = lv_event_get_code(e);
    
//     /* 当事件为键盘事件时 */
//     if (code == LV_EVENT_KEY)
//     {
//         /* 获取触发事件的键值 */
//         uint8_t key = *(uint8_t*)lv_event_get_user_data(e);
        
//         /* 如果按下了左页键 */
//         if (key == PAGE_KEY_LEFT)
//         {
//             /* 如果当前处于选择状态，则取消选择 */
//             if(isSelect)
//             {
//                 isSelect = 0;
//                 /* 取消当前焦点对象的边框宽度 */
//                 lv_obj_t* obj = lv_group_get_focused(appWindow_g);
//                 lv_obj_set_style_border_width(obj, 0, LV_PART_KNOB | LV_STATE_FOCUSED);
//             }
//             /* 如果不处于选择状态，则返回上一页 */
//             else
//             {
//                 LV_LOG("Page->Pop()\n");
//                 PM_Pop(Page);
//             }
//         }

//         /* 如果按下了上页键 */
//         if (key == PAGE_KEY_UP)
//         {
//             /* 如果不处于选择状态，则取消编辑模式并焦点移至上一个对象 */
//             if(!isSelect)
//             {
//                 lv_group_set_editing(appWindow_g, false);
//                 lv_group_focus_prev(appWindow_g);
//             }
//             /* 如果处于选择状态，则模拟按下右键事件 */
//             else
//             {
//                 lv_group_send_data(appWindow_g, LV_KEY_RIGHT);
//             }
//         }

//         /* 如果按下了下页键 */
//         if (key == PAGE_KEY_DOWN)
//         {
//             /* 如果不处于选择状态，则取消编辑模式并焦点移至下一个对象 */
//             if(!isSelect)
//             {
//                 lv_group_set_editing(appWindow_g, false);
//                 lv_group_focus_next(appWindow_g);
//             }
//             /* 如果处于选择状态，则模拟按下左键事件 */
//             else
//             {
//                 lv_group_send_data(appWindow_g, LV_KEY_LEFT);
//             }
//         }

//         /* 如果按下了右页键 */
//         if (key == PAGE_KEY_RIGHT)
//         {
//             /* 设置为选择状态 */
//             isSelect = 1;
//             /* 设置当前焦点对象的边框宽度和颜色 */
//             lv_obj_t* obj = lv_group_get_focused(appWindow_g);
//             lv_obj_set_style_border_width(obj, 2, LV_PART_KNOB | LV_STATE_FOCUSED);
//             lv_obj_set_style_border_color(obj, LV_COLOR_WHITE, LV_PART_KNOB | LV_STATE_FOCUSED);
//         }

//     }
// }
