/*
 * @Author: murnure 2662761173@qq.com
 * @Date: 2024-07-28 12:22:25
 * @LastEditors: murnure 2662761173@qq.com
 * @LastEditTime: 2024-07-28 12:42:16
 * @FilePath: \project\applications\file.c
 * @Description: 
 * 
 * Copyright (c) 2024 by murmure, All Rights Reserved. 
 */
//�ǵ���menuconfig�п���֧�־ɰ汾���ܣ�Support legacy version��
#include <board.h>
#include <rtthread.h>
#include <drv_gpio.h>
#include <dfs_posix.h>//��Ҫ��������������

//����Ҫд�������
char String[] = "Hello, RT-Thread.Welcom to RSOC!";

//��������ļ����ݵĻ�����
char buffer[100] = {};

void FileSystem_Test(void *parameter)
{
    //�ļ�������
    int fd;

    //��ֻд��ʽ���ļ�,���û�и��ļ�,�򴴽�һ���ļ�
    fd = open("/fal/FileTest.txt", O_WRONLY | O_CREAT);

    //����򿪳ɹ�
    if (fd >= 0)
    {
        //д���ļ�
        write(fd, String, sizeof(String));

        rt_kprintf("Write done.\n");

        //�ر��ļ�
        close(fd);
    }
    else
    {
        rt_kprintf("File Open Fail.\n");
    }

    //��ֻ����ʽ���ļ�
    fd = open("/fal/FileTest.txt", O_RDONLY);

    if (fd>= 0)
    {
        //��ȡ�ļ�����
        rt_uint32_t size = read(fd, buffer, sizeof(buffer));
    
        if (size < 0)
        {
            rt_kprintf("Read File Fail.\n");
            return ;
        }

        //����ļ�����
        rt_kprintf("Read from file test.txt : %s \n", buffer);

        //�ر��ļ�
        close(fd);
    }
    else
    {
        rt_kprintf("File Open Fail.\n");
    }
}
//��������
MSH_CMD_EXPORT(FileSystem_Test, FileSystem_Test);

static void readdir_sample(void)
{
    DIR *dirp;
    struct dirent *d;

    /* �� / dir_test Ŀ¼ */
    dirp = opendir("/fal");
    if (dirp == RT_NULL)
    {
        rt_kprintf("open directory error!\n");
    }
    else
    {
        /* ��ȡĿ¼ */
        while ((d = readdir(dirp)) != RT_NULL)
        {
            rt_kprintf("found %s\n", d->d_name);
        }

        /* �ر�Ŀ¼ */
        closedir(dirp);
    }
}
/* ������ msh �����б��� */
MSH_CMD_EXPORT(readdir_sample, readdir sample);

/*
#define WIFI_CS GET_PIN(F, 10)
void WIFI_CS_PULL_DOWM(void)
{
    rt_pin_mode(WIFI_CS, PIN_MODE_OUTPUT);
    rt_pin_write(WIFI_CS, PIN_LOW);
}
INIT_BOARD_EXPORT(WIFI_CS GET_PIN);
*/