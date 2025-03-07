
#include <Wire.h>
#include <EEPROM.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include "PCF8574.h"

#include "programas.h"
#define NUM_PROGRAMS 4
ProgramParameters programs[NUM_PROGRAMS];

const int markerAddress = 0;      
const byte markerValue = 0x55;   
const int dataStartAddress = 1;
const int SELECTED_PROGRAM_EEPROM_ADDR = 100;  // endereço 100 da EEPROM




PCF8574 pcf(0x3C); // endereço modulos saidas i2c
int  BitPcf = 0;
bool InputPCF[5] = {false};
extern bool InputPCF[];

PCF8574 pcf8574(0x39);// endereço modulo keypad i2c

#define pinoBuzzer 2 //led on board

const byte linhas = 4;
const byte colunas = 4;
// array que armazena as teclas (Mapeação de teclas)
char keys[linhas][colunas] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[linhas] = {0, 1, 2, 3}; 
byte colPins[colunas] = {4, 5, 6, 7};


LiquidCrystal_I2C lcd(0x3F, 20, 4);

bool reverseFlag  = false, obverse = false, solenoid = false,emergencyActed = true;
extern bool reverseFlag, obverse, solenoid,emergencyActed;
int page = 0, passwordRequestedPage = 0;
extern int page,passwordRequestedPage,selectedProgram;
unsigned long currentMilis = 0, lastTimeOneSec = 0,lastTimePcf = 0;
extern unsigned long currentMilis;
char tecla;
extern char tecla;

void setup() {
  Wire.begin();
  lcd.begin(); 
  lcd.clear(); 
  lcd.backlight(); 
  Serial.begin(115200);
  pcf.begin();
  pcf8574.begin();

pcf.write(0, true);
pcf.write(1, true);
pcf.write(2, true);

  EEPROM.begin(512);
  EEPROM_Reading();
  selectedProgram = loadSelectedProgram();
  loadProgram();
  lcd.setCursor(0, 0);
  lcd.print("Misturador de carnes");
  lcd.setCursor(0, 1);
  lcd.print("vers. 1.1.0         ");
  lcd.setCursor(0, 3);
  lcd.print("developed by Talis  ");  
  pinMode(pinoBuzzer, OUTPUT);
  digitalWrite(pinoBuzzer, HIGH);
  

  delay(2100);
  lcd.clear(); // Limpa a tela LCD
}

void loop() {
  currentMilis = millis();
  tecla = getKey(); 

  if(currentMilis - lastTimePcf > 80){ // atualiza o pcdf a cada 0.80s
    lastTimePcf = currentMilis;
  InputsBitPCF();
  }

 
  actionsPage(page);
  mixUp();
  if(currentMilis - lastTimeOneSec > 1000){ // atualiza o display a cada 1s
    lastTimeOneSec = currentMilis;
  displayEverySecond();
  }


  if (tecla) {
    feedbackSomClique(); 
    Serial.print("Press Key: ");
    Serial.println(tecla);

    if (tecla == 'J') { 
      if (efetuaLogin())
      {
        lcd.setCursor(0, 0);
        lcd.print("Voce esta logado");
        lcd.setCursor(0, 1);
        lcd.print("                ");
        delay(1500);
        lcd.clear();
      }
    }
  }
}

