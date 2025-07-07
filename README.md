# ESP32 Websocket untuk DRV8833
**Note:** Hurung iso websoket. Lagi init wifi soft AP.

## Carane nganggo ESP-IDF
1. Git clone repositori terus navigasi ke folder proyek.
  ```bash
  git clone https://github.com/bokumentation/ESP32-Websocket-RC-Car.git
  cd ESP32-Websocket-RC-Car
  ```

2. Set target (krusial)
  ```bash
  idf.py set-target esp32
  ```

3. Configure (Ra perlu. Skip wae)
  ```bash
  idf.py menuconfig
  ```

4. Build 
  ```bash
  idf.py build
  ```

5. Flash
  ```bash
  idf.py -p COM4 flash
  ```
  Note: Sesuaikan COMx yang nyambung ke PC/Laptop anda.

6. Monitoring
  ```bash
  idf.py -p COM4 monitor
  ```
  Note: Sesuaikan COMx yang nyambung ke PC/Laptop anda.