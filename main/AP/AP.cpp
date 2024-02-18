#include "AP.h"

AP::AP(char *SSID, char *PASSWORD, bool long_range, int p_channel,
       int p_ssid_hidden, int p_max_connection,
       int p_beacon_interval)
{
    this->m_long_range = long_range;
    this->nvs_init();
    this->basic_AP_init();
    esp_netif_init();
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();

    wifi_config_t ap_config = {
        .ap = {
            .channel = (uint8_t)p_channel,
            .authmode = WIFI_AUTH_WPA2_PSK,
            .ssid_hidden = 0,
            .max_connection = 10,
            .beacon_interval = 100}};

    sprintf(reinterpret_cast<char *>(ap_config.ap.ssid), SSID);
    sprintf(reinterpret_cast<char *>(ap_config.ap.password), PASSWORD);
    ESP_ERROR_CHECK(esp_wifi_set_protocol(WIFI_IF_AP, WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11N));
    if (this->m_long_range)
    {
        ESP_ERROR_CHECK(esp_wifi_set_protocol(WIFI_IF_AP, WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11N | WIFI_PROTOCOL_LR));
    }
    esp_wifi_set_config(WIFI_IF_AP, &ap_config);
    ESP_ERROR_CHECK(esp_wifi_start());
}

AP::AP(char *SSID, bool long_range, int p_channel,
       int p_ssid_hidden, int p_max_connection,
       int p_beacon_interval)
{
    this->nvs_init();
    this->basic_AP_init();
}

AP::~AP()
{
}

void AP::nvs_init()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

void AP::basic_AP_init()
{
    wifi_init_config_t wifiInitializationConfig = WIFI_INIT_CONFIG_DEFAULT();

    esp_wifi_init(&wifiInitializationConfig);

    esp_wifi_set_storage(WIFI_STORAGE_RAM);

    esp_wifi_set_mode(WIFI_MODE_AP);
}