#ifndef MY_NVS_INIT_H
#define MY_NVS_INIT_H

// --- Macro

// --- Include
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

// --- Variable

// --- Function
/**
 * @brief Menginisialisasi flash Non-Volatile Storage (NVS).
 * Fungsi ini menyiapkan partisi NVS agar siap untuk operasi baca dan tulis data. Fungsi ini akan mencoba
 * menginisialisasi NVS, dan jika mendeteksi bahwa partisi NVS rusak atau perlu diperbarui (misalnya, karena versi NVS
 * baru atau tidak ada halaman kosong), maka partisi akan dihapus dan diinisialisasi ulang.
 *
 * Ini memastikan bahwa NVS selalu dalam kondisi siap pakai untuk menyimpan data
 * yang bersifat permanen, seperti kredensial Wi-Fi atau pengaturan spesifik perangkat,
 * bahkan setelah perangkat dihidupkan ulang.
 *
 * @note Fungsi ini menggunakan ESP_ERROR_CHECK yang akan menyebabkan program berhenti
 * jika terjadi kesalahan kritis saat inisialisasi NVS.
 */
void my_nvs_init();

#endif // MY_NVS_INIT_H