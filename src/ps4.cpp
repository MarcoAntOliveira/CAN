#include <Arduino.h>
#include <PS4Controller.h>

#define PS4_MAC_ADDRESS "68:b4:50:01:34:5a"

void setup() {
  Serial.begin(115200);
  Serial.println("Aqui");

  PS4.begin(PS4_MAC_ADDRESS);

  Serial.println("Esperando controle...");
}

void loop() {

  if (PS4.isConnected()) {
    Serial.println("CONTROLE CONECTADO!");

    Serial.print("LX: ");
    Serial.println(PS4.LStickX());

    Serial.print("LY: ");
    Serial.println(PS4.LStickY());

    if (PS4.Cross()) {
      Serial.println("X pressionado");
    }

    delay(100);
  }
}