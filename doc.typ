#let tabela-sensores = figure(
  table(
    columns: 2,

     table.header( [*Sensor*],[*Tempo de Amostragem*]),
      [Giroscópio], [3.20 ms],
      [Vel. Angular], [5.36 ms],
      [Vel. Longitudinal], [2.52 ms],
      [Vel. Normal], [3.12 ms],

    align: (left, center),
    stroke: 0.8pt,
    inset: 6pt,
  ),
  caption: [Tabela de tempos de amostragem dos sensores embarcados.]
)

== Rede CAN

== 1. introdução
Este projeto destina-se a implementação tanto de software quanto de hardware de uma rede CAN.

== 2. Sensoriamento
O microcontrolador responsável pelo sensoriamento, será um esp8266,  onde serão utilizados os
pinos 3v3 para alimentação, GND que é o Terra e o pino TX , que é o envio de informações para central
=== 2.1  implementação

A simulação dos sensores será feita através de funções que as representam
```cpp
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
```
onde t é dado em milisegundos

=== 2.2 Envio de informações
para ser enviado as informações para a central foi dimensionado tempos de amostragem para cada um
dos sensores , que pode ser vizualizados na tabela abaixo.
#tabela-sensores
A verificação do tempo correto de envio , ocorre pelas variaveis agora e lastGyro, se a operação for
igual ou maior  ao tempo de amostragem vai ser enviado o valor para central.

```cpp
  unsigned long agora = millis();

  // Atualiza tempo contínuo (aproximado)
  t += dt;

  // Leitura do giroscópio
  if (agora - lastGyro >= T_gyro) {
    float gyro = simulateGyro(t);
    Serial.print("G:"); Serial.println(gyro);
    lastGyro = agora;
  }
```
O codigo acima  é identico para todas os outros sensores.
== 2.3 codigo provisorio da central

 No codigo abaixo ele lê a porta serial , e quando   houver \n , ele chama a função processInput
 para processar a entrada de qual sensores , se trata a informação.
```cpp
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
```
A função processInput verifica pelo primeiro caractere de qual sensor se trata
e o printa com sua respectiva label

```cpp

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

```

