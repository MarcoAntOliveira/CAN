#include <Arduino.h>

String inputString = "";
void processInput(String data);

void setup()
{
  Serial.begin(9600);
  while (!Serial) {}
}

void loop() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      processInput(inputString);
      inputString = "";
    } else {
      inputString += inChar;
    }
  }
}

void processInput(String data) {
  int sepIndex = data.indexOf(':');
  if (sepIndex == -1) return;

  char tipo = data.charAt(0);
  String valorStr = data.substring(sepIndex + 1);
  float valor = valorStr.toFloat();

  switch (tipo) {
    case 'G':
      Serial.print("Giroscópio: "); Serial.println(valor); break;
    case 'A':
      Serial.print("Vel. Angular: "); Serial.println(valor); break;
    case 'L':
      Serial.print("Vel. Longitudinal: "); Serial.println(valor); break;
    case 'N':
      Serial.print("Vel. Normal: "); Serial.println(valor); break;
    default:
      Serial.print("Tipo desconhecido "); Serial.println(data); break;
  }
}


