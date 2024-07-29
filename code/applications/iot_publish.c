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
// #include <dfs_posix.h>//��Ҫ��������������

// // AHT���ص���������
// #define AHT10_I2C_BUS "i2c3"
// #define GPIO_LED_B    GET_PIN(F, 11)
// #define GPIO_LED_R    GET_PIN(F, 12)
// char DEMO_PRODUCT_KEY[IOTX_PRODUCT_KEY_LEN + 1] = {0};
// char DEMO_DEVICE_NAME[IOTX_DEVICE_NAME_LEN + 1] = {0};
// char DEMO_DEVICE_SECRET[IOTX_DEVICE_SECRET_LEN + 1] = {0};
// int fd;//�ļ�������

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

// static void example_message_arrive(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)// ����Ϣ����ʱ�Ļص�����
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
// static int example_subscribe(void *handle)// ��������ĺ���
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
// /*@brief ������Ϣ��������ƽ̨��
// �ú���ͨ����ʽ��һ�������ַ�������ʹ�ø��������ݷ���һ����Ϣ��������ƽ̨��
// �����ַ����ĸ�ʽΪ "/sys/{product_key}/{device_name}/thing/event/property/post"��
// ����{product_key}��{device_name}�ֱ��滻ΪԤ������豸��Ʒ�����豸���ơ�
// ��Ϣ����Ϊһ��JSON�ַ����������豸������ֵ��
// @param handle ������ƽ̨�����Ӿ�����˴�δʹ�á�
// @return int ����0��ʾ�ɹ������ظ�ֵ��ʾʧ��*/

// static int example_publish(void *handle, int get_temp, int get_hum)// ������Ϣ������ĺ���
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
//         //д���ļ�
//         write(fd, send_inf, send_len);
//         write(fd, "\r\n", 2);
//         rt_kprintf("Write done.\n");

//         //�ر��ļ�
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
//     //�ַ����Ƚ�
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
// static void example_event_handle(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)// �¼�������
// {
//     EXAMPLE_TRACE("msg->event_type : %d", msg->event_type);
// }

// static int mqtt_main_thread(void *parameter)
// {
//     void                   *pclient = NULL;
//     int                     res = 0;
//     int                     loop_cnt = 0;
//     iotx_mqtt_param_t       mqtt_params;
//     float                   Humi, Temp;// Humi:ʪ��ֵ,Temp:�¶�ֵ
//     aht10_device_t          Dev = RT_NULL;    // AHT�豸ָ��
//     HAL_GetProductKey(DEMO_PRODUCT_KEY);
//     HAL_GetDeviceName(DEMO_DEVICE_NAME);
//     HAL_GetDeviceSecret(DEMO_DEVICE_SECRET);
//     // rt_thread_mdelay(2000);
//     // rt_enter_critical();
//     // rt_wlan_connect("123", "12345678");
//      // ��ʼ���豸
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
//             // ��ȡ��ʪ��ֵ
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
    

//     //��ֻд��ʽ���ļ�,���û�и��ļ�,�򴴽�һ���ļ�
    
//     if (fd >= 0)
//     {
//         //д���ļ�
//         // write(fd, String, sizeof(String));

//         rt_kprintf("open success.\n");

//         //�ر��ļ�
//         // close(fd);
//     }
// }
// void file_read(void)
// {
//     //��������ļ����ݵĻ�����
//     char buffer[100] = {};
//     fd = open("/fal/FileTest.txt", O_RDONLY);
//     rt_uint32_t size = read(fd, buffer, sizeof(buffer));
//     if (size < 0)
//         {
//             rt_kprintf("Read File Fail.\n");
//             return ;
//         }

//     //����ļ�����
//     rt_kprintf("Read from file test.txt : %s \n", buffer);
//     close(fd);
// }
// MSH_CMD_EXPORT(file_read, file_read);
// // AHT�߳�ָ��
// rt_thread_t MQTT_THREAD = RT_NULL;
// // ����AHT�߳�ʱ����
// #define THREAD_PRIORITY 5
// #define THREAD_STACK_SIZE 4096
// #define THREAD_TIMESLICE 5

// void MQTT_Creat_Thread(void)
// {
//     // �����߳�
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
//     // �����ɹ�������
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