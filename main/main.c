// Websocket Server responsive to GET and POST request on the same URL

#include <esp_http_server.h>
#include <stdio.h>
#include <sys/param.h>

#include "esp_event.h"
#include "esp_log.h"
#include "esp_mac.h"   // Required for MACSTR and MAC2STR
#include "esp_netif.h" // Required for esp_ip4addr_ntoa
#include "esp_system.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "nvs_flash.h"

// --- AP Mode Definitions ---
#define ESP_WIFI_AP_SSID "ESP32_Motor_AP" // Your desired AP name
#define ESP_WIFI_AP_PASS "password123"    // Your desired AP password (min 8 characters)
#define ESP_WIFI_AP_CHANNEL 1             // Wi-Fi channel
#define ESP_MAX_STA_CONN 4                // Max simultaneous stations connected to ESP32 AP

static const char *TAG = "Websocket Server: ";
static const char *WIFI_AP_TAG = "WiFi AP: "; // Tag for AP-specific logs

// --- HTML Content for Web Server (Embedded from root_html.S) ---
// These are essential for serving your main HTML page
extern const unsigned char root_html_start[] asm("my_html_start");
extern const unsigned char root_html_end[] asm("my_html_end");

static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id,
                               void *event_data) {
  if(event_base == WIFI_EVENT) {
    switch(event_id) {
    case WIFI_EVENT_AP_START:
      ESP_LOGI(WIFI_AP_TAG, "SoftAP started");
      esp_netif_ip_info_t ip_info;
      esp_netif_get_ip_info(esp_netif_get_handle_from_ifkey("WIFI_AP_DEF"), &ip_info);

      char ip_str[IP4ADDR_STRLEN_MAX];                           // Buffer to hold the IP address string
      esp_ip4addr_ntoa(&ip_info.ip, ip_str, IP4ADDR_STRLEN_MAX); // Call with two arguments
      ESP_LOGI(WIFI_AP_TAG, "AP IP: %s", ip_str);                // Log the buffer
      break;
    case WIFI_EVENT_AP_STACONNECTED:
      wifi_event_ap_staconnected_t *event_sta_connected = (wifi_event_ap_staconnected_t *)event_data;
      ESP_LOGI(WIFI_AP_TAG, "Station " MACSTR " connected, AID=%d", MAC2STR(event_sta_connected->mac),
               event_sta_connected->aid);
      break;
    case WIFI_EVENT_AP_STADISCONNECTED:
      wifi_event_ap_stadisconnected_t *event_sta_disconnected = (wifi_event_ap_stadisconnected_t *)event_data;
      ESP_LOGI(WIFI_AP_TAG, "Station " MACSTR " disconnected, AID=%d", MAC2STR(event_sta_disconnected->mac),
               event_sta_disconnected->aid);
      break;
    default:
      break;
    }
  }
}

void wifi_connection() {
  // Initialize NVS (required for Wi-Fi)
  esp_err_t ret = nvs_flash_init();
  if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  // 1 - Wi-Fi/LwIP Init Phase
  esp_netif_init();                   // TCP/IP initiation
  esp_event_loop_create_default();    // event loop
  esp_netif_create_default_wifi_ap(); // WiFi Access Point

  wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&wifi_initiation));

  // 2 - Wi-Fi Configuration Phase
  // Register the event handler for Wi-Fi events
  ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL, NULL));

  wifi_config_t wifi_configuration = {
      .ap =
          {
              .ssid = ESP_WIFI_AP_SSID,
              .password = ESP_WIFI_AP_PASS,
              .ssid_len = strlen(ESP_WIFI_AP_SSID),
              .channel = ESP_WIFI_AP_CHANNEL,
              .authmode = WIFI_AUTH_WPA2_PSK, // WPA2-PSK for password protection
              .max_connection = ESP_MAX_STA_CONN,
              .beacon_interval = 100, // Default 100, can adjust
          },
  };

  if(strlen(ESP_WIFI_AP_PASS) == 0) {
    wifi_configuration.ap.authmode = WIFI_AUTH_OPEN; // Open network if no password
  }

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_configuration));

  // 3 - Wi-Fi Start Phase (and Connect Phase in AP is essentially starting)
  ESP_ERROR_CHECK(esp_wifi_start());

  ESP_LOGI(WIFI_AP_TAG, "AP setup finished. SSID:%s password:%s channel:%d", ESP_WIFI_AP_SSID, ESP_WIFI_AP_PASS,
           ESP_WIFI_AP_CHANNEL);
}

// Asynchronous response data structure
struct async_resp_arg {
  httpd_handle_t hd; // Server instance
  int fd;            // Session socket file descriptor for client
};

