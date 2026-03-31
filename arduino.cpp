/*
 * =========================================================================
 * PROJETO HORTA MÁGICA
 * =========================================================================
 */

// Inclusão das Bibliotecas
#include <LiquidCrystal.h>  // Para o display LCD
#include <dht.h>            // Para o sensor DHT11

// --- CONFIGURAÇÕES DOS PINOS ---

// Pinos do DHT11 (Umidade do Ar)
#define DHT_DATA_PIN 8      // Pino de DADOS do DHT11
#define DHT_POWER_PIN 9     // Pino de energia do DHT11

// Pinos do Sensor de Umidade de Solo
#define SOLO_SENSOR_PIN A2  // Pino ANALÓGICO de sinal
#define SOLO_POWER_PIN 10   // Pino DIGITAL usado para alimentar o sensor

// Pinos do Sensor de Chuva
#define CHUVA_SENSOR_PIN A3 // Pino ANALÓGICO de sinal
#define CHUVA_POWER_PIN 8   // Pino DIGITAL usado para alimentar o sensor

// --- CONFIGURAÇÕES DE COMPORTAMENTO ---

// Calibração do Sensor de Solo (IMPORTANTE!)
const int VALOR_SOLO_SECO = 600;    // Acima deste valor, a horta "pede água"
const int VALOR_SOLO_MOLHADO = 300; // Abaixo deste valor, a horta "agradece"

// Calibração do Sensor de Chuva
// (Valor analógico. Seco = ~1023, Molhado = < 500.)
const int VALOR_CHUVA_DETECTADA = 500; // Abaixo deste valor, considera "chovendo"

// Faixas Ideais de Ar
const float MIN_TEMP = 18.0;   // Temperatura mínima em Celsius
const float MAX_TEMP = 28.0;   // Temperatura máxima em Celsius
const float MIN_UMID_AR = 30.0;
const float MAX_UMID_AR = 70.0;

// Temporizadores (em milissegundos)
const long INTERVALO_LEITURA = 5000;       // Ler sensores a cada 5 segundos
const long TEMPO_MSG_OBRIGADO = 3000;      // Mostrar "obrigada" por 3 segundos
const long TEMPO_PARA_TER_SEDE = 10000;    // 10 seg (para testar!)

// 4. Inicialização dos componentes
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Create an LCD object.
dht DHT;                               // Criar um objeto DHT

// 5. Variáveis Globais de Controle
float tempAr, umidAr;
int umidSolo, valChuva;

bool hortaEstaComSede = false;
bool mostrarMsgObrigado = false;
unsigned long tempoInicioSede = 0;
unsigned long tempoInicioRegada = 0;
unsigned long tempoUltimaLeitura = 0;

// =========================================================================
//                                 SETUP
// =========================================================================

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando a Horta Mágica...");

  // Inicializa o LCD
  lcd.begin(16, 2);

  // Configura pinos de ALIMENTAÇÃO dos sensores como SAÍDA
  pinMode(SOLO_POWER_PIN, OUTPUT);
  pinMode(CHUVA_POWER_PIN, OUTPUT);
  pinMode(DHT_POWER_PIN, OUTPUT);

  // Garante que os sensores intermitentes comecem desligados
  digitalWrite(SOLO_POWER_PIN, LOW);
  digitalWrite(CHUVA_POWER_PIN, LOW);
  digitalWrite(DHT_POWER_PIN, LOW);

  // Inicializa o sensor DHT11

  delay(2000); // Espera tudo estabilizar
}

