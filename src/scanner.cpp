#include <Arduino.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);

  SerialBT.begin("ESP32_SCAN", true); // modo master

  Serial.println("Iniciando scan Bluetooth...");

  BTScanResults *results = SerialBT.discover(10000);

  if (results == nullptr) {
    Serial.println("Nenhum dispositivo encontrado");
    return;
  }

  Serial.print("Dispositivos encontrados: ");
  Serial.println(results->getCount());

  for (int i = 0; i < results->getCount(); i++) {

    BTAdvertisedDevice *device = results->getDevice(i);

    Serial.print("Nome: ");
    Serial.println(device->getName().c_str());

    Serial.print("MAC: ");
    Serial.println(device->getAddress().toString().c_str());

    Serial.println("--------------------");
  }
}

void loop() {

}