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
// #include "../DisplayPrivate.h"
// PAGE_EXPORT(RGBLed);

// static void Setup()
// {
//     /* 将应用窗口移至前台，使其成为活动窗口 */
//     lv_obj_move_foreground(appWindow);
    

//     rt_kprintf("fly start");
//     PageDelay(100);

// }



// static void Exit()
// {
//     lv_obj_clean(appWindow);
//     rt_kprintf("fly exit");
// }


// static void Event(lv_event_t* e)
// {
//     lv_event_code_t code = lv_event_get_code(e);
    
//     // 当检测到的事件类型为键盘事件时，执行以下操作
//     if(code == LV_EVENT_KEY)
//     {
//         // 从事件数据中获取按键值
//         uint8_t key = *(uint8_t*)lv_event_get_user_data(e);
//         // 打印获取的按键值，用于调试
//         if(key == PAGE_KEY_RIGHT)
//         {
//             // PM_Push(Page, PAGE_MainMenu);
//             // PM_Push(Page, PAGE_MainMenu);
//         }
        
//         if(key == PAGE_KEY_LEFT)
//         {

//             // 向左翻页时，推入硬件测试页面
//             PM_Pop(Page);
//         }
//         if(key == PAGE_KEY_UP)
//         {

//         }
//         if(key == PAGE_KEY_DOWN)
//         {
//         }


//     }

// }
