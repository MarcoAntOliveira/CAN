#include <QMC5883LCompass.h>

QMC5883LCompass compass;

void setup() {
  // Inicializa a comunicação serial
  Serial.begin(9600);
  
  // Inicializa o sensor
  compass.init();
  
  Serial.println("Magnetômetro inicializado com sucesso!");
}

void loop() {
  compass.read();
  
  // Envia os dados separados por vírgula para facilitar o "split" no Python
  Serial.print(compass.getX());
  Serial.print(",");
  Serial.print(compass.getY());
  Serial.print(",");
  Serial.println(compass.getZ());

  delay(100); // 10Hz é uma boa taxa para o ROS
}