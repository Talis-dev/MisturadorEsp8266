#include "programas.h"

extern char tecla;
extern bool reverseFlag, obverse, solenoid,emergencyActed,emergencyAcK,running,topDoor,emergencyButton,emergencyRelay,faultInverter;
extern bool InputPCF[];
extern int page,passwordRequestedPage,timeObverse,timeReverse,timeRegressive,timeTotal,timePaused,numCycles,selectedProgram;
extern unsigned long currentMilis;

int selectedProgram = 1,timeTotalDispaly = 0;
int positionCursor = 0;
String valOnCursor[6];


bool isPasswordInvalid = false,isSuccessful = false;
unsigned long timeInvalidMesage = 0, lastTimeMesage = 0;


  String senhaPadrao = "41";
  String senhaInserida = "-1"; 
  String senhaEscondidaExibicao; 
  int numeroTentativas = 0; 

void displayEverySecond(){

switch (page){
  case 0:
  homePage();
  break;
  case 1:
  passwordPage();
  break;
  case 2:
  configPage();
  break;
  case 3:
  progPage();
  break;

}
}




void homePage(){
  lcd.setCursor(0, 0);
  lcd.print("MISTURADOR DE CARNES");

if(!emergencyActed){
lcd.setCursor(0, 1);
if(solenoid){
if (running){
lcd.print("RUN. Prog:  TT:     ");
}else {
lcd.print("RDY. Prog:  TT:     ");
}  
}else{
lcd.print("LOCK Prog:  TT:     ");  
}


lcd.setCursor(10, 1);
lcd.print(selectedProgram);
lcd.setCursor(15, 1);
lcd.print(timeTotal);
lcd.setCursor(0, 2);
lcd.print("TO:   TR:   TC:     ");
lcd.setCursor(3, 2);
lcd.print(timeObverse);
lcd.setCursor(9, 2);
lcd.print(timeReverse);
lcd.setCursor(15, 2);
lcd.print(timeRegressive);

  }else{

  lcd.setCursor(0, 1);  
  lcd.print("---x BLOQUEADO! x---");

  if(topDoor){
  lcd.setCursor(0, 2);   
  lcd.print("*PORTA SUPERIOR OPEN");
  } else if(emergencyButton){
  lcd.setCursor(0, 2);  
  lcd.print("*EMERGENCIA ATUADA! ");  
  } else if(emergencyRelay){
  lcd.setCursor(0, 2);   
  lcd.print("*PRESCIONE RESET*   ");  
  } else if(faultInverter){
  lcd.setCursor(0, 2);   
  lcd.print("*INVERSOR EM FALHA! ");  
  }
  }

  lcd.setCursor(0, 3);
  lcd.print("D(Config)    C(Prog)");

}



void passwordPage(){
  if(senhaInserida == "-1"){
  lcd.setCursor(0, 0);
  lcd.print("Insira a senha:     ");
  lcd.setCursor(0, 1);
  lcd.print("                    ");
  lcd.setCursor(0, 2);
  lcd.print("*(Delete)            ");
  lcd.setCursor(0, 3);
  lcd.print("D(Sair)     #(Enter)");
  senhaInserida = "";
  }
  
if(isPasswordInvalid){
  invalidPassword();
  if(currentMilis - lastTimeMesage > 2000 ){
    senhaInserida = "-1";
    senhaEscondidaExibicao = "";
    isPasswordInvalid = false;
  }
}

 
}


