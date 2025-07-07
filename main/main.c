#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "my_nvs_init.h"
#include "wifi.h"

void app_main(void) {
  my_nvs_init();
  ESP_LOGI(WIFI_SOFTAP_TAG, "ESP_WIFI_MODE_AP");
  wifi_init_softap();
}
