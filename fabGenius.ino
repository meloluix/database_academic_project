#define ledVermelho 2
#define ledVerde 3
#define ledAzul 4
#define ledAmarelo 5

int botaoVermelho = 6;
int botaoVerde = 7;
int botaoAzul = 8;
int botaoAmarelo = 9;

int buzzer = 11;

int indefinido = -1;
int rodadaAtual = 0;
int ledsRespondidos = 0;

#define rodadaLimite 20

int sequenciaLeds[rodadaLimite];

enum estados {
  PRONTO_PARA_PROXIMA_RODADA,
  USUARIO_JOGANDO,
  DERROTA,
  VITORIA
};

int sortear() {
  return random(ledVermelho, ledAmarelo + 1);
}

void iniciarJogo() {
  int jogo = analogRead(0);
  randomSeed(jogo);
  for (int i = 0; i < rodadaLimite; i++) {
    sequenciaLeds[i] = sortear();
  }
}

void reiniciarJogo() {
  delay(4000);
  rodadaAtual = 0;
  iniciarJogo();
}

void piscarLed(int corLed) {
  digitalWrite(corLed, HIGH);
  verificarSomLed(corLed);
  digitalWrite(corLed, LOW);
  delay(500);
}

void tocarSom(int frequencia) {
  tone(buzzer, frequencia);
  delay(500);
  noTone(buzzer);
}

void verificarSomLed(int corLed) {
  switch (corLed) {
    case ledVermelho:
      tocarSom(500);
      break;
    case ledVerde:
      tocarSom(1000);
      break;
    case ledAzul:
      tocarSom(1500);
      break;
    case ledAmarelo:
      tocarSom(2000);
      break;
  }
}
void checarResposta() {
  int resposta = checarCorBotao();
  Serial.println(resposta);
  delay(50);
  if (resposta == indefinido) {
    return;
  }
  if (resposta == sequenciaLeds[ledsRespondidos]) {
    ledsRespondidos++;
  } else {
    rodadaAtual = rodadaLimite + 2;  //Quando a rodadaAtual for 22, houve derrota, poderia ser 300 até, é simbólico.
  }
}

void gerarRodada() {
  for (int i = 0; i < rodadaAtual; i++) {
    piscarLed(sequenciaLeds[i]);
  }
}

void prepararProximaRodada() {
  rodadaAtual++;
  ledsRespondidos = 0;
  if (rodadaAtual <= rodadaLimite) {
    gerarRodada();
  }
}

int verificarEstadoAtual() {
  if (rodadaAtual <= rodadaLimite) {

    if (ledsRespondidos == rodadaAtual) {  //quando o jogador apertar todos os botoes na ordem
      return PRONTO_PARA_PROXIMA_RODADA;
    } else {  //quando o jogador nao apertar todos os botoes
      return USUARIO_JOGANDO;
    }
  } else if (rodadaAtual == rodadaLimite + 1) {  //quando o jogador acertar as 20 sequencias
    return VITORIA;
  } else {  //quando o jogador errar a sequencia
    return DERROTA;
  }
}

int checarCorBotao() {
  if (digitalRead(botaoVermelho) == LOW) {
    piscarLed(ledVermelho);
    return ledVermelho;
  }

  if (digitalRead(botaoVerde) == LOW) {
    piscarLed(ledVerde);
    return ledVerde;
  }

  if (digitalRead(botaoAzul) == LOW) {
    piscarLed(ledAzul);
    return ledAzul;
  }

  if (digitalRead(botaoAmarelo) == LOW) {
    piscarLed(ledAmarelo);
    return ledAmarelo;
  }
  return indefinido;
}

void tocarNotaLed(int frequencia, int duracao, int led) {
  digitalWrite(led, HIGH);
  tone(buzzer, frequencia);
  delay(duracao);
  digitalWrite(led, LOW);
  noTone(buzzer);
}

void finalizarJogoVitoria() {
  tocarNotaLed(523, 250, ledVermelho);  // C5
  tocarNotaLed(659, 250, ledVerde);     // E5
  tocarNotaLed(784, 250, ledAzul);      // G5
  tocarNotaLed(1047, 400, ledAmarelo);  // C6 (ponto alto)

  delay(100);

  tocarNotaLed(784, 250, ledAzul);      // G5
  tocarNotaLed(880, 250, ledVerde);     // A5
  tocarNotaLed(988, 250, ledVermelho);  // B5
  tocarNotaLed(1047, 400, ledAmarelo);  // C6 final

  noTone(buzzer);

  reiniciarJogo();
}

void finalizarJogoDerrota() {
  int notas[] = {
    1047,  // C6
    880,   // A5
    698,   // F5
    587,   // D5
    494,   // B4
    392,   // G4
    330    // E4 (final)
  };

  int duracoes[] = {
    250, 250, 300, 300, 350,
    400, 900  // Final mais longo
  };

  for (int i = 0; i < 7; i++) {
    tone(buzzer, notas[i]);

    // Acende todos os LEDs
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledAzul, HIGH);
    digitalWrite(ledAmarelo, HIGH);

    delay(duracoes[i]);

    // Desliga som e LEDs
    noTone(buzzer);
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAzul, LOW);
    digitalWrite(ledAmarelo, LOW);

    delay(70);  // Pausa entre notas
  }
  reiniciarJogo();
}

void setup() {
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAzul, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);

  pinMode(botaoVermelho, INPUT_PULLUP);
  pinMode(botaoVerde, INPUT_PULLUP);
  pinMode(botaoAzul, INPUT_PULLUP);
  pinMode(botaoAmarelo, INPUT_PULLUP);

  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);

  iniciarJogo();
}

void loop() {

  switch (verificarEstadoAtual()) {
    case DERROTA:
      Serial.println("Perdeu!");
      finalizarJogoDerrota();
      break;
    case VITORIA:
      Serial.println("Venceu!");
      finalizarJogoVitoria();
      break;
    case USUARIO_JOGANDO:
      Serial.println("Responde Logo!");
      checarResposta();
      break;
    case PRONTO_PARA_PROXIMA_RODADA:
      Serial.println("Bora lá mais uma meu patrão!");
      prepararProximaRodada();
      break;
  }
  delay(200);
}