// =========================================================================
//                           LOOP PRINCIPAL
// =========================================================================
void loop() {
  unsigned long tempoAtual = millis();

  // --- 1. Bloco de Leitura dos Sensores ---
  if (tempoAtual - tempoUltimaLeitura >= INTERVALO_LEITURA) {
    tempoUltimaLeitura = tempoAtual;

    // Lê os sensores (as funções internas cuidam de ligar/desligar)
    umidSolo = lerSensorUmidadeSolo();
    valChuva = lerSensorChuva();

    // Loop de inicialização e leitura DHT11
    digitalWrite(DHT_POWER_PIN, HIGH);
    delay(500);
    int readData = DHT.read11(DHT_DATA_PIN);
    tempAr = DHT.temperature;
    umidAr = DHT.humidity;
    digitalWrite(DHT_POWER_PIN, LOW);

    // Envia dados para o Serial Monitor (para debug)
    Serial.print("Umidade Solo: "); Serial.print(umidSolo);
    Serial.print(" | Chuva (Seco>=1000): "); Serial.print(valChuva);
    Serial.print(" | Temp Ar: "); Serial.print(tempAr);
    Serial.print(" | Umid Ar: "); Serial.println(umidAr);

    // Verificação de erro do DHT
    if (isnan(umidAr) || isnan(tempAr)) {
      Serial.println("Falha ao ler o sensor DHT11!");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Erro no Sensor");
      lcd.setCursor(0, 1);
      lcd.print("de Ar! :(");
      delay(2000);
      return;
    }
  }

  // --- 2. Bloco de Lógica (Personalidade) ---

  // Lógica da Sede
  if (umidSolo > VALOR_SOLO_SECO) {
    if (!hortaEstaComSede) {
      hortaEstaComSede = true;
      tempoInicioSede = tempoAtual;
    }
  }
  else if (umidSolo < VALOR_SOLO_MOLHADO) {
    if (hortaEstaComSede) {
      mostrarMsgObrigado = true;
      tempoInicioRegada = tempoAtual;
    }
    hortaEstaComSede = false;
    tempoInicioSede = 0;
  }
  else {
    hortaEstaComSede = false;
    tempoInicioSede = 0;
  }

  // Lógica da Mensagem "Obrigado"
  if (mostrarMsgObrigado) {
    if (tempoAtual - tempoInicioRegada > TEMPO_MSG_OBRIGADO) {
      mostrarMsgObrigado = false;
    }
  }

  // --- 3. Bloco de Display (A "Voz" da Horta) ---

  lcd.clear();

  // PRIORIDADE 1: Mensagem "Obrigado"
  if (mostrarMsgObrigado) {
    lcd.setCursor(0, 0);
    lcd.print("Muito obrigada!");
    lcd.setCursor(0, 1);
    lcd.print(":D");
  }
  // PRIORIDADE 2: Mensagem "Estou com sede"
  else if (hortaEstaComSede) {
    if (tempoAtual - tempoInicioSede > TEMPO_PARA_TER_SEDE) {
      lcd.setCursor(0, 0);
      lcd.print("Estou com sede!!");
      lcd.setCursor(0, 1);
      lcd.print("Me regue... :(");
    }
    else {
      lcd.setCursor(0, 0);
      lcd.print("A terra esta");
      lcd.setCursor(0, 1);
      lcd.print("ficando seca...");
    }
  }
  // PRIORIDADE 3: Mensagem "Chovendo" (LÓGICA ATUALIZADA)
  else if (valChuva < VALOR_CHUVA_DETECTADA) { // Usa o novo valor de calibração
    lcd.setCursor(0, 0);
    lcd.print("Esta chovendo!");
    lcd.setCursor(0, 1);
    lcd.print("Oba! Agua! :D");
  }
  // PRIORIDADE 4: Alertas de Ar (Frio/Calor/Umidade)
  else if (tempAr < MIN_TEMP) {
    lcd.setCursor(0, 0);
    lcd.print("Brrr! Frio!");
    lcd.setCursor(0, 1);
    lcd.print("T: "); lcd.print(tempAr, 0); lcd.print("C");
  }
  else if (tempAr > MAX_TEMP) {
    lcd.setCursor(0, 0);
    lcd.print("Que calor! o_O");
    lcd.setCursor(0, 1);
    lcd.print("T: "); lcd.print(tempAr, 0); lcd.print("C");
  }
  else if (umidAr < MIN_UMID_AR) {
    lcd.setCursor(0, 0);
    lcd.print("O ar esta muito");
    lcd.setCursor(0, 1);
    lcd.print("seco! :S");
    // lcd.print("U.Ar: "); lcd.print(umidAr, 0); lcd.print("%");
  }
  // PRIORIDADE 5: Status "Tudo OK"
  else {
    lcd.setCursor(0, 0);
    lcd.print("Estou feliz! :)");
    lcd.setCursor(0, 1);
    lcd.print("T: "); lcd.print(tempAr, 0);
    lcd.print("C U.Ar: "); lcd.print(umidAr, 0); lcd.print("%");
  }
}

// =========================================================================
//                 FUNÇÕES DE LEITURA INTERMITENTE
// =========================================================================

/*
 * Lê o sensor de umidade do solo (PINO A2, POWER D10)
 */
int lerSensorUmidadeSolo() {
  digitalWrite(SOLO_POWER_PIN, HIGH); // Liga o sensor
  delay(100);                         // Espera estabilizar
  int valor = analogRead(SOLO_SENSOR_PIN); // Lê o valor
  digitalWrite(SOLO_POWER_PIN, LOW);  // Desliga o sensor
  return valor;
}

/*
 * Lê o sensor de chuva (PINO A3, POWER D8)
 * (Função ATUALIZADA para ler analogRead)
 */
int lerSensorChuva() {
  digitalWrite(CHUVA_POWER_PIN, HIGH); // Liga o sensor
  delay(100);                          // Espera estabilizar
  int valor = analogRead(CHUVA_SENSOR_PIN); // Lê o valor ANALÓGICO
  digitalWrite(CHUVA_POWER_PIN, LOW);  // Desliga o sensor
  return valor;
}