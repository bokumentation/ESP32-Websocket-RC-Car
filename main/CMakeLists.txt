idf_component_register(SRCS "main.c" "root_html.S"
                    INCLUDE_DIRS "."
                    REQUIRES "esp_wifi" "esp_event" "nvs_flash" "esp_netif" "esp_http_server" "driver" "freertos")