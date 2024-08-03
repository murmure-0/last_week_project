/*
 * @Author: murnure 2662761173@qq.com
 * @Date: 2024-07-29 16:00:39
 * @LastEditors: murnure 2662761173@qq.com
 * @LastEditTime: 2024-08-02 20:41:11
 * @FilePath: \code\applications\main.c
 * @Description: 
 * 
 * Copyright (c) 2024 by murmure, All Rights Reserved. 
 */
/*
 * Copyright (c) 2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-07-06     Supperthomas first version
 * 2023-12-03     Meco Man     support nano version
 */

#include <board.h>
#include <rtthread.h>
#include <drv_gpio.h>
#ifndef RT_USING_NANO
#include <rtdevice.h>
#endif /* RT_USING_NANO */

// #define GPIO_LED_B    GET_PIN(F, 11)
// #define GPIO_LED_R    GET_PIN(F, 12)
int main(void)
{
    // 释放资源
    // ap3216c_deinit(dev);

    // return 0;
    // while (1)
    // {
    //     // 读取光照数据
    //     ambient_light = ap3216c_read_ambient_light(dev);
    //     rt_kprintf("Ambient Light: %.2f lux\n", ambient_light);

    //     // 读取接近度数据
    //     proximity = ap3216c_read_ps_data(dev);
    //     rt_kprintf("Proximity: %u\n", proximity);
    //     rt_thread_mdelay(1000);
    // }
}
