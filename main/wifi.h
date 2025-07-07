#ifndef WIFI_H
#define WIFI_H

// --- Macro
#define EXAMPLE_ESP_WIFI_SSID "esp_wroom_32"
#define EXAMPLE_ESP_WIFI_PASS "qwertyuiop"
#define EXAMPLE_ESP_WIFI_CHANNEL CONFIG_ESP_WIFI_CHANNEL
#define EXAMPLE_MAX_STA_CONN CONFIG_ESP_MAX_STA_CONN

// --- Include
#include "esp_event.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "nvs_flash.h"

// --- Variable
static const char* WIFI_SOFTAP_TAG = "wifi_softap";

// --- Function
void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
void wifi_init_softap(void);

#endif // WIFI_H