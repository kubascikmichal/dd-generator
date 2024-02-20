#ifndef __NVS_H__
#define __NVS_H__

#include "cJSON.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_system.h"
#include "string.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#define NVS_CONFIG_LABEL "settings"
#define NVS_CONFIG_SIZE_LABEL "settings_size"
#define NVS_TOTAL_TRESHOLDS_LABEL "total_tresholds"

class NVS
{
private:
    nvs_handle_t storage_handle;
    SemaphoreHandle_t xSemaphore;
public:
    NVS();
    ~NVS();

    bool saveConfig(cJSON* config);
    cJSON *getConfig();

    uint64_t getTotalyOver();
    bool saveTotalyOver(uint64_t p_total);

private:
    void init();
};

#endif