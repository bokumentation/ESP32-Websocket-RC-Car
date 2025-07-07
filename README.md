# ESP32 Websocket nggo DRV8833
**Note:** Hurung iso websoket. Lagi init wifi soft AP. Iki repo anyar sing pindah ke FULL BAHASA C. CPP ne ngangeli.Pan kapan pindah nggo cpp neh.

## Bahan
- GIT
- ESP-IDF Tools
- ESP-IDF VS Code Extension

## Carane Setting IDF-TOOLS
- Carane install ESP-IDF Tools -> [Link](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/windows-setup.html)
  - Ojo lali set path variable jenenge `IDF_PATH` karo `IDF_TOOLS_PATH` 
  - Contoh nggonku
    - IDF_PATH -> `D:\DEV\LIBS\embedded\esp-idf\frameworks\esp-idf-v5.4.1`
    - IDF_TOOLS_PATH -> `D:\DEV\LIBS\embedded\esp-idf\tools`
- Carane setting Extentions ne -> [Link](https://github.com/espressif/vscode-esp-idf-extension)
  - ⚠️ Ojo lali pilih framework sing wis `USE EXISTING SETUP`. Arahke ke folder instalasi ESP-IDF tools. 
  - CMake Extension e dipateni wae.

# PENTING! Ojo lali nggo ESP-IDF POWERSHELL ngeer ameh mbukak projek. (📌 Skip nek wis paham)
1. Bukak ESP-IDF Powershell
2. CD nandi sak karepmu. Misal nggonku `D:\DEV\PROJECTS\embedded\ESP32`
```bash
cd <folder karepmu. penting ojo dadi siji nang folder esp-idf tools e>
```
3. Clone `git clone` terus `cd`
```bash
git clone https://github.com/bokumentation/ESP32-Websocket-RC-Car.git
cd ./ESP32-Websocket-RC-Car
```
4. Terus bukak vscode e seko kene `code .`
```bash
code . 
```
## Carane nganggo ESP-IDF versi terminal
1. Set target (krusial)
  ```bash
  idf.py set-target esp32
  ```

2. Configure (Ra perlu. Skip wae)
  ```bash
  idf.py menuconfig
  ```

3. Build 
  ```bash
  idf.py build
  ```

4. Flash
  ```bash
  idf.py -p COM4 flash
  ```
  Note: Sesuaikan COMx yang nyambung ke PC/Laptop anda.

5. Monitoring
  ```bash
  idf.py -p COM4 monitor
  ```
  Note: Sesuaikan COMx yang nyambung ke PC/Laptop anda.