void configPage(){
if(isSuccessful){
  successMessage();
  if(currentMilis - lastTimeMesage > 2000 ){ 
    isSuccessful = false;
positionCursor = 0;
  }
}else{
lcd.setCursor(0, 0);
lcd.print("Editar Prog:  TT:   ");

lcd.setCursor(12, 0);
lcd.print(valOnCursor[1]); 

lcd.setCursor(17, 0);
lcd.print(timeTotalDispaly);

lcd.setCursor(0, 1);
lcd.print("TO:    TR:    CL:   ");

lcd.setCursor(3, 1);
lcd.print(valOnCursor[2]);
lcd.setCursor(10, 1);
lcd.print(valOnCursor[3]);
lcd.setCursor(17, 1);
lcd.print(valOnCursor[4]);

  lcd.setCursor(0, 2);
  lcd.print("D(Sair)     C(Delet)");
  lcd.setCursor(0, 3);
  lcd.print("*(Selec)   #(Salvar)"); 
}
switch (positionCursor) {
case 1:
lcd.setCursor(12, 0);
lcd.cursor();
break;
case 2:
lcd.setCursor(3, 1);
lcd.cursor();
break;
case 3:
lcd.setCursor(10, 1);
lcd.cursor();
break;
case 4:
lcd.setCursor(17, 1);
lcd.cursor();
break;

}

}


void progPage(){
if(isSuccessful){
  successMessage();
  if(currentMilis - lastTimeMesage > 2000 ){ 
    isSuccessful = false;
    lcdClear();
    page=0;
  }
}else{

  lcd.setCursor(0, 0);
  lcd.print("---Programa Ativo---");
  lcd.setCursor(0, 1);
  lcd.print("( )prog 1 ( )prog 2 ");
  lcd.setCursor(0, 2);
  lcd.print("( )prog 3 ( )prog 4 ");
  lcd.setCursor(0, 3);
  lcd.print("*(Seleciona) #(Sair)"); 

switch (selectedProgram) {
case 1:
lcd.setCursor(1, 1);
lcd.print("X");
break;
case 2:
lcd.setCursor(11, 1);
lcd.print("X");
break;
case 3:
lcd.setCursor(1, 2);
lcd.print("X");
break;
case 4:
lcd.setCursor(11, 2);
lcd.print("X");
break;
}

}
}


void actionsPage(int numPage){

switch (numPage){
case 0:

if (tecla == 'D') {
  passwordRequestedPage = 1;
  page = 1;
  Serial.println("tecla D");
}

if (tecla == 'C') {
  passwordRequestedPage = 2;
  page = 1;
  Serial.println("tecla C");
}

break;

case 1:

  if(tecla){
if (!isAlpha(tecla) && tecla != '#' && tecla != '*') { // se a tecla pressionada não for nenhuma letra, ...
        senhaInserida += tecla; // adiciona a tecla pressionada para a váriavel senhaInserida
        senhaEscondidaExibicao += '*'; // adiciona o caractere '*' para a váriavel senhaEscondidaExibicao
        Serial.println(senhaInserida);
          lcd.setCursor(0, 1);
          lcd.print("                    ");
          lcd.setCursor(0, 1);
          lcd.print(senhaEscondidaExibicao);
          int posCursor = senhaEscondidaExibicao.length();
  lcd.setCursor(posCursor, 1);
  lcd.blink();
}
  }
if (tecla == '*') { 
        senhaInserida.remove(senhaInserida.length() - 1);
        senhaEscondidaExibicao.remove(senhaEscondidaExibicao.length() - 1); 
        Serial.println(senhaInserida);
          lcd.setCursor(0, 1);
          lcd.print("                    ");
          lcd.setCursor(0, 1);
          lcd.print(senhaEscondidaExibicao);       
          int posCursor = senhaEscondidaExibicao.length();
  lcd.setCursor(posCursor, 1);
  lcd.blink();
     }

if (tecla == '#') {
  lcd.noBlink();
  Serial.println("tecla #");  
 if(senhaInserida == senhaPadrao){
 senhaEscondidaExibicao = "";
 senhaInserida = "-1";
 Serial.println("sucesso");

 if(passwordRequestedPage == 1){
  page=2;
 }else if(passwordRequestedPage == 2){
  page=3;
 }

 }else{
  lastTimeMesage = currentMilis;
  isPasswordInvalid = true;
 }

}

if (tecla == 'D') {
  lcd.noBlink();
  senhaInserida = "-1";
  senhaEscondidaExibicao = "";
  page = 0;
}

break;

case 2:

if(tecla == '*'){
positionCursor ++;
if (positionCursor >= 5) {
positionCursor = 1;
}
}

if (tecla && !isAlpha(tecla) && tecla != '#' && tecla != '*') {
    valOnCursor[positionCursor] += tecla;
    
    if (positionCursor == 1) {
      int progID = valOnCursor[1].toInt();

      if (progID < 1 || progID > 4) {
        valOnCursor[1] = "";
      } else {
        selectedProgram = progID;
        loadProgram();  
        valOnCursor[2] = String(timeObverse);
        valOnCursor[3] = String(timeReverse);
        valOnCursor[4] = String(numCycles);
        // Recalcula os tempos totais
        RecalculaTotal();
      }
    }
    else {
      if (valOnCursor[positionCursor].length() > 3) {
        valOnCursor[positionCursor] = "";
      }
    }
    RecalculaTotal();
    Serial.println(valOnCursor[positionCursor]);
  }


if(tecla == 'D'){
 positionCursor = 0; 
lcd.noCursor();
page = 0;
}

if(tecla == 'C'){
 valOnCursor[positionCursor] = "";
}

if(tecla == '#'){ //SALVAR alteraão do program
saveProgramChanges();
positionCursor = 0;
lcd.noCursor(); 
  lastTimeMesage = currentMilis;
  isSuccessful = true;
}


break;

case 3:


if(tecla == '#'){
  loadProgram();
  saveSelectedProgram(selectedProgram);
  lastTimeMesage = currentMilis;
  isSuccessful = true;
}
if(tecla == '*'){
selectedProgram ++;
if (selectedProgram >= 5) {
selectedProgram = 1;
}
defineTimes();
}
break;


}// end switch
}//end void



