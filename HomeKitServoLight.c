/* HomeKitServoLight
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <esp_wifi.h>
#include <esp_event_loop.h>
#include <esp_log.h>
#include <nvs_flash.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <rom/ets_sys.h>
#include <driver/gpio.h>

#include <homekit/homekit.h>
#include <homekit/characteristics.h>
#include "wifi.h"

#include "string.h"
#include "freertos/queue.h"
#include "esp_attr.h"
#include "soc/rtc.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"

#define SERVO_MIN_PULSEWIDTH 500  //最小脉冲间隔
#define SERVO_MAX_PULSEWIDTH 2500 //最大脉冲间隔
#define SERVO_MAX_DEGREE 180      // 180度舵机

static void mcpwm_example_gpio_initialize(void)
{
    printf("初始化mcpwm GPIO引脚\n");
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, 2); //设置GPIO引脚2为PWM0A, 舵机信号线连接于此
}
/**
 * @brief 该函数用于计算角度(单位度数)对应的脉冲宽度
 *
 * @param  目标角度
 *
 * @return
 *     - 指定角度对应的脉冲宽度
 */
static uint32_t servo_per_degree_init(uint32_t degree_of_rotation)
{
    uint32_t cal_pulsewidth = 0;
    cal_pulsewidth = (SERVO_MIN_PULSEWIDTH + (((SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH) * (degree_of_rotation)) / (SERVO_MAX_DEGREE)));
    return cal_pulsewidth;
}

/**
 * 开灯操作
 * 
 */
void turn_on(void *arg)
{
    uint32_t angle;
    //1. mcpwm gpio 初始化
    mcpwm_example_gpio_initialize();
    //2. 初始化 mcpwm 配置
    printf("Configuring Initial Parameters of mcpwm......\n");
    mcpwm_config_t pwm_config;
    pwm_config.frequency = 50; // 舵机信号频率为50Hz,每个周期时长20ms
    pwm_config.cmpr_a = 0;     //duty cycle of PWMxA = 0
    pwm_config.cmpr_b = 0;     //duty cycle of PWMxb = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config); //应用上面配置到 PWM0A & PWM0B
    // 旋转角度
    printf("Angle of rotation: %d\n", 0);
    // 计算0度对应的脉冲宽度
    angle = servo_per_degree_init(60);
    printf("pulse width: %dus\n", angle);
    mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, angle);
    vTaskDelay(10); //Add delay, since it takes time for servo to rotate, generally 100ms/60degree rotation at 5V
    vTaskDelete(NULL);
}

/**
 * 关灯操作
 * 
 */
void turn_off(void *arg)
{
    uint32_t angle;
    //1. mcpwm gpio 初始化
    mcpwm_example_gpio_initialize();
    //2. 初始化 mcpwm 配置
    printf("Configuring Initial Parameters of mcpwm......\n");
    mcpwm_config_t pwm_config;
    pwm_config.frequency = 50; //frequency = 50Hz, i.e. for every servo motor time period should be 20ms
    pwm_config.cmpr_a = 0;     //duty cycle of PWMxA = 0
    pwm_config.cmpr_b = 0;     //duty cycle of PWMxb = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config); //Configure PWM0A & PWM0B with above settings
    printf("Angle of rotation: %d\n", 0);
    // 计算90度对应的脉冲宽度
    angle = servo_per_degree_init(120);
    printf("pulse width: %dus\n", angle);
    mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, angle);
    vTaskDelay(10); //Add delay, since it takes time for servo to rotate, generally 100ms/60degree rotation at 5V
    vTaskDelete(NULL);
}

void on_wifi_ready();

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id)
    {
    case SYSTEM_EVENT_STA_START:
        printf("STA start\n");
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        printf("WiFI ready\n");
        on_wifi_ready();
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        printf("STA disconnected\n");
        esp_wifi_connect();
        break;
    default:
        break;
    }
    return ESP_OK;
}

static void wifi_init()
{
    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASSWORD,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}

const int led_gpio = 2;
bool led_on = false;

void led_write(bool on)
{
    if (on)
    {
        // 接收到开灯命令，执行开灯任务
        xTaskCreate(turn_on, "turn_on", 4096, NULL, 5, NULL);
    }
    else
    { // 接收到关灯命令，执行关灯任务
        xTaskCreate(turn_off, "turn_off", 4096, NULL, 5, NULL);
    }
}

void led_init()
{
    gpio_set_direction(led_gpio, GPIO_MODE_OUTPUT);
    led_write(led_on);
}

void led_identify_task(void *_args)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            led_write(true);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            led_write(false);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }

        vTaskDelay(250 / portTICK_PERIOD_MS);
    }

    led_write(led_on);

    vTaskDelete(NULL);
}

void led_identify(homekit_value_t _value)
{
    printf("LED identify\n");
    xTaskCreate(led_identify_task, "LED identify", 512, NULL, 2, NULL);
}

homekit_value_t led_on_get()
{
    return HOMEKIT_BOOL(led_on);
}

void led_on_set(homekit_value_t value)
{
    if (value.format != homekit_format_bool)
    {
        printf("Invalid value format: %d\n", value.format);
        return;
    }

    led_on = value.bool_value;
    led_write(led_on);
}

homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(.id = 1, .category = homekit_accessory_category_lightbulb, .services = (homekit_service_t *[]){HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics = (homekit_characteristic_t *[]){HOMEKIT_CHARACTERISTIC(NAME, "Home Light"), HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Goo"), HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "037A2BABF19D"), HOMEKIT_CHARACTERISTIC(MODEL, "Light"), HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "0.2"), HOMEKIT_CHARACTERISTIC(IDENTIFY, led_identify), NULL}), HOMEKIT_SERVICE(LIGHTBULB, .primary = true, .characteristics = (homekit_characteristic_t *[]){HOMEKIT_CHARACTERISTIC(NAME, "Home Light"), HOMEKIT_CHARACTERISTIC(ON, false, .getter = led_on_get, .setter = led_on_set), NULL}), NULL}),
    NULL};

homekit_server_config_t config = {
    .accessories = accessories,
    .password = "111-11-111"};

void on_wifi_ready()
{
    homekit_server_init(&config);
}

void app_main(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    wifi_init();
    led_init();
}
