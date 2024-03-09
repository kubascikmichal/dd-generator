#ifndef __ACCESS_POINT_H__
#define __ACCESS_POINT_H__

#include <stdio.h>
#include "esp_log.h"

#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"

class AP
{
    typedef enum AP_TYPE
    {
        NO_PASSWORD,
        WITH_PASSWORD
    } Type;

private:
    int m_channel;
    int m_ssid_hidden;
    int m_max_connections;
    int m_beacon_interval;
    bool m_long_range;
    wifi_config_t ap;

public:
    AP(char *SSID, char *PASSWORD, bool long_range = false, int p_channel = 0,
       int p_ssid_hidden = 0, int p_max_connection = 4,
       int p_beacon_interval = 0);
    AP(char *SSID, bool long_range = false, int p_channel = 0,
       int p_ssid_hidden = 0, int p_max_connection = 4,
       int p_beacon_interval = 0);
    ~AP();
    bool start();
    bool stop();

private:
    void nvs_init();
    void basic_AP_init();
};

#endif