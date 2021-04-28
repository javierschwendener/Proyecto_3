// Código original tomado de:
// https://henryforceblog.wordpress.com/2015/05/02/blink-example-using-timer-on-a-tiva-launchpad/
// Modificado/comentado por Luis Alberto Rivera

#include "noteCode.h"
#include "SD.h"

#include "wiring_private.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"

const byte interruptPin2 = 17; //Boton 2
const byte interruptPin1 = 31; //Boton 1
const byte interruptPin3 = PB_3; //Boton 3
volatile byte S1 = false;
volatile byte S2 = false;
volatile byte S3 = false;
volatile byte S4 = true;

int pre;
//Terra's Theme
int m1[] = {
    NOTE_DS4,NOTE_GS4,NOTE_B4,NOTE_DS4,NOTE_GS4,NOTE_B4,NOTE_GS4,NOTE_DS4,NOTE_GS4,NOTE_B4,
    NOTE_DS4,NOTE_GS4,NOTE_B4,NOTE_GS4,NOTE_DS4,NOTE_GS4,NOTE_B4,
    NOTE_DS4,NOTE_GS4,NOTE_B4,NOTE_GS4,NOTE_DS4,NOTE_GS4,NOTE_B4,NOTE_DS4,NOTE_GS4,
    NOTE_GS5,NOTE_AS5,NOTE_B5,NOTE_DS6,NOTE_B5,NOTE_AS5,NOTE_GS5,NOTE_AS5,NOTE_DS5,
    NOTE_GS5,NOTE_AS5,NOTE_B5,NOTE_DS6,NOTE_B5,NOTE_AS5,NOTE_GS5,NOTE_AS5,NOTE_DS6,
    NOTE_B5,NOTE_CS6,NOTE_DS6,NOTE_FS6,NOTE_DS6,NOTE_CS6,NOTE_B5,NOTE_CS6,NOTE_FS5,
    NOTE_B5,NOTE_AS5,NOTE_GS5,1,1,NOTE_B5,NOTE_AS5,NOTE_GS5,1,1
};
int d1[] = {
    8,8,4,8,8,8,8,8,8,4,
    8,8,8,8,8,8,4,
    8,8,8,8,8,8,4,8,8,
    8,8,8,8,1,8,8,2,1,
    8,8,8,8,1,8,8,2,1,
    8,8,8,8,1,8,8,2,1,
    8,8,1,2,8,8,8,1,2,8
};

