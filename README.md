## Platfomio com mais de uma main

[env:esp8266]
platform = espressif8266
board = nodemcuv2  ; ou o modelo da sua placa
framework = arduino
monitor_speed = 9600
build_src_filter = +<instrumentacao.cpp>
;upload_port = /dev/ttyUSB0


;[env:uno]
;platform = atmelavrp
;board = uno
;framework = arduino
;monitor_speed = 9600
;build_src_filter = +<central.cpp>
;upload_port = /dev/ttyUSB0  ; Porta do primeiro Arduino


[env:esp32]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 9600
src_filter = +<central.cpp>
;upload_port = /dev/ttyUSB0  ; Porta do primeiro Arduino
