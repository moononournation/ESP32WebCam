// =============================== SETUP ======================================

// 1. Board setup (Uncomment):
// #define BOARD_WROVER_KIT
#define BOARD_ESP32CAM_AITHINKER

// ================================ CODE ======================================

#include <string.h>

#include <esp_eth.h>
#include <esp_event.h>
#include <esp_http_server.h>
#include <esp_log.h>
#include <esp_system.h>
#include <esp_timer.h>
#include <esp_wifi.h>
#include <freertos/event_groups.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <lwip/err.h>
#include <lwip/sys.h>
#include <sys/param.h>
#include <tcpip_adapter.h>
#include <nvs_flash.h>

// components
#include <esp_camera.h>

// local function
#include "camera.h"
#include "jpg_handler.h"
#include "wifi_ap.h"
#include "wifi_sta.h"
#include "http_server.h"

static const char *TAG = "WebCam";

void app_main()
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    //Initialize Camera
    ESP_LOGI(TAG, "init_camera()");
    init_camera();

    //Initialize WiFi
#if CONFIG_WIFI_AP == 1
    ESP_LOGI(TAG, "wifi_init_softap()");
    wifi_init_softap();
#else
    ESP_LOGI(TAG, "wifi_init_sta()");
    wifi_init_sta();
#endif

    // Initialize HTTP Server
    static httpd_handle_t server = NULL;

    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connect_handler, &server));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server));

    /* Start the server for the first time */
    server = start_webserver();

    httpd_register_uri_handler(server, &jpg);
    httpd_register_uri_handler(server, &jpg_stream);
}
