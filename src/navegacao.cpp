#include <Arduino.h>

int ValorRef, ValorSaida, AcaoPWM, PID=0;
double Referencia, Realimentacao, Erro, ErroAnterior, P=0, I=0, D=0, SetPoint;
float deltaTempo, UltimaAcao;

// Com deltaTempo = 0.002
double kp=60, ki=0.1, kd=0.00;

#define Ref       A0    // Leitura analógica da referência no pino A0
#define LeSaida   A1    // Leitura analógica da saída realimentada no pino A1
#define Atua1      3     // Vai atuar no PWM do pino 3
#define Atua2      4     // Vai atuar no PWM do pino 4
#define Atua3      5     // Vai atuar no PWM do pino 5

void setup() {

  Serial.begin(9600);

  pinMode(Ref, INPUT);
  pinMode(LeSaida, INPUT);
  pinMode(Atua1, OUTPUT);
  pinMode(Atua2, OUTPUT);
  pinMode(Atua3, OUTPUT);

}


 void Controle(){

    // Implementação PID

      Erro = Referencia - Realimentacao;

      //SetPoint = 70;
      //Erro = SetPoint - Realimentacao;

      UltimaAcao = millis();
      //Proporcional
      P = Erro * kp;
      //Integral
      I = I + (Erro * ki) * deltaTempo;
      if (I>20) {PID=20;}
      if (I<-20) {I=-20;}
      //Derivativo
      D = (Erro - ErroAnterior ) * kd / deltaTempo;
      ErroAnterior  = Erro;
      // Soma tudo
      PID = P + I + D;
      if (PID>80) {PID=80;}
      if (PID<1) {PID=1;}


      //analogWrite(Atua, PID);


      if (PID<10){
        digitalWrite(Atua1, LOW);
        digitalWrite(Atua2, LOW);
        digitalWrite(Atua3, LOW);
        }
      if (PID>10 and PID<20){
        digitalWrite(Atua1, HIGH);
        digitalWrite(Atua2, LOW);
        digitalWrite(Atua3, LOW);
        }
      if (PID>20 and PID<30){
        digitalWrite(Atua1, LOW);
        digitalWrite(Atua2, HIGH);
        digitalWrite(Atua3, LOW);
        }
      if (PID>30 and PID<40){
        digitalWrite(Atua1, HIGH);
        digitalWrite(Atua2, HIGH);
        digitalWrite(Atua3, LOW);
        }
      if (PID>40 and PID<50){
        digitalWrite(Atua1, LOW);
        digitalWrite(Atua2, LOW);
        digitalWrite(Atua3, HIGH);
        }
      if (PID>50 and PID<60){
        digitalWrite(Atua1, HIGH);
        digitalWrite(Atua2, LOW);
        digitalWrite(Atua3, HIGH);
        }
      if (PID>60 and PID<70){
        digitalWrite(Atua1, LOW);
        digitalWrite(Atua2, HIGH);
        digitalWrite(Atua3, HIGH);
        }
      if (PID>70){
        digitalWrite(Atua1, HIGH);
        digitalWrite(Atua2, HIGH);
        digitalWrite(Atua3, HIGH);
        }
}




void loop() {

  ValorRef = analogRead(Ref);
  ValorSaida = analogRead(LeSaida);

  Referencia = map(ValorRef, 0, 1023, 0, 100);
  Realimentacao = map(ValorSaida, 0, 1023, 0, 100);

  deltaTempo = (millis() - UltimaAcao) / 1000.0;

  if (deltaTempo>=0.002) {Controle();}

}
