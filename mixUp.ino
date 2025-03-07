extern char tecla;
extern bool reverseFlag, obverse, solenoid,emergencyActed,running,topDoor,emergencyButton,emergencyRelay,faultInverter,
 InputPCF[];
extern unsigned long currentMilis;
extern int timeObverse,timeReverse,timeRegressive,timeTotal,timePaused,numCycles;

bool pushButtonOn = false,emergencyAcK = false,running = false, solenoidSafety = false,
 topDoor = false,emergencyButton = false,emergencyRelay = false,faultInverter = false;
int timeObverse = 3, timeReverse = 3, timePaused = 2, runningTime = 0,timeRegressive = 0,timeTotal = 0;
unsigned long bouncing = 0, timeOnSelenoid = 0, timeOffSelenoid = 0;
int bouncingDelay = 900;
bool lastButtonState = false;
int cycleCount = 0, numCycles = 3; 


// Variáveis para timing do mix
unsigned long tempoObverseOn = 0;
unsigned long tempoObverseOff = 0;
unsigned long tempoReverseOn = 0;
unsigned long tempoReverseOff = 0;
unsigned long totalCycleTime = 0;
unsigned long totalProcessTime = 0;
unsigned long elapsedTime = 0;
unsigned long tempoDecorrente = 0;

unsigned long lastStateChange = 0;
int mixState = 0;         // 0 = inicial, 1 = obverse on, 2 = obverse off, 3 = reverse on, 4 = reverse off
unsigned long startMixTime = 0;



void mixUp(){

 // porta superior
topDoor = !InputPCF[4];


 // botao emergencia
emergencyButton = !InputPCF[3];


 // relé emergencia
emergencyRelay = !InputPCF[2];


 // relé falha inversor
faultInverter = !InputPCF[1];


  bool currentButtonState = InputPCF[0];
  if (currentButtonState && !lastButtonState) { 
    static bool attTime = false;
    
    if (solenoidSafety) {
      running = !running;
            if (!running) {
          resetMixProcess();
      }else{
        attTime = false;
      }
    if(!attTime){
      timeOffSelenoid = currentMilis;
      attTime = true;
    }
    }
  }
  lastButtonState = currentButtonState;

if(topDoor || emergencyButton || emergencyRelay || faultInverter){
emergencyActed = true;
emergencyAcK = false;
resetMixProcess();
}else{
if(!emergencyAcK) {
timeOnSelenoid = currentMilis;
emergencyAcK = true;
}
emergencyActed = false;
}

defineTimes();
if(running && !emergencyActed){      // start mixing
runningMix();
}else{
  running = false;
  reverseFlag = false;
  obverse = false;
  solenoid = false;
  resetMixProcess();
}

if(!emergencyActed && emergencyAcK ){
if(currentMilis - timeOnSelenoid > 4000){
  solenoidSafety=true;
  solenoid = true;
  if(solenoid && running && currentMilis - timeOffSelenoid > 10000){
  solenoid = false;
  }
}
}else{
  solenoidSafety = false;
  solenoid = false; 
}




} // end void in loop

void runningMix() {

 elapsedTime = currentMilis - startMixTime;
 tempoDecorrente = (elapsedTime < totalProcessTime) ? (totalProcessTime - elapsedTime) : timeTotal;

  switch(mixState) {
    case 0: // Inicialização: Inicia o ciclo
      startMixTime = currentMilis;
      cycleCount = 0;
      obverse = true;
      reverseFlag = false;
      lastStateChange = currentMilis;
      mixState = 1;
      break;
      
    case 1: // obverse ligado por 5s
      if(currentMilis - lastStateChange >= tempoObverseOn) {
        obverse = false;
        lastStateChange = currentMilis;
        mixState = 2;
      }
      break;
      
    case 2: // obverse desligado por 2s
      if(currentMilis - lastStateChange >= tempoObverseOff) {
        reverseFlag = true;
        lastStateChange = currentMilis;
        mixState = 3;
      }
      break;
      
    case 3: // reverse ligado por 6s
      if(currentMilis - lastStateChange >= tempoReverseOn) {
        reverseFlag = false;
        lastStateChange = currentMilis;
        mixState = 4;
      }
      break;
      
    case 4: // reverse desligado por 2s
      if(currentMilis - lastStateChange >= tempoReverseOff) {
        cycleCount++; // Um ciclo completo terminou
        if(cycleCount < numCycles) {
          // Reinicia o ciclo: ativa obverse novamente
          obverse = true;
          lastStateChange = currentMilis;
          mixState = 1;
        } else {
          // Finaliza o processo: garante que ambos fiquem desligados
          obverse = false;
          reverseFlag = false;
          running = false;  // Opcional: encerra a mistura
          mixState = 0;     // Reseta o estado para permitir nova operação
        }
      }
      break;
      
    default:
      mixState = 0;
      break;
  }
}

// Função para reiniciar o processo de mixagem
void resetMixProcess() {
    mixState = 0;
    cycleCount = 0;
    startMixTime = 0;
    elapsedTime = 0;
    tempoDecorrente = totalProcessTime;
    obverse = false;
    reverseFlag = false;
}

void defineTimes(){
   tempoObverseOn  = (unsigned long)timeObverse * 1000UL;
   tempoObverseOff = (unsigned long)timePaused  * 1000UL;
   tempoReverseOn  = (unsigned long)timeReverse * 1000UL;
   tempoReverseOff = (unsigned long)timePaused  * 1000UL;

 totalCycleTime   = tempoObverseOn + tempoObverseOff + tempoReverseOn + tempoReverseOff; // 15000 ms
 totalProcessTime = totalCycleTime * numCycles; // 45000 ms

timeTotal = totalProcessTime / 1000;
timeRegressive = tempoDecorrente / 1000;
}

