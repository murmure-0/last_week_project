// #include "rtthread.h"
// #include "dev_sign_api.h"
// #include "mqtt_api.h"
// #include <board.h>
// #include <rtthread.h>
// #include <drv_gpio.h>
// #include "aht10.h"
// #include "iot_publish.h"
// #include "wlan_mgnt.h"
// #include <board.h>
// #include <rtthread.h>
// #include <dfs_posix.h>//需要添加软件包进这里

// // AHT挂载的总线名字
// #define AHT10_I2C_BUS "i2c3"
// #define GPIO_LED_B    GET_PIN(F, 11)
// #define GPIO_LED_R    GET_PIN(F, 12)
// char DEMO_PRODUCT_KEY[IOTX_PRODUCT_KEY_LEN + 1] = {0};
// char DEMO_DEVICE_NAME[IOTX_DEVICE_NAME_LEN + 1] = {0};
// char DEMO_DEVICE_SECRET[IOTX_DEVICE_SECRET_LEN + 1] = {0};
// int fd;//文件描述符

// void *HAL_Malloc(uint32_t size);
// void HAL_Free(void *ptr);
// void HAL_Printf(const char *fmt, ...);
// int HAL_GetProductKey(char product_key[IOTX_PRODUCT_KEY_LEN + 1]);
// int HAL_GetDeviceName(char device_name[IOTX_DEVICE_NAME_LEN + 1]);
// int HAL_GetDeviceSecret(char device_secret[IOTX_DEVICE_SECRET_LEN]);
// uint64_t HAL_UptimeMs(void);
// int HAL_Snprintf(char *str, const int len, const char *fmt, ...);
// int infoHandler(char *str);

// #define EXAMPLE_TRACE(fmt, ...)  \
//     do { \
//         HAL_Printf("%s|%03d :: ", __func__, __LINE__); \
//         HAL_Printf(fmt, ##__VA_ARGS__); \
//         HAL_Printf("%s", "\r\n"); \
//     } while(0)

// static void example_message_arrive(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)// 当消息到达时的回调函数
// {
//     iotx_mqtt_topic_info_t     *topic_info = (iotx_mqtt_topic_info_pt) msg->msg;

//     switch (msg->event_type) {
//         case IOTX_MQTT_EVENT_PUBLISH_RECEIVED:
//             infoHandler(topic_info->payload);
//             /* print topic name and topic message */
//             // EXAMPLE_TRACE("Message Arrived:");
//             // EXAMPLE_TRACE("Topic  : %.*s", topic_info->topic_len, topic_info->ptopic);
//             // EXAMPLE_TRACE("Payload: %.*s", topic_info->payload_len, topic_info->payload);
//             // EXAMPLE_TRACE("\n");
//             break;
//         default:
//             break;
//     }
// }
// static int example_subscribe(void *handle)// 订阅主题的函数
// {
//     int res = 0;
//     const char *fmt = "/%s/%s/user/get";
//     char *topic = NULL;
//     int topic_len = 0;

//     topic_len = strlen(fmt) + strlen(DEMO_PRODUCT_KEY) + strlen(DEMO_DEVICE_NAME) + 1;
//     topic = HAL_Malloc(topic_len);
//     if (topic == NULL) {
//         EXAMPLE_TRACE("memory not enough");
//         return -1;
//     }
//     memset(topic, 0, topic_len);
//     HAL_Snprintf(topic, topic_len, fmt, DEMO_PRODUCT_KEY, DEMO_DEVICE_NAME);

//     res = IOT_MQTT_Subscribe(handle, topic, IOTX_MQTT_QOS0, example_message_arrive, NULL);
//     if (res < 0) {
//         EXAMPLE_TRACE("subscribe failed");
//         HAL_Free(topic);
//         return -1;
//     }

//     HAL_Free(topic);
//     return 0;

// }
// /*@brief 发布消息到物联网平台。
// 该函数通过格式化一个主题字符串，并使用给定的数据发布一条消息到物联网平台。
// 主题字符串的格式为 "/sys/{product_key}/{device_name}/thing/event/property/post"，
// 其中{product_key}和{device_name}分别被替换为预定义的设备产品键和设备名称。
// 消息负载为一个JSON字符串，包含设备的属性值。
// @param handle 物联网平台的连接句柄，此处未使用。
// @return int 返回0表示成功，返回负值表示失败*/

// static int example_publish(void *handle, int get_temp, int get_hum)// 发布消息到主题的函数
// {
//     int             res = 0;
//     const char     *fmt = "/sys/%s/%s/thing/event/property/post";
//     char           *topic = NULL;
//     char           *send_inf = NULL;
//     int             topic_len = 0;
//     int             send_len = 0;
//     char           *payload = "{\"params\":{\"Temp\":%d,\"Hum\":%d}}";
    
//     topic_len = strlen(fmt) + strlen(DEMO_PRODUCT_KEY) + strlen(DEMO_DEVICE_NAME) + 1;
//     send_len = strlen(payload)+4;
//     topic = HAL_Malloc(topic_len);
//     send_inf = HAL_Malloc(send_len);
    
