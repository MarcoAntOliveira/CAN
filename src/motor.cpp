#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); //first I2C address, the number of columns, and the number of rows

#define R_EN 4
#define L_EN 5
#define R_PWM 6
#define L_PWM 7


void setup() {
  // put your setup code here, to run once:

  lcd.init();
  lcd.backlight();

  
  pinMode(R_EN, OUTPUT);
  pinMode(L_EN, OUTPUT);
  pinMode(R_PWM, OUTPUT);
  pinMode(L_PWM, OUTPUT);\
  

  Serial.begin(9600);
  

}

void loop() {
  // put your main code here, to run repeatedly:

  //Forward Motion
  
  int PWM = 150;
  int RPM_estimado = (PWM*120/255);
  
  digitalWrite(R_EN, LOW);
  digitalWrite(L_EN, LOW);
  digitalWrite(R_PWM, 150); // (0-255) controla potência entregue ao motor (Pulse Width Modulation)
  digitalWrite(L_PWM, 0);
  
  


  lcd.setCursor(0, 0); // (x,y)
  lcd.print("RPM: ");
  lcd.print(RPM_estimado);

  delay(1000);

  //Stop
  
  digitalWrite(R_EN, LOW);
  digitalWrite(L_EN, LOW);
  digitalWrite(R_PWM, 0);
  digitalWrite(L_PWM, 0);
  

  lcd.setCursor(0,1);
  lcd.print("Seconds: ");
  lcd.print(millis()/1000);

  delay(1000);

}