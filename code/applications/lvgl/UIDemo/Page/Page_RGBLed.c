/*
 * @Author: murnure 2662761173@qq.com
 * @Date: 2024-07-30 23:26:06
 * @LastEditors: murnure 2662761173@qq.com
 * @LastEditTime: 2024-08-02 18:03:33
 * @FilePath: \code\applications\lvgl\UIDemo\Page\Page_RGBLed.c
 * @Description: 
 * 
 * Copyright (c) 2024 by murmure, All Rights Reserved. 
 */
#include "../DisplayPrivate.h"
#include "ap3216c.h"
#include <drv_matrix_led.h>
PAGE_EXPORT(RGBLed);
void hardship_init(void);
void thread_init(void);
void mb_init(void);
ap3216c_device_t dev;



rt_mailbox_t mailbox; // 定义一个邮箱变量

static rt_thread_t RGB_Thread_Handle = RT_NULL;
static rt_thread_t Read_ap3216c_Thread_Handle = RT_NULL;

static void RGB_Con_thread_entry(void* parameter);
static void Read_ap3216c_thread_entry(void* parameter);
static void Setup()
{
    /* 将应用窗口移至前台，使其成为活动窗口 */
    lv_obj_move_foreground(appWindow);
    mb_init();
    hardship_init();
    thread_init();
    rt_kprintf("fly start");
    PageDelay(100);

}
//初始化邮箱函数
void mb_init(void)
{
    // 创建一个邮箱，参数分别为：邮箱名称、邮箱最大长度、邮箱消息大小
    mailbox = rt_mb_create("light_mailbox", 10, RT_IPC_FLAG_FIFO);
    if (mailbox == RT_NULL)
    {
        rt_kprintf("init mailbox failed.\n");
    }
}

void thread_init(void)
{
    // 设备读取亮度和距离线程
    Read_ap3216c_Thread_Handle = rt_thread_create("Read_ap3216c",
                                          Read_ap3216c_thread_entry,
                                          RT_NULL,
                                          1024,
                                          20,
                                          10);
    //启动线程
    rt_thread_startup(Read_ap3216c_Thread_Handle);
    RGB_Thread_Handle = rt_thread_create("RGB_Con",
                                          RGB_Con_thread_entry,
                                          RT_NULL,
                                          1024,
                                          20,
                                          10);
    rt_thread_startup(RGB_Thread_Handle);
}
static void RGB_Con_thread_entry(void* parameter)
{
    int Temp_light;
    RGBColor_TypeDef CUR_COLOR = {255, 255, 255};
    while (1)
    {
        //等待邮箱信息
        rt_mb_recv(mailbox, (rt_ubase_t*)&Temp_light, RT_WAITING_FOREVER);
        if ((int)(Temp_light/10) < 200)
        {
            CUR_COLOR.R = 200-(int)(Temp_light/10);
        }
        else
        {
            CUR_COLOR.R = 0;
        }
        if ((int)(Temp_light/10) < 200)
        {
            CUR_COLOR.G = 200 - (int)(Temp_light/10);
        }
        else
        {
            CUR_COLOR.G = 0;
        }
        if ((int)(Temp_light/10) < 400)
        {
            CUR_COLOR.B = 200 - (int)(Temp_light/10);
        }
        else
        {
            CUR_COLOR.B = 0;
        }
        

        for (int i = 0;i<19;i++)
        {
            Set_LEDColor(i, CUR_COLOR);
        }
        
        RGB_Reflash();
        rt_kprintf("loght: %d\n", Temp_light);
        // ambient_light = ap3216c_read_ambient_light(dev);
        // proximity = ap3216c_read_ps_data(dev);
        // rt_kprintf("ambient_light: %f\n", ambient_light);
        // rt_kprintf("proximity: %d\n", proximity);
        // PageDelay(1000);
    }
}

static void Read_ap3216c_thread_entry(void* parameter)
{
    int ambient_light;
    uint16_t proximity;
    while (1)
    {
        ambient_light = ap3216c_read_ambient_light(dev);
        // proximity = ap3216c_read_ps_data(dev);
        // rt_kprintf("ambient_light: %f\n", ambient_light);
        // rt_kprintf("proximity: %d\n", proximity);
        //发送光照数据到邮箱中
        rt_mb_send(mailbox, (rt_uint32_t)ambient_light);
        rt_thread_mdelay(1000);
    }
}
void hardship_init(void)
{
    // 初始化AP3216C设备
    dev = ap3216c_init("i2c2"); // 替换 "i2c_bus_name" 为实际的 I2C 总线名称
    if (dev == RT_NULL)
    {
        rt_kprintf("Failed to initialize AP3216C.\n");
    }
}

static void Exit()
{
    lv_obj_clean(appWindow);
    //关闭两个线程
    rt_thread_delete(RGB_Thread_Handle);
    rt_thread_delete(Read_ap3216c_Thread_Handle);
    for (int i = 0;i<19;i++)
    {
        Set_LEDColor(i, DARK);
    }
    RGB_Reflash();

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
