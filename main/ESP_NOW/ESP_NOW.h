#ifndef __ESPNOW_H__
#define __ESPNOW_H__
#include "esp_system.h"
#include <esp_now.h>
#include <esp_wifi.h>
#include <nvs_flash.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "../Resolvers/MACResolving.h"
#define CONFIG_ESPNOW_WIFI_MODE_STATION 0
#if CONFIG_ESPNOW_WIFI_MODE_STATION
#define ESPNOW_WIFI_MODE WIFI_MODE_STA
#define ESPNOW_WIFI_IF WIFI_IF_STA
#else
#define ESPNOW_WIFI_MODE WIFI_MODE_AP
#define ESPNOW_WIFI_IF WIFI_IF_AP
#endif
class ESP_NOW
{
private:
    cJSON *peerConfig;

public:
    ESP_NOW(/* args */);
    ~ESP_NOW();

    int set_callbacks(esp_now_recv_cb_t recv_cb, esp_now_send_cb_t send_cb);
    int clear_callbacks();
    int send_message(const uint8_t *addr, uint8_t *data, uint8_t length);

    bool register_recieve_callback(esp_now_recv_cb_t callback);
    bool unregister_recieve_callback();
    bool register_send_callback(esp_now_send_cb_t callback);
    bool unregister_send_callback();
    bool addPeer(uint8_t *addr);
    inline void setPeerConfig(cJSON *config) { this->peerConfig = config; };
    inline cJSON* getPeerConfig(){return this->peerConfig;};
    bool restart();
};

#endif