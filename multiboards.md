# Configuração de Múltiplas Placas no PlatformIO

Este guia mostra como configurar múltiplas placas (ESP32 e ESP8266) em um único projeto do PlatformIO, facilitando o upload para dispositivos conectados em diferentes portas USB.

---

## Identificação das Placas

### ESP32
```bash
udevadm info -q all -n /dev/ttyUSB0 | grep ID_SERIALE: ID_SERIAL=1a86_USB_Serial
```

### ESP8266
Execute o mesmo comando para identificar o serial único:
```bash
udevadm info -q all -n /dev/ttyUSBX | grep ID_SERIAL
```

---

## Regras Udev

Crie o arquivo `/etc/udev/rules.d/99-platformio-multi.rules`:

```udev
SUBSYSTEM=="tty", ATTRS{idVendor}=="1a86", ATTRS{serial}=="SERIAL_ESP32", SYMLINK+="ttyESP32"
SUBSYSTEM=="tty", ATTRS{idVendor}=="1a86", ATTRS{serial}=="SERIAL_ESP8266", SYMLINK+="ttyESP8266"
```

> Substitua `SERIAL_ESP32` e `SERIAL_ESP8266` pelos valores reais de `ID_SERIAL` obtidos com o comando `udevadm`.

Recarregue as regras:
```bash
sudo udevadm control --reload-rules
sudo udevadm trigger
```

Agora os dispositivos estarão disponíveis em:
- `/dev/ttyESP32`
- `/dev/ttyESP8266`

---

## Configuração do `platformio.ini`

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
upload_port = /dev/ttyESP32

[env:esp8266]
platform = espressif8266
board = nodemcuv2
framework = arduino
monitor_speed = 115200
upload_port = /dev/ttyESP8266
```

---

## Uso

- Upload para ESP32:
```bash
pio run -t upload -e esp32dev
```

- Upload para ESP8266:
```bash
pio run -t upload -e esp8266
```

- Abrir monitor serial:
```bash
pio device monitor -e esp32dev
pio device monitor -e esp8266
```

---

## Observações

- Usar `SYMLINK` garante que as placas sempre terão o mesmo nome, mesmo mudando a porta USB.
- Útil para trabalhar com múltiplos dispositivos em uma régua USB.
