/*
 * @Author: murnure 2662761173@qq.com
 * @Date: 2024-07-29 16:00:39
 * @LastEditors: murnure 2662761173@qq.com
 * @LastEditTime: 2024-08-01 09:21:48
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
    // rt_pin_mode(GPIO_LED_R, PIN_MODE_OUTPUT);

    // while (1)
    // {
    //     rt_pin_write(GPIO_LED_R, PIN_HIGH);
    //     rt_thread_mdelay(500);
    //     rt_pin_write(GPIO_LED_R, PIN_LOW);
    //     rt_thread_mdelay(500);
    // }
}