void invalidPassword(){
lcd.setCursor(0, 0);
lcd.print("--------------------");
lcd.setCursor(0, 1);
lcd.print("  SENHA INVALIDA!   ");
lcd.setCursor(0, 2);
lcd.print("--------------------");
lcd.setCursor(0, 3);
lcd.print("                    ");
}


void successMessage(){
lcd.setCursor(0, 0);
lcd.print("--------------------");
lcd.setCursor(0, 1);
lcd.print(" FEITO COM SUCESSO! ");
lcd.setCursor(0, 2);
lcd.print("--------------------");
lcd.setCursor(0, 3);
lcd.print("                    ");
}

void lcdClear(){
lcd.setCursor(0, 0);
lcd.print("                    ");
lcd.setCursor(0, 1);
lcd.print("                    ");
lcd.setCursor(0, 2);
lcd.print("                    ");
lcd.setCursor(0, 3);
lcd.print("                    ");
}

void loadProgram(){
  timeObverse = programs[selectedProgram-1].timeObverse;
  timeReverse = programs[selectedProgram-1].timeReverse;
  timePaused  = programs[selectedProgram-1].timePaused;
  numCycles   = programs[selectedProgram-1].cycles;
}


void saveProgramChanges() {
  // Converte o valor digitado para o ID do programa
  int progID = valOnCursor[1].toInt();
  if (progID < 1 || progID > 4) {
    Serial.println("Programa inválido para salvar.");
    return;
  }
  
  // Converte os outros valores digitados
  uint16_t newTimeObverse = valOnCursor[2].toInt();
  uint16_t newTimeReverse = valOnCursor[3].toInt();
  uint16_t newTimePaused  = timePaused;  
  uint8_t newCycles       = valOnCursor[4].toInt();
  
  // Atualiza o programa no array e grava na EEPROM
  updateProgramEEPROM(progID, newTimeObverse, newTimeReverse, newTimePaused, newCycles);
  
  defineTimes();
  
  Serial.print("Programa ");
  Serial.print(progID);
  Serial.println(" salvo com sucesso.");
}

void RecalculaTotal(){

  uint16_t newTimeObverse = valOnCursor[2].toInt();
  uint16_t newTimeReverse = valOnCursor[3].toInt();
  uint16_t newTimePaused  = timePaused;  
  uint8_t newCycles       = valOnCursor[4].toInt();

 unsigned long totalCycle   = newTimeObverse + timePaused + newTimeReverse + timePaused; 
 unsigned long totalProcess = totalCycle * newCycles;
timeTotalDispaly = totalProcess;
}
