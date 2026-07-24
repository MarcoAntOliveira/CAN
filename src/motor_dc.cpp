#include <Arduino.h>
// Definicao dos pinos do ESP32 conectados ao driver de motor
#define IN1 13  // Canal A - Sentido 1
#define IN2 12  // Canal A - Sentido 2
#define IN3 27  // Canal B - Sentido 1
#define IN4 26  // Canal B - Sentido 2

// --- Funcoes Auxiliares de Controle ---

void moverFrente() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moverTras() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void pararMotores() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void setup() {
  Serial.begin(115200);
  Serial.println("--- Teste de Driver de Motor com ESP32 ---");

  // Configura os pinos do driver como SAIDA
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Garante que os motores iniciem desligados
  pararMotores();
}

void loop() {
  Serial.println(">> Mover para FRENTE");
  moverFrente();
  delay(2000);

  Serial.println(">> PARAR");
  pararMotores();
  delay(1000);

  Serial.println(">> Mover para TRAS");
  moverTras();
  delay(2000);

  Serial.println(">> PARAR");
  pararMotores();
  delay(2000);
}

