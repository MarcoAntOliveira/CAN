#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define R_EN 7
#define L_EN 8
#define R_PWM 9
#define L_PWM 10

const int potPin = A3;
const int potMax = 681;

float normalizar(int valor, int maximo) {
  float n = (float)valor / maximo;
  return constrain(n, 0.0, 1.0);
}

void mostrar(int rpm) {
  lcd.setCursor(0, 0);
  lcd.print("RPM: ");
  lcd.print(rpm);
  lcd.print("    ");

  lcd.setCursor(0, 1);
  lcd.print("Seconds: ");
  lcd.print(millis() / 1000);
  lcd.print("    ");
}

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(R_EN, OUTPUT);
  pinMode(L_EN, OUTPUT);
  pinMode(R_PWM, OUTPUT);
  pinMode(L_PWM, OUTPUT);

  Serial.begin(115200);
}

void loop() {
  // Para controlar com o potenciômetro, use isto no lugar de PWM = 150:
  // int PWM = normalizar(analogRead(potPin), potMax) * 255;
  int PWM = 150;
  int rpmEstimado = PWM * 120L / 255;

  // Frente
  digitalWrite(R_EN, HIGH);
  digitalWrite(L_EN, HIGH);   // no BTS7960 ambos ENs costumam ficar HIGH
  analogWrite(R_PWM, PWM);
  analogWrite(L_PWM, 0);
  mostrar(rpmEstimado);
  delay(1000);

  // Parado
  analogWrite(R_PWM, 0);
  analogWrite(L_PWM, 0);
  digitalWrite(R_EN, LOW);
  digitalWrite(L_EN, LOW);
  mostrar(0);
  delay(1000);
}