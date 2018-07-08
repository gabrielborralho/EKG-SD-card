/** ##############################################################
**
**  Filename  : EKG-SD-card-save.ino
**  Project   : EKG-SD-card-save
**  Processor : ATMega328/P
**  Version   : 1.0
**  Compiler  : Arduino IDE
**  Date/Time : 22/09/2017
**  Aluno     : Gabriel Borralho
**  Last Mod  : 27/09/2017
**
** ##############################################################*/

/******************************************************************************
Hardware:

  # AD8232 Heart Monitor Version: 1.0 -> Hardware Platform: Arduino Pro 3.3V/8MHz
      >LO- - pin 5
      >LO+ - pin 6
      >OUT - A0
      
  # SD Card:
      > CS   - pin 10 
      > MOSI - pin 11
      > MISO - pin 12
      > CLK  - pin 13
******************************************************************************/
#include <SdFat.h>

SdFat sdCard;
SdFile meuArquivo;

const int chipSelect = 10;// Pino ligado ao CS do modulo

void setup() { 
  Serial.begin(9600);  // initialize the serial communication
  pinMode(5, INPUT); // Setup for leads off detection LO -
  pinMode(6, INPUT); // Setup for leads off detection LO +
  
  if(!sdCard.begin(chipSelect,SPI_HALF_SPEED))sdCard.initErrorHalt();// Inicializa o modulo SD  
  if (!meuArquivo.open("LER_ECG.txt", O_RDWR | O_CREAT | O_AT_END))// Abre o arquivo LER_ECG.TXT
  {
    sdCard.errorHalt("Erro na abertura do arquivo LER_ECG.TXT!");
  }
}

void loop() {    
  if((digitalRead(10) == 1)||(digitalRead(11) == 1)){
    Serial.println('!');
  }
  else{  
    Serial.println(analogRead(A0));// Exibe o valor lido na entrada analogica A0 do AD8232 no Serial Monitor
    meuArquivo.println(analogRead(A0));// Grava dados do ECG em LER_ECG.TXT    
  }
  if (analogRead(A0) < 1)
  {
    // Interrompe o processo e fecha o arquivo
    Serial.println("Processo de gravacao interrompido. Retire o SD!");
    meuArquivo.close();
    while (1) {}
  }
  
}

