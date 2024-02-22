#include "NVS.h"

NVS::NVS(/* args */)
{
    this->xSemaphore = xSemaphoreCreateMutex();
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
    this->init();
}

NVS::~NVS()
{
}

bool NVS::saveConfig(cJSON *config)
{
    esp_err_t err = nvs_set_str(this->storage_handle, NVS_CONFIG_LABEL, cJSON_PrintUnformatted(config));
    err = nvs_commit(this->storage_handle);
    return (err == ESP_OK);
}

cJSON *NVS::getConfig()
{
    cJSON *ret = NULL;
    if (xSemaphoreTake(this->xSemaphore, 300))
    {
        size_t size = 0;
        esp_err_t err = nvs_get_str(this->storage_handle, NVS_CONFIG_LABEL, NULL, &size);
        printf("size %d\n\r", size);
        if (err != ESP_OK || size <= 0)
        {
            return NULL;
        }
        char *json = (char *)malloc(size * sizeof(char));
        err = nvs_get_str(this->storage_handle, NVS_CONFIG_LABEL, json, &size);
        ret = cJSON_Parse(json);
        printf("json parsed\n\r");
        if (sizeof(json))
        {
            free(json);
        }
        xSemaphoreGive((this->xSemaphore));
    }
    return ret;
}

uint64_t NVS::getTotalyOver()
{
    uint64_t total = 0;
    if (xSemaphoreTake(this->xSemaphore, 300))
    {
        esp_err_t err = nvs_get_u64(this->storage_handle, NVS_TOTAL_TRESHOLDS_LABEL, &total);
        xSemaphoreGive((this->xSemaphore));
    }
    return total;
}

bool NVS::saveTotalyOver(uint64_t p_total)
{
    if (xSemaphoreTake(this->xSemaphore, 300))
    {
        esp_err_t err = nvs_set_u64(this->storage_handle, NVS_TOTAL_TRESHOLDS_LABEL, p_total);
        xSemaphoreGive((this->xSemaphore));
    }
    return true;
}

void NVS::init()
{
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &this->storage_handle);
    if (err != ESP_OK)
    {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
}