#include <Arduino.h>

#define RELAY_PIN PA_7

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  digitalWrite(RELAY_PIN, HIGH);  // relé desenergizado -> NC fechado -> motor GIRA
  Serial.println("MOTOR ON (via NC)");
  delay(2000);

  digitalWrite(RELAY_PIN, LOW);   // relé energizado -> NC abre -> motor PARA
  Serial.println("MOTOR OFF");
  delay(2000);
}