bool efetuaLogin() {


  String senhaInserida; 
  String senhaEscondidaExibicao; 
  int numeroTentativas = 0; 
  int numeroMaximoTentativas = 3; 

  while (true) { // loop para inserção de senha
    lcd.blink(); // faz o cursor do display ficar piscando
    char tecla = getKey(); // armazena em uma variável o retorno da tecla obtida
    if (tecla) { // se alguma tecla foi obtida, ...
      feedbackSomClique(); 
 
      if (tecla == '#') { // se a tecla pressionada foi a # (ENTER), ...
        if (senhaInserida == "1234") { // compara se a senha inserida pelo usuário é igual a senha para fazer login
          lcd.noBlink(); // faz o cursor do dislpay LCD parar de piscar
      // efetua o som de feedback de login bem sucedido
          return true; // retorna true para a chamada da função
        } else { // se a senhas não forem iguais, ...
          lcd.noBlink(); // faz o cursor do dislpay LCD parar de piscar
          numeroTentativas++; // incrementa a váriavel de número de tentativas do usuário
          senhaInserida = ""; // "zera" a váriavel senhaInserida
          senhaEscondidaExibicao = ""; // "zera" a váriavel senhaEscondidaExibicao
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Senha Incorreta ");
          lcd.setCursor(0, 1);
          lcd.print("Tentativa ");
          lcd.print(numeroTentativas);
          lcd.print(" de ");
          lcd.print(numeroMaximoTentativas);
 
          lcd.clear();
          if (numeroTentativas >= numeroMaximoTentativas) { // testa se o número de tentativas for igual ou superior ao número máximo de tentativas
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Tentat. Zeradas ");
            lcd.setCursor(0, 1);
            lcd.print("Entre novamente ");

            lcd.clear();
            return false; // caso o usuário chegue ou ultrapasse o número máximo de tentativas, retorna false para a chamada da função
          }
        }
      } else if (tecla == '*') { // se não, se a tecla pressionada foi a * (APAGAR), ...
        // remove o último caractere da váriavel senhaInserida. O caractere a ser removido é o de número que corresponda ao número total de caracteres da váriavel subtraido 1.
        senhaInserida.remove(senhaInserida.length() - 1);
        senhaEscondidaExibicao.remove(senhaEscondidaExibicao.length() - 1); // também remove o último caractere da váriavel senhaEscondidaExibicao
        lcd.clear();
        lcd.print(senhaEscondidaExibicao);
      } else if (tecla == 'B') { // se a tecla pressionada foi a B (SAIR), ...
        return false; // retorna false para a chamada da função
      } else if (!isAlpha(tecla)) { // se a tecla pressionada não for nenhuma letra, ...
        senhaInserida += tecla; // adiciona a tecla pressionada para a váriavel senhaInserida
        senhaEscondidaExibicao += '*'; // adiciona o caractere '*' para a váriavel senhaEscondidaExibicao
        lcd.clear(); // limpa o display
        lcd.print(senhaEscondidaExibicao); // mostra a váriavel senhaEscondidaExibicao
      }
    }
  }
}

// função de toque do buzzer para feedback do aperto da tecla
void feedbackSomClique() {
  digitalWrite(pinoBuzzer, LOW);
  delay(50);
  digitalWrite(pinoBuzzer, HIGH);
}



void InputsBitPCF(){
  BitPcf = pcf.read8();
  InputPCF[0] = bitRead(BitPcf, 3) == 0;
  InputPCF[1] = bitRead(BitPcf, 4) == 0;
  InputPCF[2] = bitRead(BitPcf, 5) == 0;
  InputPCF[3] = bitRead(BitPcf, 6) == 0;
  InputPCF[4] = bitRead(BitPcf, 7) == 0;


 pcf.write(0, !reverseFlag);
 pcf.write(1, !obverse);
 pcf.write(2, !solenoid);

}


// Função customizada para ler a tecla pressionada
char getKey() {
  const unsigned long debounceDelay = 50; // tempo mínimo para considerar estável
  static char lastRawKey = '\0';        // última leitura bruta da tecla
  static char stableKey = '\0';         // última tecla estável (debounced)
  static char lastReturnedKey = '\0';   // última tecla retornada (para evitar repetições)
  static unsigned long lastDebounceTime = 0; // momento da última mudança

  char rawKey = '\0';

  // Varre as linhas do teclado
  for (byte l = 0; l < linhas; l++) {
    // Coloca todas as linhas em HIGH
    for (byte i = 0; i < linhas; i++) {
      pcf8574.write(rowPins[i], HIGH);
    }
    // Coloca a linha atual em LOW
    pcf8574.write(rowPins[l], LOW);

    // Varre as colunas
    for (byte c = 0; c < colunas; c++) {
      if (pcf8574.read(colPins[c]) == LOW) {
        rawKey = keys[l][c];
        break;
      }
    }
    if (rawKey != '\0') break;
  }

  // Se a leitura bruta mudou, reinicia o timer de debounce
  if (rawKey != lastRawKey) {
    lastDebounceTime = millis();
  }

  // Se o sinal se manteve estável por debounceDelay, atualiza a tecla estável
  if (millis() - lastDebounceTime > debounceDelay) {
    stableKey = rawKey;
  }
  lastRawKey = rawKey;

  // Retorna a tecla somente na transição de não pressionada para pressionada (rising edge)
  if (stableKey != '\0' && stableKey != lastReturnedKey) {
    lastReturnedKey = stableKey;
    return stableKey;
  }
  // Se a tecla for liberada, reseta a última tecla retornada
  if (stableKey == '\0') {
    lastReturnedKey = '\0';
  }
  
  return '\0';
}


