#include "HTTP_Server.h"

NVS *HTTP_Server::nvs;

HTTP_Server::HTTP_Server(NVS* nvs)
{
    this->nvs = nvs;
}

HTTP_Server::~HTTP_Server()
{
}

esp_err_t HTTP_Server::root_get_handler(httpd_req_t *req)
{
    extern const unsigned char root_start[] asm("_binary_index_html_start");
    extern const unsigned char root_end[] asm("_binary_index_html_end");
    httpd_resp_send(req, (const char *)root_start, root_end - root_start);
    return ESP_OK;
}

const httpd_uri_t HTTP_Server::mainPageHandler = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = HTTP_Server::root_get_handler,
    .user_ctx = NULL,
};

esp_err_t HTTP_Server::get_favicon_ico(httpd_req_t *req)
{
    extern const unsigned char favicon_ico_start[] asm("_binary_favicon_ico_start");
    extern const unsigned char favicon_ico_end[] asm("_binary_favicon_ico_end");
    const size_t favicon_ico_size = (favicon_ico_end - favicon_ico_start);
    httpd_resp_set_type(req, "image/x-icon");
    httpd_resp_send(req, (const char *)favicon_ico_start, favicon_ico_size);
    return ESP_OK;
}

const httpd_uri_t HTTP_Server::getFaviconIco = {
    .uri = "/favicon.ico",
    .method = HTTP_GET,
    .handler = HTTP_Server::get_favicon_ico,
    .user_ctx = NULL,
};

esp_err_t HTTP_Server::get_style_css(httpd_req_t *req)
{
    extern const unsigned char style_start[] asm("_binary_style_css_start");
    extern const unsigned char style_end[] asm("_binary_style_css_end");
    httpd_resp_set_type(req, "text/css");
    httpd_resp_send(req, (const char *)style_start, style_end - style_start);
    return ESP_OK;
}
const httpd_uri_t HTTP_Server::getStyleCSS = {
    .uri = "/style.css",
    .method = HTTP_GET,
    .handler = HTTP_Server::get_style_css,
    .user_ctx = NULL,
};
esp_err_t HTTP_Server::get_script_js(httpd_req_t *req)
{
    extern const unsigned char script_start[] asm("_binary_script_js_start");
    extern const unsigned char script_end[] asm("_binary_script_js_end");
    httpd_resp_send(req, (const char *)script_start, script_end - script_start);
    return ESP_OK;
}
const httpd_uri_t HTTP_Server::getScriptJS = {
    .uri = "/script.js",
    .method = HTTP_GET,
    .handler = HTTP_Server::get_script_js,
    .user_ctx = NULL,
};

esp_err_t HTTP_Server::get_MAC(httpd_req_t *req)
{
    printf("get MAC\n\r");
    cJSON *getMAC = cJSON_CreateObject();
    uint8_t MAC[6];
    esp_efuse_mac_get_default(MAC);
    char mac[20] = {0};
    sprintf(mac, MACSTR, MAC2STR(MAC));
    if (getMAC != NULL)
    {
        cJSON_AddStringToObject(getMAC, "MAC", mac);
        httpd_resp_send(req, cJSON_PrintUnformatted(getMAC), strlen(cJSON_PrintUnformatted(getMAC)));
    }
    else
    {
        httpd_resp_send(req, "{}", strlen("{}"));
    }
    return ESP_OK;
}

const httpd_uri_t HTTP_Server::getMAC = {
    .uri = "/getMAC",
    .method = HTTP_GET,
    .handler = HTTP_Server::get_MAC,
    .user_ctx = NULL};

esp_err_t HTTP_Server::get_settings(httpd_req_t *req)
{
    printf("get settings\n\r");
    
    cJSON *settings = nvs->getConfig();
    if (settings != NULL)
    {
        httpd_resp_send(req, cJSON_PrintUnformatted(settings), strlen(cJSON_PrintUnformatted(settings)));
    }
    else
    {
        httpd_resp_send(req, "{}", strlen("{}"));
    }
    return ESP_OK;
}

const httpd_uri_t HTTP_Server::getSettings = {
    .uri = "/getSettings",
    .method = HTTP_GET,
    .handler = HTTP_Server::get_settings,
    .user_ctx = NULL};

esp_err_t HTTP_Server::set_settings(httpd_req_t *req)
{
    char *content = (char *)malloc(req->content_len * sizeof(char));
    memset(content, 0, req->content_len);

    int ret = httpd_req_recv(req, content, req->content_len);
    printf("Content: %s\n\r", content);
    cJSON *setSettings = cJSON_CreateObject();
    if (ret <= 0)
    {
        cJSON_AddStringToObject(setSettings, "status", "400");
    }
    else
    {
        cJSON *settings = cJSON_Parse(content);
        nvs->saveConfig(settings);
        cJSON_AddStringToObject(setSettings, "status", "200");
    }
    httpd_resp_send(req, cJSON_PrintUnformatted(setSettings), strlen(cJSON_PrintUnformatted(setSettings)));
    free(content);
    return ESP_OK;
}

const httpd_uri_t HTTP_Server::setSettings = {
    .uri = "/setSettings",
    .method = HTTP_POST,
    .handler = HTTP_Server::set_settings,
    .user_ctx = NULL};

bool HTTP_Server::init()
{
    /* Generate default configuration */
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = 11;

    /* Empty handle to esp_http_server */
    httpd_handle_t server = NULL;

    /* Start the httpd server */
    if (httpd_start(&server, &config) == ESP_OK)
    {
        /* Register URI handlers */
        httpd_register_uri_handler(server, &mainPageHandler);
        httpd_register_uri_handler(server, &getFaviconIco);
        httpd_register_uri_handler(server, &getScriptJS);
        httpd_register_uri_handler(server, &getStyleCSS);
        httpd_register_uri_handler(server, &getMAC);
        httpd_register_uri_handler(server, &getSettings);
        httpd_register_uri_handler(server, &setSettings);
        return true;
    }
    return false;
}