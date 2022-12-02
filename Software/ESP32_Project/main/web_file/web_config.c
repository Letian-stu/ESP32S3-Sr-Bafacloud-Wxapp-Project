/*
 * @Author: letian
 * @Date: 2022-11-30 09:21
 * @LastEditors: letian
 * @LastEditTime: 2022-12-02 16:19
 * @FilePath: \ESP32_Project\main\web_file\web_config.c
 * @Description: )
 * Copyright (c) 2022 by letian 1656733965@qq.com, All Rights Reserved.
 */
#include "web_config.h"

#define TAG "webconfig"

nvs_handle_t wifi_config;
httpd_handle_t server = NULL;
httpd_config_t config = HTTPD_DEFAULT_CONFIG();
char recvwifiname[32] = {'\0'};
char recvwifissid[32] = {'\0'};


#define FILE_PATH_MAX (ESP_VFS_PATH_MAX + CONFIG_SPIFFS_OBJ_NAME_LEN)

#define SCRATCH_BUFSIZE 8192
struct file_server_data
{
    char base_path[ESP_VFS_PATH_MAX + 1];
    char scratch[SCRATCH_BUFSIZE];
};

static esp_err_t http_resp_dir_html(httpd_req_t *req, const char *dirpath)
{
    extern const unsigned char config_start[] asm("_binary_config_html_start");
    extern const unsigned char config_end[] asm("_binary_config_html_end");
    size_t size = (config_end - config_start);
    httpd_resp_send(req, (const char *)config_start, size);
    return ESP_OK;
}

static const char *get_path_from_uri(char *dest, const char *base_path, const char *uri, size_t destsize)
{
    const size_t base_pathlen = strlen(base_path);
    size_t pathlen = strlen(uri);
    const char *quest = strchr(uri, '?');
    if (quest)
    {
        pathlen = MIN(pathlen, quest - uri);
    }
    const char *hash = strchr(uri, '#');
    if (hash)
    {
        pathlen = MIN(pathlen, hash - uri);
    }
    if (base_pathlen + pathlen + 1 > destsize)
    {
        return NULL;
    }
    strcpy(dest, base_path);
    strlcpy(dest + base_pathlen, uri, pathlen + 1);
    return dest + base_pathlen;
}

static esp_err_t index_get_handler(httpd_req_t *req)
{
    char filepath[FILE_PATH_MAX];
    const char *filename = get_path_from_uri(filepath, ((struct file_server_data *)req->user_ctx)->base_path,
                                            req->uri, sizeof(filepath));
    if (!filename)
    {
        ESP_LOGE(TAG, "Filename is too long");
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Filename too long");
        return ESP_FAIL;
    }

    if (filename[strlen(filename) - 1] == '/')
    {
        printf("return http_resp_dir_html_OK! filepath:%s \n", filepath);

        return http_resp_dir_html(req, filepath);
    }
    return ESP_OK;
}

static esp_err_t wificonfig_handler(httpd_req_t *req)
{
    int total_len = req->content_len;
    int cur_len = 0;
    char *buf = ((struct file_server_data *)(req->user_ctx))->scratch;
    int received = 0;
    if (total_len >= SCRATCH_BUFSIZE)
    {
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "content too long");
        return ESP_FAIL;
    }
    while (cur_len < total_len)
    {
        received = httpd_req_recv(req, buf + cur_len, total_len);
        if (received <= 0)
        {
            /* Respond with 500 Internal Server Error */
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to post control value");
            return ESP_FAIL;
        }
        cur_len += received;
    }

    ESP_LOGI(TAG, "recv len = %d data = %s", total_len, buf);
    cJSON *root = cJSON_Parse(buf);

    char *wifi_name = cJSON_GetObjectItem(root, "wifi_name")->valuestring;
    char *wifi_ssid = cJSON_GetObjectItem(root, "wifi_ssid")->valuestring;

    uint8_t namelen = strlen(wifi_name) + 1; 
    uint8_t ssidlen = strlen(wifi_ssid) + 1; 
    if(namelen > 32 || ssidlen > 32)
    {
        ESP_LOGE(TAG, "name or ssid too long");
    }
    strcpy(recvwifiname, wifi_name);
    strcpy(recvwifissid, wifi_ssid);
    printf("len=%d name=%s\n", namelen, recvwifiname);
    printf("len=%d ssid=%s\n", ssidlen, recvwifissid);

    esp_err_t err = nvs_open("nvs", NVS_READWRITE, &wifi_config);
    if (err != ESP_OK)
    {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
    else
    {
        err = nvs_set_u8(wifi_config, WIFINAMELEN, namelen);
        printf((err != ESP_OK) ? "set wifi name len Failed!\n" : "set len ok\n");
        err = nvs_set_str(wifi_config, WIFINAME, recvwifiname);
        printf((err != ESP_OK) ? "set name Failed!\n" : "set name ok\n");
        err = nvs_set_u8(wifi_config, WIFISSIDLEN, ssidlen);
        printf((err != ESP_OK) ? "set wifi ssid len Failed!\n" : "set len ok\n");
        err = nvs_set_str(wifi_config, WIFISSID, recvwifissid);
        printf((err != ESP_OK) ? "set wifi ssid Failed!\n" : "set ssid ok\n");
        //commit
        err = nvs_commit(wifi_config);
        // Close
        nvs_close(wifi_config);
    }
    cJSON_Delete(root);
    httpd_resp_sendstr(req, "Link wifi successfully");

    //发送配置事件,切换到STA模式
    xEventGroupSetBits(Event_Group, CONFIGWIFIOK);
    return ESP_OK;
}

/* Function to start the file server */
esp_err_t start_wifi_config_server(const char *base_path)
{
    static struct file_server_data *server_data = NULL;

    if (server_data)
    {
        ESP_LOGE(TAG, "wifi_config_server already started");
        return ESP_ERR_INVALID_STATE;
    }

    /* Allocate memory for server data */
    server_data = calloc(1, sizeof(struct file_server_data));
    if (!server_data)
    {
        ESP_LOGE(TAG, "Failed to allocate memory for server data");
        return ESP_ERR_NO_MEM;
    }
    strlcpy(server_data->base_path, base_path,
            sizeof(server_data->base_path));

    /* Use the URI wildcard matching function in order to
     * allow the same handler to respond to multiple different
     * target URIs which match the wildcard scheme */
    config.uri_match_fn = httpd_uri_match_wildcard;

    ESP_LOGI(TAG, "Starting HTTP Server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to start file server!");
        return ESP_FAIL;
    }

    /* URI handler for getting uploaded files */
    httpd_uri_t index = {
        .uri = "/*", // Match all URIs of type /path/to/file
        .method = HTTP_GET,
        .handler = index_get_handler,
        .user_ctx = server_data // Pass server data as context
    };
    httpd_register_uri_handler(server, &index);

    /* URI handler for getting uploaded files */
    httpd_uri_t wificonfig = {
        .uri = "/wificonfig", // Match all URIs of type /path/to/file
        .method = HTTP_POST,
        .handler = wificonfig_handler,
        .user_ctx = server_data // Pass server data as context
    };
    httpd_register_uri_handler(server, &wificonfig);
    return ESP_OK;
}
