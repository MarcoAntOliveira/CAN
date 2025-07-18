## Platfomio com mais de uma main
```
[env:uno]
platform = atmelavr
board = uno
framework = arduino
build_src_filter = +<central.cpp>
upload_port = /dev/ttyUSB0  ; Porta do primeiro Arduino
```
