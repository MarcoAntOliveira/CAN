#include <Arduino.h>
void setup()
{
  Serial.begin(9600); // Para o Monitor Serial
  Serial.println("Esperando dados do ESP8266...");
}

void loop()
{
  if (Serial.available())
  {
    int c = Serial.read();
    Serial.println(c);
    delay(100);
  }
}