//Battle Theme
int m2[] = {
    NOTE_D3,NOTE_D3,NOTE_D3,NOTE_D3,NOTE_D3,NOTE_D3,NOTE_C3,NOTE_C3,
    NOTE_D3,NOTE_D3,NOTE_D3,NOTE_D3,NOTE_D3,NOTE_D3,NOTE_C3,NOTE_C3,
    NOTE_D5,NOTE_E5,NOTE_F5,NOTE_A5,NOTE_C6,NOTE_AS5,NOTE_G5,NOTE_AS5,NOTE_A5,NOTE_D5,
    NOTE_G5,NOTE_F5,NOTE_DS5,NOTE_D5,NOTE_A4,NOTE_A4,NOTE_AS4,NOTE_C5,NOTE_D6,
    NOTE_C6,NOTE_AS5,NOTE_A5,NOTE_G5,NOTE_F5,NOTE_DS5,NOTE_D5,NOTE_C5,
    NOTE_D5,NOTE_E5,NOTE_F5,NOTE_A5,NOTE_C6,NOTE_AS5,NOTE_G5,NOTE_AS5,NOTE_A5,NOTE_D5,
    NOTE_G5,NOTE_F5,NOTE_DS5,NOTE_D5,NOTE_A4,NOTE_AS4,NOTE_C5,
    NOTE_D5,NOTE_D5,NOTE_F5,NOTE_F5,NOTE_AS5,NOTE_AS5,NOTE_DS6,NOTE_DS6,
    NOTE_D5,NOTE_D5,1,NOTE_DS5,NOTE_FS5,NOTE_FS5,1,
    NOTE_DS5,NOTE_D5,NOTE_DS5,NOTE_D5,NOTE_A4,NOTE_AS4,NOTE_DS5,
    NOTE_D5,NOTE_D5,1,NOTE_DS5,NOTE_FS5,NOTE_FS5,1,
    NOTE_G5,NOTE_F5,NOTE_DS5,NOTE_D5,NOTE_A4,NOTE_AS4,NOTE_DS5,NOTE_FS5,NOTE_AS5,
    NOTE_D5,NOTE_D5,1,NOTE_DS5,NOTE_FS5,NOTE_FS5,1,
    NOTE_DS5,NOTE_D5,NOTE_DS5,NOTE_D5,NOTE_A4,NOTE_AS4,NOTE_DS5,
    NOTE_D5,NOTE_D5,1,NOTE_DS5,NOTE_FS5,NOTE_FS5,1,
    NOTE_G5,NOTE_F5,NOTE_DS5,NOTE_D5,NOTE_A4,NOTE_AS4,NOTE_DS5,NOTE_FS5,NOTE_AS5,
    NOTE_D5,NOTE_E5,NOTE_F5,NOTE_A5,NOTE_C6,NOTE_AS5,NOTE_G5,NOTE_AS5,NOTE_A5,NOTE_D5,
    NOTE_G5,NOTE_F5,NOTE_DS5,NOTE_D5,NOTE_A4,NOTE_A4,NOTE_AS4,NOTE_C5,NOTE_D6,
    NOTE_C6,NOTE_AS5,NOTE_A5,NOTE_G5,NOTE_F5,NOTE_DS5,NOTE_D5,NOTE_C5,
    NOTE_D5,NOTE_E5,NOTE_F5,NOTE_A5,NOTE_C6,NOTE_AS5,NOTE_G5,NOTE_AS5,NOTE_A5,NOTE_D5,
    NOTE_G5,NOTE_F5,NOTE_DS5,NOTE_D5,NOTE_A4,NOTE_AS4,NOTE_C5,
    NOTE_D5,NOTE_D5,NOTE_F5,NOTE_F5,NOTE_AS5,NOTE_AS5,NOTE_DS6,NOTE_DS6,
    NOTE_DS5,NOTE_D5,NOTE_CS5,NOTE_D5,
    NOTE_A5,NOTE_A5,NOTE_A5,NOTE_A5,NOTE_A5,NOTE_A5,NOTE_A5,NOTE_A5,
    NOTE_DS5,NOTE_D5,NOTE_CS5,NOTE_D5,
    NOTE_DS5,NOTE_D5,NOTE_F5,NOTE_DS5,NOTE_D5,NOTE_A4,
    NOTE_F5,NOTE_DS5,NOTE_G5,NOTE_F5,NOTE_DS5,NOTE_AS4,
    NOTE_G5,NOTE_F5,NOTE_A5,NOTE_G5,NOTE_AS5,NOTE_A5,NOTE_C6,NOTE_AS5,
    NOTE_D6,1,NOTE_D6,1,NOTE_D6,1,NOTE_D6,1,
    NOTE_F6,NOTE_DS6,NOTE_D6,1,NOTE_D6,1,NOTE_CS6
};
int d2[] = {
    8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,4,3,
    8,8,8,4,3,8,8,8,8,
    16,16,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,4,3,
    4,8,8,2,8,8,8,
    8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,
    8,4,8,4,8,8,8,
    8,8,8,8,8,8,8,
    8,8,16,16,8,4,8,8,8,
    8,8,8,8,8,8,8,
    8,4,8,4,8,8,8,
    8,8,8,8,8,8,8,
    8,8,16,16,8,4,8,8,8,
    8,8,8,8,8,8,8,8,4,3,
    8,8,8,4,3,8,8,8,8,
    16,16,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,4,3,
    4,8,8,2,8,8,8,
    8,8,8,8,8,8,8,8,
    2,1,2,1,
    4,4,4,4,4,4,4,4,
    2,1,2,1,
    8,8,8,8,8,4,
    4,8,8,8,5,4,
    8,8,8,8,8,8,8,8,
    4,4,4,4,4,4,4,4,
    3,3,4,4,4,4,1
};