void EEPROM_Reading(){


 if (EEPROM.read(markerAddress) != markerValue) {
    Serial.println("EEPROM não inicializada. Gravando valores padrão...");
    
    // Define os programas padrão (você pode alterar esses valores conforme necessário)
    programs[0] = {5, 6, 2, 3};  // Programa 1: 5s obverse, 6s reverse, 2s pausa, 3 ciclos
    programs[1] = {4, 7, 2, 3};  // Programa 2
    programs[2] = {6, 5, 2, 3};  // Programa 3
    programs[3] = {5, 5, 3, 2};  // Programa 4
    
    // Escreve o marcador para indicar que a EEPROM foi inicializada
    EEPROM.write(markerAddress, markerValue);
    
    // Armazena cada programa na EEPROM
    for (int i = 0; i < NUM_PROGRAMS; i++) {
      int addr = dataStartAddress + i * sizeof(ProgramParameters);
      EEPROM.put(addr, programs[i]);
    }
    EEPROM.commit();
  }
  else {
    Serial.println("EEPROM já inicializada. Lendo valores...");
    // Lê os programas da EEPROM
    for (int i = 0; i < NUM_PROGRAMS; i++) {
      int addr = dataStartAddress + i * sizeof(ProgramParameters);
      EEPROM.get(addr, programs[i]);
    }
  }
  
  // Exibe os valores lidos (para depuração)
  for (int i = 0; i < NUM_PROGRAMS; i++) {
    Serial.print("Programa ");
    Serial.print(i + 1);
    Serial.print(": Obverse=");
    Serial.print(programs[i].timeObverse);
    Serial.print("s, Reverse=");
    Serial.print(programs[i].timeReverse);
    Serial.print("s, Pausa=");
    Serial.print(programs[i].timePaused);
    Serial.print("s, Ciclos=");
    Serial.println(programs[i].cycles);
  }

}


// Função para atualizar um programa na EEPROM
// programNumber: número do programa (1 a NUM_PROGRAMS)
// newTimeObverse, newTimeReverse, newTimePaused: tempos em segundos
// newCycles: número de ciclos
void updateProgramEEPROM(int programNumber, uint16_t newTimeObverse, uint16_t newTimeReverse, uint16_t newTimePaused, uint8_t newCycles) {
  // Verifica se o número do programa é válido
  if(programNumber < 1 || programNumber > NUM_PROGRAMS) {
    Serial.println("Número do programa inválido.");
    return;
  }
  
  // Converte para índice (0 a NUM_PROGRAMS-1)
  int index = programNumber - 1;
  
  // Atualiza a estrutura do programa em memória
  programs[index].timeObverse = newTimeObverse;
  programs[index].timeReverse = newTimeReverse;
  programs[index].timePaused  = newTimePaused;
  programs[index].cycles      = newCycles;
  
  // Calcula o endereço na EEPROM para este programa
  int addr = dataStartAddress + index * sizeof(ProgramParameters);
  
  // Grava a estrutura atualizada na EEPROM
  EEPROM.put(addr, programs[index]);
  EEPROM.commit();
  
  Serial.print("Programa ");
  Serial.print(programNumber);
  Serial.println(" atualizado com sucesso.");
}

void saveSelectedProgram(int prog) {
  EEPROM.put(SELECTED_PROGRAM_EEPROM_ADDR, prog);
  EEPROM.commit();
}


int loadSelectedProgram() {
  int prog;
  EEPROM.get(SELECTED_PROGRAM_EEPROM_ADDR, prog);
  if(prog < 1 || prog > NUM_PROGRAMS) {
    prog = 1;
  }
  return prog;
}