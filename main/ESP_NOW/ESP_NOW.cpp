#include "ESP_NOW.h"
#include "esp_http_client.h"

ESP_NOW::ESP_NOW(/* args */)
{
    /*esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(ESPNOW_WIFI_MODE));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_set_protocol(ESPNOW_WIFI_IF, WIFI_PROTOCOL_LR));
    */
    ESP_ERROR_CHECK(esp_now_init());
}

ESP_NOW::~ESP_NOW()
{
    ESP_ERROR_CHECK(esp_now_deinit());
}

bool ESP_NOW::register_recieve_callback(esp_now_recv_cb_t callback)
{
    ESP_ERROR_CHECK(esp_now_register_recv_cb(callback));
    return true;
}

bool ESP_NOW::unregister_recieve_callback()
{
    ESP_ERROR_CHECK(esp_now_unregister_recv_cb());
    return true;
}

bool ESP_NOW::register_send_callback(esp_now_send_cb_t callback)
{
    ESP_ERROR_CHECK(esp_now_register_send_cb(callback));
    return true;
}

bool ESP_NOW::unregister_send_callback()
{
    ESP_ERROR_CHECK(esp_now_unregister_send_cb());
    return true;
}

int ESP_NOW::set_callbacks(esp_now_recv_cb_t recv_cb, esp_now_send_cb_t send_cb)
{
    ESP_ERROR_CHECK(esp_now_register_recv_cb(recv_cb));
    ESP_ERROR_CHECK(esp_now_register_send_cb(send_cb));
    return 0;
}

int ESP_NOW::clear_callbacks()
{
    ESP_ERROR_CHECK(esp_now_unregister_recv_cb());
    ESP_ERROR_CHECK(esp_now_unregister_send_cb());
    return 0;
}

int ESP_NOW::send_message(const uint8_t *addr, uint8_t *data, uint8_t length)
{
    esp_err_t ret = esp_now_send(addr, data, length);
    switch (ret)
    {
    case ESP_OK:
        return 0;
    case ESP_ERR_ESPNOW_NOT_INIT:
        return 1;
    case ESP_ERR_ESPNOW_ARG:
        return 2;
    case ESP_ERR_ESPNOW_INTERNAL:
        return 3;
    case ESP_ERR_ESPNOW_NO_MEM:
        return 4;
    case ESP_ERR_ESPNOW_NOT_FOUND:
        return 5;
    case ESP_ERR_ESPNOW_IF:
        return 6;
    default:
        return -1;
    }
    return 0;
}

bool ESP_NOW::addPeer(uint8_t *addr)
{
    /* Add peer information to peer list. */
    esp_now_peer_info_t *peer = (esp_now_peer_info_t *)malloc(sizeof(esp_now_peer_info_t));
    memset(peer, 0, sizeof(esp_now_peer_info_t));
    peer->channel = 1;
    peer->ifidx = ESPNOW_WIFI_IF;
    peer->encrypt = false;
    memcpy(peer->peer_addr, addr, ESP_NOW_ETH_ALEN);
    int ret = esp_now_add_peer(peer);
    free(peer);
    return ret == ESP_OK;
}

bool ESP_NOW::restart()
{
    ESP_ERROR_CHECK(esp_wifi_restore());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(ESPNOW_WIFI_MODE));
    ESP_ERROR_CHECK(esp_wifi_set_protocol(ESPNOW_WIFI_IF, WIFI_PROTOCOL_LR));
    ESP_ERROR_CHECK(esp_wifi_start());
    return true;
}