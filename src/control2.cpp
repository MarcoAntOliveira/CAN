#include <Arduino.h>

unsigned long tempoAnterior = 0;
// Tempo dinâmico: 3 segundos para ler histórias, mas cai para 400ms na hora de pintar!
unsigned long intervaloAtual = 3000; 

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
        // Menu -> Avança para História 1 (Simula clique no botão SEGUIR)
        // Formato: Colorir(0), Seguir(1), R(0), G(0), B(0)
        Serial.println("0,1,0,0,0"); 
        intervaloAtual = 3000;  
        estadoFluxo = 1;
        break;

      case 1:
        // História 1 -> Avança para História 2 (Simula clique no botão SEGUIR)
        Serial.println("0,1,0,0,0"); 
        intervaloAtual = 3000;
        estadoFluxo = 2;
        break;

      case 2:
        // História 2 -> Entra na tela de Jogo (Simula clique no botão SEGUIR)
        Serial.println("0,1,0,0,0"); 
        intervaloAtual = 1500; // Tempo seguro de fade para o Pygame abrir a tela de jogo
        estadoFluxo = 3;
        break;

      case 3:
        // HORA DE PINTAR: Simula clique em COLORIR (1) + envia a cor correta do sensor simulado
        // Formato: Colorir(1), Seguir(0), R, G, B
        switch (faseJogo) {
          case 0: Serial.println("1,0,255,140,0"); break;  // Laranja corrigido para a faixa do Python
          case 1: Serial.println("1,0,50,180,50"); break;   // Verde corrigido
          case 2: Serial.println("1,0,140,40,200"); break;  // Roxo
          case 3: Serial.println("1,0,101,67,33"); break;   // Marrom
        }
        intervaloAtual = 2000; // Dá 2 segundos para o Python processar as partículas e pintura
        estadoFluxo = 4;
        break;

      case 4:
        // Avança de fase pós-pintura (Simula clique no botão SEGUIR)
        Serial.println("0,1,0,0,0"); 
        
        faseJogo++;
        if (faseJogo > 3) {
          faseJogo = 0;   
          estadoFluxo = 0; 
          intervaloAtual = 5000; // Tempo para ver a tela de encerramento
        } else {
          estadoFluxo = 3; 
          intervaloAtual = 1500; // Tempo seguro para o fade e carregamento da nova imagem
        }
        break;
    }
  } 
  else {
    // Estado de repouso: Nenhum botão apertado, nenhuma cor sendo enviada.
    // Sempre mantendo o padrão de 5 colunas que o Python exige!
    Serial.println("0,0,0,0,0");
    delay(50); 
  }
}