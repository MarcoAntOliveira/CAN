#include <Arduino.h>

// tempos de amostragem em milissegundos
const unsigned long T_gyro    = 2;
const unsigned long T_velAng  = 3;
const unsigned long T_velLong = 5;
const unsigned long T_velNorm = 7;

// variáveis para controle de tempo
unsigned long lastGyro    = 0;
unsigned long lastVelAng  = 0;
unsigned long lastVelLong = 0;
unsigned long lastVelNorm = 0;
unsigned long lastUpdate  = 0;  // controle do tempo contínuo

float t = 0.0;   // tempo contínuo em segundos
float dt = 0.0;  // passo de tempo calculado

// funções de simulação
float simulateGyro(float t) {
  return 30.0 * sin(2 * PI * 0.5 * t);
}
float simulateVelAngular(float t){
  return 2.0 * sin(t);
}
float simulateVelLongitudinal(float t){
  return 1.5 + 0.5 * sin(t * 0.5);
}
float simulateVelNormal(float t){
  return 0.2 * cos(t * 1.2);
}

void setup() {
  Serial.begin(9600);
  lastUpdate = millis();  // inicializa referência
}

void loop() {
  unsigned long agora = millis();

  // Atualiza tempo contínuo em segundos
  dt = (agora - lastUpdate) / 1000.0;
  t += dt;
  lastUpdate = agora;

  // Leitura do giroscópio
  if (agora - lastGyro >= T_gyro) {
    float gyro = simulateGyro(t);
    Serial.print("G:"); Serial.println(gyro);
    lastGyro = agora;
  }

  // Leitura da velocidade angular
  if (agora - lastVelAng >= T_velAng) {
    float velAng = simulateVelAngular(t);
    Serial.print("A:"); Serial.println(velAng);
    lastVelAng = agora;
  }

  // Leitura da velocidade longitudinal
  if (agora - lastVelLong >= T_velLong) {
    float velLong = simulateVelLongitudinal(t);
    Serial.print("L:"); Serial.println(velLong);
    lastVelLong = agora;
  }

  // Leitura da velocidade normal
  if (agora - lastVelNorm >= T_velNorm) {
    float velNorm = simulateVelNormal(t);
    Serial.print("N:"); Serial.println(velNorm);
    lastVelNorm = agora;
  }

  // Pequeno delay para reduzir carga de CPU
  delay(1);
}