//     if (topic == NULL) {
//         EXAMPLE_TRACE("topic memory not enough");
//         return -1;
//     }
//     if (send_inf == NULL) {
//         EXAMPLE_TRACE("send_inf memory not enough");
//         return -1;
//     }
//     memset(topic, 0, topic_len);
//     memset(send_inf, 0, send_len);
//     HAL_Snprintf(topic, topic_len, fmt, DEMO_PRODUCT_KEY, DEMO_DEVICE_NAME);
//     HAL_Snprintf(send_inf, send_len, payload, get_temp, get_hum);
//     res = IOT_MQTT_Publish_Simple(0, topic, IOTX_MQTT_QOS0, send_inf, strlen(send_inf));
//     fd = open("/fal/FileTest.txt", O_APPEND | O_CREAT | O_WRONLY);
//     if (fd >= 0)
//     {
//         //写入文件
//         write(fd, send_inf, send_len);
//         write(fd, "\r\n", 2);
//         rt_kprintf("Write done.\n");

//         //关闭文件
//         close(fd);
//     }
    
//     // close(fd);
//     if (res < 0) {
//         // EXAMPLE_TRACE("publish failed, res = %d", res);
//         HAL_Free(topic);
//         HAL_Free(send_inf);
//         return -1;
//     }
//     HAL_Free(send_inf);
//     HAL_Free(topic);
//     return 0;
// }
// int infoHandler(char * str)
// {
//     //字符串比较
//     if(strcmp(str, "ON") == 0)
//     {
//         rt_pin_write(GPIO_LED_R, PIN_LOW);
//         rt_pin_write(GPIO_LED_B, PIN_LOW);
//         return 1;
//     }
//     else if(strcmp(str, "OFF") == 0)
//     {
//         rt_pin_write(GPIO_LED_R, PIN_HIGH);
//         rt_pin_write(GPIO_LED_B, PIN_HIGH);
//         return 0;
//     }
// }
// static void example_event_handle(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)// 事件处理函数
// {
//     EXAMPLE_TRACE("msg->event_type : %d", msg->event_type);
// }

// static int mqtt_main_thread(void *parameter)
// {
//     void                   *pclient = NULL;
//     int                     res = 0;
//     int                     loop_cnt = 0;
//     iotx_mqtt_param_t       mqtt_params;
//     float                   Humi, Temp;// Humi:湿度值,Temp:温度值
//     aht10_device_t          Dev = RT_NULL;    // AHT设备指针
//     HAL_GetProductKey(DEMO_PRODUCT_KEY);
//     HAL_GetDeviceName(DEMO_DEVICE_NAME);
//     HAL_GetDeviceSecret(DEMO_DEVICE_SECRET);
//     // rt_thread_mdelay(2000);
//     // rt_enter_critical();
//     // rt_wlan_connect("123", "12345678");
//      // 初始化设备
//     Dev = aht10_init(AHT10_I2C_BUS);


//     memset(&mqtt_params, 0x0, sizeof(mqtt_params));


//     mqtt_params.handle_event.h_fp = example_event_handle;
    
//     pclient = IOT_MQTT_Construct(&mqtt_params);
//     if (NULL == pclient) {
//         EXAMPLE_TRACE("MQTT construct failed");
//         return -1;
//     }

//     res = example_subscribe(pclient);
//     if (res < 0) {
//         IOT_MQTT_Destroy(&pclient);
//         return -1;
//     }

//     while (1) {
//         if (0 == loop_cnt % 20) {
//             // 读取温湿度值
//             Humi = aht10_read_humidity(Dev);
//             Temp = aht10_read_temperature(Dev);
//             example_publish(pclient, (int)Temp, (int)Humi);
//         }

//         IOT_MQTT_Yield(pclient, 200);

//         loop_cnt += 1;
//     }

//     return 0;
// }

// void file_init(void)
// {
    

//     //用只写方式打开文件,如果没有该文件,则创建一个文件
    
//     if (fd >= 0)
//     {
//         //写入文件
//         // write(fd, String, sizeof(String));

//         rt_kprintf("open success.\n");

//         //关闭文件
//         // close(fd);
//     }
// }
// void file_read(void)
// {
//     //定义接受文件内容的缓冲区
//     char buffer[100] = {};
//     fd = open("/fal/FileTest.txt", O_RDONLY);
//     rt_uint32_t size = read(fd, buffer, sizeof(buffer));
//     if (size < 0)
//         {
//             rt_kprintf("Read File Fail.\n");
//             return ;
//         }

//     //输出文件内容
//     rt_kprintf("Read from file test.txt : %s \n", buffer);
//     close(fd);
// }
// MSH_CMD_EXPORT(file_read, file_read);
// // AHT线程指针
// rt_thread_t MQTT_THREAD = RT_NULL;
// // 创建AHT线程时待用
// #define THREAD_PRIORITY 5
// #define THREAD_STACK_SIZE 4096
// #define THREAD_TIMESLICE 5

// void MQTT_Creat_Thread(void)
// {
//     // 创建线程
//     // rt_thread_mdelay(12000);
//     // rt_enter_critical();
//     // file_init();
//     rt_wlan_connect("123", "12345678");
//     rt_thread_mdelay(2000);
//     rt_pin_mode(GPIO_LED_B, PIN_MODE_OUTPUT);
//     rt_pin_mode(GPIO_LED_R, PIN_MODE_OUTPUT);
//     rt_pin_write(GPIO_LED_R, PIN_HIGH);
//     rt_pin_write(GPIO_LED_B, PIN_HIGH);
//     // rt_exit_critical();
//     MQTT_THREAD = rt_thread_create("mqtt_push", mqtt_main_thread, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
//     // 创建成功就启动
//     if (MQTT_THREAD != RT_NULL)
//     {
//         rt_thread_startup(MQTT_THREAD);
//     }
//     else
//     {
//         rt_kprintf("MQTT_Thread Create Fail");
//     }
// }
// MSH_CMD_EXPORT(MQTT_Creat_Thread, aiot_thread);