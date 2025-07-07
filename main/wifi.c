#include "wifi.h"

void wifi_init_softap(void) {
  // Inisialisasi tumpukan jaringan IP. Ini harus dipanggil sekali sebelum penggunaan antarmuka jaringan apa pun.
  ESP_ERROR_CHECK(esp_netif_init());
  // Membuat event loop default sistem. Ini diperlukan untuk menangani event Wi-Fi.
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  // Membuat antarmuka jaringan default untuk Wi-Fi AP.
  esp_netif_create_default_wifi_ap();

  // Mengambil konfigurasi inisialisasi Wi-Fi default.
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  // Menginisialisasi driver Wi-Fi dengan konfigurasi yang diberikan.
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  // Mendaftarkan fungsi `wifi_event_handler` untuk menangani semua event Wi-Fi.
  // Ini memastikan bahwa fungsi kita akan dipanggil setiap kali event Wi-Fi terjadi.
  ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL));

  // Mendefinisikan struktur konfigurasi untuk Wi-Fi AP.
  wifi_config_t wifi_config = {
      .ap =
          {
              .ssid = EXAMPLE_ESP_WIFI_SSID,             // SSID (nama) dari Access Point
              .ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID), // Panjang SSID
              .channel = EXAMPLE_ESP_WIFI_CHANNEL,       // Saluran Wi-Fi
              .password = EXAMPLE_ESP_WIFI_PASS,         // Kata sandi AP
              .max_connection = EXAMPLE_MAX_STA_CONN,    // Jumlah maksimum stasiun yang dapat terhubung
#ifdef CONFIG_ESP_WIFI_SOFTAP_SAE_SUPPORT                // Jika dukungan WPA3 SAE diaktifkan di menuconfig
              .authmode = WIFI_AUTH_WPA3_PSK,            // Mode otentikasi WPA3-Personal (SAE)
              .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
#else /* CONFIG_ESP_WIFI_SOFTAP_SAE_SUPPORT */
              .authmode = WIFI_AUTH_WPA2_PSK, // Mode otentikasi WPA2-Personal (PSK)
#endif
              .pmf_cfg =
                  {
                      .required = true, // Mengaktifkan PMF (Protected Management Frames)
                  },
          },
  };
  // Jika kata sandi SSID kosong, atur mode otentikasi ke terbuka (tanpa kata sandi).
  if(strlen(EXAMPLE_ESP_WIFI_PASS) == 0) {
    wifi_config.ap.authmode = WIFI_AUTH_OPEN;
  }

  // Mengatur mode Wi-Fi ESP32 ke mode Access Point (AP).
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
  // Mengatur konfigurasi AP yang telah ditentukan ke antarmuka Wi-Fi AP.
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
  // Memulai antarmuka Wi-Fi AP.
  ESP_ERROR_CHECK(esp_wifi_start());

  // Mencatat informasi bahwa inisialisasi SoftAP telah selesai, bersama dengan SSID, kata sandi, dan saluran.
  ESP_LOGI(WIFI_SOFTAP_TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d", EXAMPLE_ESP_WIFI_SSID,
           EXAMPLE_ESP_WIFI_PASS, EXAMPLE_ESP_WIFI_CHANNEL);
}