// The asynchronous response GET for /ws
static void generate_async_resp_get(void *arg) {
  // Data format to be sent from the server as a response to the client
  char http_string[250];
  char *data_string = "GET response from ESP32 websocket server ...";
  sprintf(http_string, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n", (int)strlen(data_string));
  // for sprintf
  // Initialize asynchronous response data structure
  struct async_resp_arg *resp_arg = (struct async_resp_arg *)arg;
  httpd_handle_t hd = resp_arg->hd;
  int fd = resp_arg->fd;

  // Send data to the client
  ESP_LOGI(TAG, "Executing queued work GET fd : %d", fd);
  httpd_socket_send(hd, fd, http_string, strlen(http_string), 0);
  httpd_socket_send(hd, fd, data_string, strlen(data_string), 0);

  free(arg);
}

// The asynchronous response POST for /ws
static void generate_async_resp_post(void *arg) {
  // Data format to be sent from the server as a response to the client
  char http_string[250];
  char *data_string = "POST response from ESP32 websocket server ...";
  sprintf(http_string, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n", (int)strlen(data_string));

  // Initialize asynchronous response data structure
  struct async_resp_arg *resp_arg = (struct async_resp_arg *)arg;
  httpd_handle_t hd = resp_arg->hd;
  int fd = resp_arg->fd;

  // Send data to the client
  ESP_LOGI(TAG, "Executing queued work POST fd : %d", fd);
  httpd_socket_send(hd, fd, http_string, strlen(http_string), 0);
  httpd_socket_send(hd, fd, data_string, strlen(data_string), 0);

  free(arg);
}

// Handler for HTTP GET requests to the root path ("/")
// This function will serve your main HTML page.
static esp_err_t root_get_handler(httpd_req_t *req) {
  ESP_LOGI(TAG, "Serving root page");
  httpd_resp_set_type(req, "text/html");
  // Cast to (const char *) because httpd_resp_send expects char*
  httpd_resp_send(req, (const char *)root_html_start, root_html_end - root_html_start);
  return ESP_OK;
}

// Initialize a queue for asynchronous communication for GET on /ws
static esp_err_t async_get_handler(httpd_req_t *req) {
  struct async_resp_arg *resp_arg = malloc(sizeof(struct async_resp_arg));
  if(resp_arg == NULL) { // Always check malloc return
    ESP_LOGE(TAG, "Failed to allocate memory for async_resp_arg");
    return ESP_ERR_NO_MEM;
  }
  resp_arg->hd = req->handle;
  resp_arg->fd = httpd_req_to_sockfd(req);
  ESP_LOGI(TAG, "Queuing work GET fd : %d", resp_arg->fd);
  httpd_queue_work(req->handle, generate_async_resp_get, resp_arg);
  return ESP_OK;
}

// Initialize a queue for asynchronous communication for POST on /ws
esp_err_t async_post_handler(httpd_req_t *req) {
  // Read data send by the client
  char content[100];
  size_t recv_size = MIN(req->content_len, sizeof(content) - 1); // -1 for null terminator
  int ret = httpd_req_recv(req, content, recv_size);
  if(ret <= 0) { // 0 for timeout, < 0 for error
    if(ret == HTTPD_SOCK_ERR_TIMEOUT) {
      httpd_resp_send_408(req);
    }
    return ESP_FAIL;
  }
  content[ret] = '\0';                              // Null-terminate the received data
  printf("Data send by the client: %s\n", content); // Print as string, not with precision specifier

  // Generate server response queue
  struct async_resp_arg *resp_arg = malloc(sizeof(struct async_resp_arg));
  if(resp_arg == NULL) { // Always check malloc return
    ESP_LOGE(TAG, "Failed to allocate memory for async_resp_arg");
    return ESP_ERR_NO_MEM;
  }
  resp_arg->hd = req->handle;
  resp_arg->fd = httpd_req_to_sockfd(req);
  ESP_LOGI(TAG, "Queuing work POST fd : %d", resp_arg->fd);
  httpd_queue_work(req->handle, generate_async_resp_post, resp_arg);
  return ESP_OK;
}

// URI handler for the root path (/)
static const httpd_uri_t uri_root = {
    .uri = "/", // The root URL
    .method = HTTP_GET,
    .handler = root_get_handler,
    .user_ctx = NULL,
};

// Create URI (Uniform Resource Identifier) handler structure for GET
// for the server which is added to default gateway
static const httpd_uri_t uri_get = {
    .uri = "/ws", // URL added to WiFi's default gateway for WebSocket communication
    .method = HTTP_GET,
    .handler = async_get_handler,
    .user_ctx = NULL,
};

// URI handler structure for POST
httpd_uri_t uri_post = {.uri = "/ws", .method = HTTP_POST, .handler = async_post_handler, .user_ctx = NULL};

static void websocket_app_start(void) {
  httpd_handle_t server = NULL;
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();

  // Start the httpd server
  ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
  if(httpd_start(&server, &config) == ESP_OK) {
    // Registering the uri_handlers
    ESP_LOGI(TAG, "Registering URI handler for '/'");
    httpd_register_uri_handler(server, &uri_root); // Register handler for the root path

    ESP_LOGI(TAG, "Registering URI handler for '/ws' GET");
    httpd_register_uri_handler(
        server, &uri_get); // This handles the GET requests to /ws (which is often the WebSocket handshake)

    ESP_LOGI(TAG, "Registering URI handler for '/ws' POST");
    httpd_register_uri_handler(server, &uri_post); // This handles the POST requests to /ws
  } else {
    ESP_LOGE(TAG, "Error starting HTTP server!");
  }
}

void app_main(void) {
  // Initialize NVS and start Wi-Fi in AP mode
  wifi_connection();

  // Give Wi-Fi some time to start up before starting the server
  vTaskDelay(pdMS_TO_TICKS(1500));

  // Start the HTTP and WebSocket (via HTTP server)
  websocket_app_start();
}