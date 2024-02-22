#ifndef __HTTP_SERVER__
#define __HTTP_SERVER__
#include "string.h"
#include "string"
#include <esp_http_server.h>
#include <cJSON.h>
#include <stdio.h>
#include "../NVS/NVS.h"
#include "esp_mac.h"

#define _STREAM_BOUNDARY "\r\n--123456789000000000000987654321\r\n"

using namespace std;
class HTTP_Server
{
private:
public:
    HTTP_Server(NVS* nvs);
    ~HTTP_Server();

    static NVS *nvs;

    static esp_err_t root_get_handler(httpd_req_t *req);
    static const httpd_uri_t mainPageHandler;

    static esp_err_t get_favicon_ico(httpd_req_t *req);
    static const httpd_uri_t getFaviconIco;

    static esp_err_t get_style_css(httpd_req_t *req);
    static const httpd_uri_t getStyleCSS;

    static esp_err_t get_script_js(httpd_req_t *req);
    static const httpd_uri_t getScriptJS;

    static esp_err_t get_MAC(httpd_req_t *req);
    static const httpd_uri_t getMAC;

    static esp_err_t get_settings(httpd_req_t *req);
    static const httpd_uri_t getSettings;

    static esp_err_t set_settings(httpd_req_t *req);
    static const httpd_uri_t setSettings;
    
    bool init();

private:
};

#endif