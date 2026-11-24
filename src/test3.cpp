#include <Arduino.h>

#define LED_PIN PC13

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  digitalWrite(LED_PIN, LOW);   // na Blue Pill, geralmente LOW = aceso
  Serial.println("LED ON");
  delay(500);

  digitalWrite(LED_PIN, HIGH);  // HIGH = apagado
  Serial.println("LED OFF");
  delay(500);
}