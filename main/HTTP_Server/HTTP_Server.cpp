#include "HTTP_Server.h"

HTTP_Server::HTTP_Server(SharedData *p_data)
{
    this->shared_data = p_data;
}

SharedData *HTTP_Server::shared_data;

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

bool HTTP_Server::init()
{
    /* Generate default configuration */
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    /* Empty handle to esp_http_server */
    httpd_handle_t server = NULL;

    /* Start the httpd server */
    if (httpd_start(&server, &config) == ESP_OK)
    {
        /* Register URI handlers */
        httpd_register_uri_handler(server, &mainPageHandler);
        httpd_register_uri_handler(server, &getFaviconIco);
        httpd_register_uri_handler(server, &getInternalSettings);
        httpd_register_uri_handler(server, &getActualFrame);
        httpd_register_uri_handler(server, &getScriptJS);
        httpd_register_uri_handler(server, &getStyleCSS);
        return true;
    }
    return false;
}