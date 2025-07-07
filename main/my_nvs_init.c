#include "my_nvs_init.h"

void my_nvs_init() {
  // Coba inisialisasi NVS flash.
  esp_err_t ret = nvs_flash_init();
  // Periksa apakah inisialisasi gagal karena NVS rusak atau versi baru ditemukan.
  if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    // Jika ya, hapus partisi NVS.
    ESP_ERROR_CHECK(nvs_flash_erase());
    // Coba inisialisasi NVS lagi setelah penghapusan.
    ret = nvs_flash_init();
  }
  // Periksa apakah inisialisasi akhir berhasil; jika tidak, hentikan program.
  ESP_ERROR_CHECK(ret);
}