//Prelude
int m3[] = {
    NOTE_C3,NOTE_D3,NOTE_E3,NOTE_G3,NOTE_C4,NOTE_D4,NOTE_E4,NOTE_G4,NOTE_C5,NOTE_D5,NOTE_E5,NOTE_G5,NOTE_C6,NOTE_D6,NOTE_E6,NOTE_G6,NOTE_C7,
    NOTE_G6,NOTE_E6,NOTE_D6,NOTE_C6,NOTE_G5,NOTE_E5,NOTE_D5,NOTE_C5,NOTE_G4,NOTE_E4,NOTE_D4,NOTE_C4,NOTE_G3,NOTE_E3,NOTE_D3,
    NOTE_A2,NOTE_B2,NOTE_C3,NOTE_E3,NOTE_A3,NOTE_B3,NOTE_C4,NOTE_E4,NOTE_A4,NOTE_B4,NOTE_C5,NOTE_E5,NOTE_A5,NOTE_B5,NOTE_C6,NOTE_E6,NOTE_A6,
    NOTE_E6,NOTE_C6,NOTE_B5,NOTE_A5,NOTE_E5,NOTE_C5,NOTE_B4,NOTE_A4,NOTE_E4,NOTE_C4,NOTE_B3,NOTE_A3,NOTE_E3,NOTE_C3,NOTE_B2,
    NOTE_C3,NOTE_D3,NOTE_E3,NOTE_G3,NOTE_C4,NOTE_D4,NOTE_E4,NOTE_G4,NOTE_C5,NOTE_D5,NOTE_E5,NOTE_G5,NOTE_C6,NOTE_D6,NOTE_E6,NOTE_G6,NOTE_C7,
    NOTE_G6,NOTE_E6,NOTE_D6,NOTE_C6,NOTE_G5,NOTE_E5,NOTE_D5,NOTE_C5,NOTE_G4,NOTE_E4,NOTE_D4,NOTE_C4,NOTE_G3,NOTE_E3,NOTE_D3,
    NOTE_A2,NOTE_B2,NOTE_C3,NOTE_E3,NOTE_A3,NOTE_B3,NOTE_C4,NOTE_E4,NOTE_A4,NOTE_B4,NOTE_C5,NOTE_E5,NOTE_A5,NOTE_B5,NOTE_C6,NOTE_E6,NOTE_A6,
    NOTE_E6,NOTE_C6,NOTE_B5,NOTE_A5,NOTE_E5,NOTE_C5,NOTE_B4,NOTE_A4,NOTE_E4,NOTE_C4,NOTE_B3,NOTE_A3,NOTE_E3,NOTE_C3,NOTE_B2,
    NOTE_A2,NOTE_C3,NOTE_F3,NOTE_G3,NOTE_A3,NOTE_C4,NOTE_F4,NOTE_G4,NOTE_A4,NOTE_C5,NOTE_F5,NOTE_G5,NOTE_A5,NOTE_C6,NOTE_F6,NOTE_G6,NOTE_A6,
    NOTE_G6,NOTE_F6,NOTE_C6,NOTE_A5,NOTE_G5,NOTE_F5,NOTE_C5,NOTE_A4,NOTE_G4,NOTE_F4,NOTE_C4,NOTE_A3,NOTE_G3,NOTE_F3,NOTE_C3,
    NOTE_B2,NOTE_D3,NOTE_G3,NOTE_A3,NOTE_B3,NOTE_D4,NOTE_G4,NOTE_A4,NOTE_B4,NOTE_D5,NOTE_G5,NOTE_A5,NOTE_B5,NOTE_D6,NOTE_G6,NOTE_A6,NOTE_B6,
    NOTE_A6,NOTE_G6,NOTE_D6,NOTE_B5,NOTE_A5,NOTE_G5,NOTE_D5,NOTE_B4,NOTE_A4,NOTE_G4,NOTE_D4,NOTE_B3,NOTE_A3,NOTE_G3,NOTE_D3,
    NOTE_GS2,NOTE_C3,NOTE_DS3,NOTE_G3,NOTE_GS3,NOTE_C4,NOTE_DS4,NOTE_G4,NOTE_GS4,NOTE_C5,NOTE_DS5,NOTE_G5,NOTE_GS5,NOTE_C6,NOTE_DS6,NOTE_G6,NOTE_GS6,
    NOTE_G6,NOTE_DS6,NOTE_C6,NOTE_GS5,NOTE_G5,NOTE_DS5,NOTE_C5,NOTE_GS4,NOTE_G4,NOTE_DS4,NOTE_C4,NOTE_GS3,NOTE_G3,NOTE_DS3,NOTE_C3,NOTE_AS2,
    NOTE_D3,NOTE_F3,NOTE_A3,NOTE_AS3,NOTE_D4,NOTE_F4,NOTE_A4,NOTE_AS4,NOTE_D5,NOTE_F5,NOTE_A5,NOTE_AS5,NOTE_D6,NOTE_F6,NOTE_A6,NOTE_AS6,
    NOTE_A6,NOTE_F6,NOTE_D6,NOTE_AS5,NOTE_A5,NOTE_F5,NOTE_D5,NOTE_AS4,NOTE_A4,NOTE_F4,NOTE_D4,NOTE_AS3,NOTE_A3,NOTE_F3,NOTE_D3
};
int d3[] = {
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8
};

