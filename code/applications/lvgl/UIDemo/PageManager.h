/*
 * @Author: murnure 2662761173@qq.com
 * @Date: 2024-07-29 20:11:21
 * @LastEditors: murnure 2662761173@qq.com
 * @LastEditTime: 2024-07-30 15:40:10
 * @FilePath: \06_demo_factory\applications\lvgl\UIDemo\PageManager.h
 * @Description: 
 * 
 * Copyright (c) 2024 by murmure, All Rights Reserved. 
 */
/*
 * MIT License
 * Copyright (c) 2018-2020 _VIFEXTech
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the follo18wing conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
 
/**
  ******************************************************************************
  * @file    PageManager.h
  * @author  _VIFEXTech
  * @version v1.4
  * @date    2020-7-28
  * @brief   基于状态机的异步页面调度器，不依赖任何GUI框架，支持操作系统
  * @finished: 2018.10.6  - v1.0 拥有简单的页面切换管理，事件传递调度
  * @upgrade:  2019.4.1   - v1.1 增加注释，重写事件传递调度，减少重复代码
  * @upgrade:  2019.10.5  - v1.2 使用栈结构管理页面嵌套，默认栈深度10层(可调)
  * @upgrade:  2019.12.4  - v1.3 修复在Setup和Exit阻塞情况下页面切换的BUG，添加析构
  * @upgrade:  2020.7.28  - v1.4 修改EventFunction_t形参顺序，判断指针使用 "ptr != NULL" 的形式，添加MIT开源协议
  * @upgrade:  2020.11.3  - v1.5 将事件回调改成消息机制，统一使用一个回调函数处理；
  *                              修改函数命名，去除Page前缀；
  *                              为每个页面添加Name，可使用GetCurrentName()访问当前页面的名字
  * @upgrade:  2020.11.6  - v1.6 优化Running函数，去除其中的IS_PAGE判断
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PAGEMANAGER_H
#define __PAGEMANAGER_H

#include <stdint.h>
/**
 * @brief 消息类型枚举定义
 * 
 * 该枚举定义了系统中处理消息的类型，用于标识不同的消息事件。
 * 每个消息类型代表一个特定的系统操作或状态变化。
 */
typedef enum {
    PM_MSG_NONE,       /**< 无消息 */
    PM_MSG_Setup,      /**< 系统设置消息 */
    PM_MSG_Exit,       /**< 系统退出消息 */
    PM_MSG_Loop,       /**< 系统循环消息 */
    PM_MSG_MAX         /**< 消息类型最大值，用于界定枚举范围 */
} PM_MSG_t;

typedef void(*CallbackFunction_t)(void*, uint8_t);

typedef struct PageList {
    CallbackFunction_t Callback;
    const char* Name;
} PageList_t;

/**
 * 页面管理器结构体定义
 * 用于管理页面的切换和记录
 */
typedef struct PageManager {
    uint8_t NowPage;         /**< 当前页面编号 */
    uint8_t LastPage;        /**< 上一个页面编号 */
    uint8_t NextPage;        /**< 下一个页面编号 */
    uint8_t NewPage;         /**< 新的页面编号 */
    uint8_t OldPage;         /**< 旧的页面编号 */
    PageList_t* PageList;    /**< 页面列表指针，用于存储所有页面信息 */
    uint8_t* PageStack;      /**< 页面栈指针，用于记录页面的切换历史 */
    uint8_t PageStackSize;   /**< 页面栈大小，定义了可以记录的页面切换历史的最大长度 */
    uint8_t PageStackTop;    /**< 页面栈顶指针，指示当前页面栈的顶部位置 */
    uint8_t MaxPage;         /**< 最大页面数，定义了系统支持的最大页面数量 */
    uint8_t IsPageBusy;      /**< 页面状态标志，用于表示当前页面是否正忙 */
} PageManager_t;

/**
 * 初始化页面管理器。
 * @param pm 页面管理器的指针。
 * @param pageMax 最大页面数量。
 * @param pageStackSize 页面栈的最大深度。
 */
void PM_PageManagerInit(PageManager_t* pm, uint8_t pageMax, uint8_t pageStackSize);

/**
 * 反初始化页面管理器。
 * @param pm 页面管理器的指针。
 */
void PM_PageManagerDeinit(PageManager_t* pm);

/**
 * 注册一个新的页面。
 * @param pm 页面管理器的指针。
 * @param pageID 新页面的ID。
 * @param callback 页面回调函数。
 * @param name 页面的名称。
 * @return 注册结果，0表示成功，非0表示失败。
 */
uint8_t PM_Register(PageManager_t* pm, uint8_t pageID, CallbackFunction_t callback, const char* name);

/**
 * 清除指定的页面。
 * @param pm 页面管理器的指针。
 * @param pageID 要清除的页面ID。
 * @return 清除结果，0表示成功，非0表示失败。
 */
uint8_t PM_Clear(PageManager_t* pm, uint8_t pageID);

/**
 * 将页面推入栈中。
 * @param pm 页面管理器的指针。
 * @param pageID 要推入栈中的页面ID。
 * @return 推入结果，0表示成功，非0表示失败。
 */
uint8_t PM_Push(PageManager_t* pm, uint8_t pageID);

/**
 * 从栈中弹出一个页面。
 * @param pm 页面管理器的指针。
 * @return 弹出结果，0表示成功，非0表示失败。
 */
uint8_t PM_Pop(PageManager_t* pm);

/**
 * 切换到指定的页面。
 * @param pm 页面管理器的指针。
 * @param pageID 要切换到的页面ID。
 */
void PM_ChangeTo(PageManager_t* pm, uint8_t pageID);

/**
 * 发送事件到当前页面。
 * @param pm 页面管理器的指针。
 * @param obj 事件对象。
 * @param event 事件类型。
 */
void PM_EventTransmit(PageManager_t* pm, void* obj, uint8_t event);

/**
 * 清空页面栈。
 * @param pm 页面管理器的指针。
 */
void PM_StackClear(PageManager_t* pm);

/**
 * 获取当前页面的名称。
 * @param pm 页面管理器的指针。
 * @return 当前页面的名称。
 */
const char* PM_GetCurrentName(PageManager_t* pm);

/**
 * 更新页面状态。
 * @param pm 页面管理器的指针。
 */
void PM_Running(PageManager_t* pm);

#endif
