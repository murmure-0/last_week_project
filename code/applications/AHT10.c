/*
 * @Author: murnure 2662761173@qq.com
 * @Date: 2024-07-26 21:12:05
 * @LastEditors: murnure 2662761173@qq.com
 * @LastEditTime: 2024-07-27 17:56:44
 * @FilePath: \project\applications\AHT10.C
 * @Description: 
 * 
 * Copyright (c) 2024 by murmure, All Rights Reserved. 
 */
#include <board.h>
#include <rtthread.h>
#include <drv_gpio.h>
#include "aht10.h"

// AHT���ص���������
#define AHT10_I2C_BUS "i2c3"

// ����AHT�߳�ʱ����
#define THREAD_PRIORITY 25
#define THREAD_STACK_SIZE 2048
#define THREAD_TIMESLICE 5

// AHT�߳�ָ��
rt_thread_t AHT10 = RT_NULL;

// AHT��������
void AHT10_Test(void *parameter)
{
    // AHT�豸ָ��
    aht10_device_t Dev = RT_NULL;

    // Humi:ʪ��ֵ,Temp:�¶�ֵ
    float Humi, Temp;
    if (Dev == RT_NULL)
    {
        rt_kprintf("AHT10_init Fail");
        return;
    }

    while (1)
    {
        // ��ȡ��ʪ��ֵ
        Humi = aht10_read_humidity(Dev);
        Temp = aht10_read_temperature(Dev);

        // û������rt_vsprintf_full�������ʱ��
        rt_kprintf("Humi: %d.%d\n", (int)Humi, (int)(Humi * 10) % 10);
        rt_kprintf("Temp: %d.%d\n", (int)Temp, (int)(Temp * 10) % 10);

        // ���rt_vsnprintf_full�����ʹ��
        //  rt_kprintf("Humi: %f, Temp: %f\n", Humi, Temp);

        rt_thread_mdelay(1000);
    }
}

void AHT10_Creat_Thread(void)
{
    // �����߳�
    AHT10 = rt_thread_create("AHT10", AHT10_Test, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    // �����ɹ�������
    if (AHT10 != RT_NULL)
    {
        rt_thread_startup(AHT10);
    }
    else
    {
        rt_kprintf("AHT10_Thread Create Fail");
    }
}
// ����Shell����
MSH_CMD_EXPORT(AHT10_Creat_Thread, This Function will creat a AHT10 thread.);