//Victory Fanfare
int m4[] = {
    NOTE_C5,NOTE_C5,NOTE_C5,NOTE_C5,NOTE_GS4,NOTE_AS4,NOTE_C5,NOTE_AS4,NOTE_C5
};
int d4[] = {
    8,8,8,3,3,3,5,8,1
};

void setup(){
  //put your setup code here, to run once:
  //se llama la configuracion del timer:
  //configureTimer12A();
  Serial.begin(9600);
  delay(100);
  
  pinMode(interruptPin1, INPUT_PULLUP);
  pinMode(interruptPin2, INPUT_PULLUP);
  pinMode(interruptPin3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin1), state_Swap1, RISING);
  attachInterrupt(digitalPinToInterrupt(interruptPin2), state_Swap2, RISING);
  attachInterrupt(digitalPinToInterrupt(interruptPin3), state_Swap3, RISING);
  //RISING para cambiar de estado al soltar el boton indicado
  //FALLING para cambiar de estado al presionar el boton indicado
}

void loop(){
  if (S1 == true){
    for(int note = 0; note<63; note++){
      if (d1[note] == 1){
        pre=1100;
      }
      else{
        pre=1000;
      }
      int t = pre/d1[note];
      tone(PC_4, m1[note], t);
      int pause = t*1.3;
      delay(pause);
      noTone(PC_4);
      if (S1 == false){
        break;
      }
    }
  }
  if (S2 == true) {
    for(int note = 0; note<231; note++){
      pre = 1000;
      int t = pre/d2[note];
      tone(PC_4, m2[note], t);
      int pause = t*1.3;
      delay(pause);
      noTone(PC_4);
      if (S2 == false){
        break;
      }
    }
  }
  if (S3 == true) {
    for(int note = 0; note<256; note++){
      pre = 1000;
      int t = pre/d3[note];
      tone(PC_4, m3[note], t);
      int pause = t*1.3;
      delay(pause);
      noTone(PC_4);
      if (S3 == false){
        break;
      }
    }
  }
  if (S4 == true){
    for(int note = 0; note<9; note++){
      pre = 1000;
      int t = pre/d4[note];
      tone(PC_4, m4[note], t);
      int pause = t*1.3;
      delay(pause);
      noTone(PC_4);
      if (S4 == false){
        break;
      }
    }
  }
}

//configuracion de los TIMERS 1A y 2A
void configureTimer12A(){
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1); // Enable Timer 1 Clock
  ROM_IntMasterEnable(); // Enable Interrupts
  
  ROM_TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC); // Configure Timer Operation as Periodic
  //80MHz / CustomValue = Freq(Hz)
  ROM_TimerLoadSet(TIMER1_BASE, TIMER_A, 8000000);
  //interrupciones definidas para cada TIMER
  TimerIntRegister(TIMER1_BASE, TIMER_A, &ola);
  ROM_TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT); // Timer 1A Interrupt when Timeout
  ROM_TimerEnable(TIMER1_BASE, TIMER_A); // Start Timer 1A
  ROM_IntEnable(INT_TIMER1A);  // Enable Timer 1A Interrupt

  ROM_TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC); // Configure Timer Operation as Periodic
  ROM_TimerLoadSet(TIMER2_BASE, TIMER_A, 8000000);
  TimerIntRegister(TIMER2_BASE, TIMER_A, &wenas);
  ROM_TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT); // Timer 1A Interrupt when Timeout
  ROM_TimerEnable(TIMER2_BASE, TIMER_A); // Start Timer 1A
  ROM_IntEnable(INT_TIMER2A);  // Enable Timer 1A Interrupt
}

//interrupcion del TIMER1
void ola(void){
  //reiniciar el TIMER1
  ROM_TimerIntClear(TIMER1_BASE, TIMER_A);
}

//interrupcion del TIMER2
void wenas(void){
  //reiniciar el TIMER2
  ROM_TimerIntClear(TIMER2_BASE, TIMER_A);
}

void state_Swap1(){
  S1 = !S1;
  if (S1 == true){
    Serial.println("Now playing: Terra's Theme");
  }
  else {
    Serial.println("M1 closed");
  }
  
}

void state_Swap2(){
  S2 = !S2;
  if (S2 == true){
    Serial.println("Now playing: Battle Theme");
  }
  else {
    Serial.println("M2 closed");
  }
}

void state_Swap3(){
  S3 = !S3;
  if (S3 == true){
    Serial.println("Now playing: Prelude");
  }
  else {
    Serial.println("M3 closed");
  }
}
