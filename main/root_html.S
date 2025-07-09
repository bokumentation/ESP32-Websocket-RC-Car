.section .rodata
    .global my_html_start
    .global my_html_end
my_html_start:
    .incbin "D:/DEV/PROJECTS/embedded/ESP32/01_esp32_websocket_rc_car/main/index.html"  // <--- Use absolute path
my_html_end: