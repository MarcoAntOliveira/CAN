#include <Arduino.h>

unsigned long tempoAnterior = 0;
// Tempo dinâmico: 3 segundos para ler histórias, mas cai para 400ms na hora de pintar!
unsigned long intervaloAtual = 3000; 

const int COR_LARANJA[3] = {255, 165, 0};
const int COR_VERDE[3]   = {0, 255, 0};
const int COR_ROXO[3]    = {140, 40, 200};
const int COR_MARROM[3]  = {101, 67, 33};

int estadoFluxo = 0; // 0: Menu, 1: Hist1, 2: Hist2, 3: Mandar Cor, 4: Passar Fase
int faseJogo = 0;    // 0 a 3

void setup() {
  Serial.begin(115200);
}

void loop() {
  unsigned long tempoAtual = millis();

  if (tempoAtual - tempoAnterior >= intervaloAtual) {
    tempoAnterior = tempoAtual;

    switch (estadoFluxo) {
      case 0:
        Serial.println("0,1"); 
        intervaloAtual = 3000;  
        estadoFluxo = 1;
        break;

      case 1:
        Serial.println("0,1"); 
        intervaloAtual = 3000;
        estadoFluxo = 2;
        break;

      case 2:
        Serial.println("0,1"); 
        intervaloAtual = 400; // Tempo para o Pygame abrir a tela de jogo
        estadoFluxo = 3;
        break;

      case 3:
        // ENVIA A COR E SÓ A COR!
        switch (faseJogo) {
          case 0: Serial.println("255,165,0"); break; // Laranja
          case 1: Serial.println("0,255,0");   break; // Verde
          case 2: Serial.println("140,40,200"); break; // Roxo
          case 3: Serial.println("101,67,33");  break; // Marrom
        }
        intervaloAtual = 2000; // Dá 2 segundos para o Python processar a pintura com calma
        estadoFluxo = 4;
        break;

      case 4:
        Serial.println("0,1"); // Avança de fase
        
        faseJogo++;
        if (faseJogo > 3) {
          faseJogo = 0;   
          estadoFluxo = 0; 
          intervaloAtual = 4000; 
        } else {
          estadoFluxo = 3; 
          intervaloAtual = 800; // Tempo seguro para carregar a próxima imagem do Pygame
        }
        break;
    }
  } 
  else {
    // === O SEGREDO ESTÁ AQUI ===
    // Só envia "0,0" se NÃO for o momento exato de enviar comandos pelos cases acima
    // E adicionamos um delay ligeiramente maior para não entupir o buffer do canal Serial
    Serial.println("0,0");
    delay(40); 
  }
}