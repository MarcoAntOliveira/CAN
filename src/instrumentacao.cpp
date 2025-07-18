#include <Arduino.h>


float t = 0;
float dt = 0.1; // intervalo de amostragem (100ms)
float simulateGyro(float t);

void setup() {
  Serial.begin(9600);

}

void loop() {
  float gyro = simulateGyro(t);

  // Envia para a Serial padrão (monitor serial)
  Serial.println(gyro);

  
 delay(500);
  t += dt;
}

// Simula dados de giroscópio com oscilação senoidal
float simulateGyro(float t) {
  return 30.0 * sin(2 * PI * 0.5 * t); // 30°/s de amplitude, 0.5 Hz
}
