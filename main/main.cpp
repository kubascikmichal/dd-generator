/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "AP/AP.h"
#include "NVS/NVS.h"
#include "HTTP_Server/HTTP_Server.h"
#include "ESP_NOW/ESP_NOW.h"
#include "driver/ledc.h"
#include "rom/ets_sys.h"
#include "Generator/Generator.h"

TaskHandle_t applicationTaskHandle;

static void clbck(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{
    printf("%s\n\r", data);
    printf("%02x:%02x:%02x:%02x:%02x:%02x\n\r", MAC2STR(mac_addr));
    xTaskNotify(applicationTaskHandle, 1, eSetValueWithOverwrite);
}

void applicationTask(void *param)
{
    Generator *generator = static_cast<Generator *>(param);
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS(100);
    while (1)
    {
        uint32_t ret = ulTaskNotifyTake(pdTRUE, xMaxBlockTime);
        if (ret > 0)
        {
            generator->generate();
        }
    }
}

extern "C"
{
    void app_main(void);
}

void app_main(void)
{
    NVS *nvs = new NVS();
    AP *ap = new AP("Generator", "123456789");
    HTTP_Server *server = new HTTP_Server(nvs);
    server->init();

    ESP_NOW *now = new ESP_NOW();
    uint8_t mac[6];
    cJSON *dev_cfg = nvs->getConfig();
    Generator *gen = new Generator(GPIO_NUM_3);
    uint32_t delay = atoi(cJSON_GetObjectItem(dev_cfg, "delay")->valuestring);
    uint32_t min = atoi(cJSON_GetObjectItem(dev_cfg, "minimum")->valuestring);
    uint32_t max = atoi(cJSON_GetObjectItem(dev_cfg, "maximum")->valuestring);
    uint32_t step = atoi(cJSON_GetObjectItem(dev_cfg, "step_size")->valuestring);
    uint32_t step_timeout = atoi(cJSON_GetObjectItem(dev_cfg, "step_timeout")->valuestring);
    uint32_t interval = atoi(cJSON_GetObjectItem(dev_cfg, "interval")->valuestring);
    gen->setup(delay, min, max, step, step_timeout, interval);
    xTaskCreatePinnedToCore(applicationTask, "Detection", 2048, gen, 1, &applicationTaskHandle, 0);
    if (dev_cfg)
    {
        printf("config => %s\n\r", cJSON_PrintUnformatted(dev_cfg));
        strToMAC(cJSON_GetObjectItem(dev_cfg, "MAC")->valuestring, 17, mac, 6);
        now->addPeer(mac);
        now->register_recieve_callback((esp_now_recv_cb_t)clbck);
    }

    int64_t start = esp_timer_get_time();
    int64_t stop = esp_timer_get_time();
    bool active = true;

    while (true)
    {
        vTaskDelay(10000/portTICK_PERIOD_MS);
        if (active)
        {
            if (((esp_timer_get_time() - start) / 1000) > 60000)
            {
                ap->stop();
                now->restart();
                stop = esp_timer_get_time();
                active = false;
            }
        }
        else
        {
            if ((esp_timer_get_time() - stop) / 1000 > 240000)
            {
                ap->start();
                start = esp_timer_get_time();
                active = true;
            }
        }
    }
}
