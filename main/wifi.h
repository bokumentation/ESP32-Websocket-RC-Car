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
#include "lwip/err.h"
#include "lwip/sys.h"
#include "nvs_flash.h"

// --- Variable
/**
 * @brief Tag untuk logging spesifik Wi-Fi SoftAP.
 *
 * Variabel ini adalah pointer ke string konstanta yang digunakan sebagai tag
 * oleh fungsi-fungsi logging ESP-IDF (misalnya, `ESP_LOGI`, `ESP_LOGE`).
 * Penggunaan tag yang spesifik memungkinkan penyaringan output log di konsol
 * untuk hanya menampilkan pesan yang relevan dengan fungsionalitas Wi-Fi SoftAP,
 * sehingga memudahkan proses debugging.
 *
 * Kata kunci `static` membatasi visibilitas variabel ini hanya pada file `.c`
 * tempatnya didefinisikan (dalam hal ini, `wifi.c`), mencegah konflik nama
 * jika ada variabel dengan nama yang sama di file lain.
 *
 * Kata kunci `const` memastikan bahwa isi string yang ditunjuk oleh `WIFI_SOFTAP_TAG`
 * tidak dapat diubah setelah inisialisasi.
 */
static const char* WIFI_SOFTAP_TAG = "wifi_softap";

// --- Function
/**
 * @brief Fungsi penangan event Wi-Fi untuk mode SoftAP.
 *
 * Fungsi ini dipanggil oleh sistem event ESP-IDF setiap kali ada event Wi-Fi yang terjadi.
 * Saat ini, fungsi ini secara khusus menangani event koneksi dan diskoneksi stasiun (perangkat klien)
 * ke Access Point (AP) yang dibuat oleh ESP32.
 *
 * @param arg Argumen data kustom yang didaftarkan bersama dengan event handler. (Tidak digunakan saat ini)
 * @param event_base Base event dari event yang terjadi (misal: WIFI_EVENT).
 * @param event_id ID spesifik dari event yang terjadi (misal: WIFI_EVENT_AP_STACONNECTED).
 * @param event_data Pointer ke struktur data spesifik event, yang berisi informasi lebih lanjut tentang event tersebut.
 */
void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

/**
 * @brief Menginisialisasi mode Wi-Fi SoftAP pada ESP32.
 *
 * Fungsi ini mengkonfigurasi ESP32 untuk beroperasi sebagai Access Point (AP),
 * memungkinkan perangkat lain untuk terhubung kepadanya. Ini melibatkan langkah-langkah
 * seperti inisialisasi tumpukan jaringan, pembuatan event loop, pengaturan konfigurasi
 * Wi-Fi AP, pendaftaran event handler, dan memulai AP.
 *
 * SSID, kata sandi, dan saluran AP diambil dari definisi makro yang telah ditentukan
 * (misalnya, EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS). Jika kata sandi kosong,
 * AP akan diatur ke mode terbuka (tanpa kata sandi).
 *
 * @note Fungsi ini menggunakan `ESP_ERROR_CHECK` untuk memeriksa kesalahan pada setiap langkah.
 * Jika ada langkah yang gagal, program akan berhenti.
 */
void wifi_init_softap(void);

#endif // WIFI_H