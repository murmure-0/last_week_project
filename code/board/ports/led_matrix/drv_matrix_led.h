/*
 * @Author: murnure 2662761173@qq.com
 * @Date: 2024-07-29 16:00:39
 * @LastEditors: murnure 2662761173@qq.com
 * @LastEditTime: 2024-08-02 17:32:13
 * @FilePath: \code\board\ports\led_matrix\drv_matrix_led.h
 * @Description: 
 * 
 * Copyright (c) 2024 by murmure, All Rights Reserved. 
 */
#ifndef _LED_MATRIX
#define _LED_MATRIX
#include <rtthread.h>

typedef struct RGBColor_TypeDef
{
    uint8_t G;
    uint8_t R;
    uint8_t B;
} RGBColor_TypeDef; // 颜色结构体

extern const RGBColor_TypeDef DARK;
extern const RGBColor_TypeDef GREEN;
extern const RGBColor_TypeDef RED;
extern const RGBColor_TypeDef BLUE;
extern const RGBColor_TypeDef WHITE;

/**
 * 设置LED颜色
 * 
 * @param LedId LED标识符，用于指定要设置的LED
 * @param Color RGB颜色类型，用于指定LED的新颜色
 * 
 * 说明：该函数用于根据指定的LED标识符和颜色来设置LED的颜色值
 */
extern void Set_LEDColor(uint16_t LedId, RGBColor_TypeDef Color);

/**
 * RGB刷新
 * 
 * 说明：该函数用于刷新RGB LED的显示，以确保颜色更改能够及时反映
 */
extern void RGB_Reflash(void);

/**
 * LED矩阵复位
 * 
 * 说明：该函数用于对LED矩阵进行复位操作，可能是初始化或清除LED矩阵的状态
 */
extern void led_matrix_rst();
#endif
