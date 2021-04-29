// Código original tomado de:
// https://henryforceblog.wordpress.com/2015/05/02/blink-example-using-timer-on-a-tiva-launchpad/
// Modificado/comentado por Luis Alberto Rivera

#include "noteCode.h"

#include "wiring_private.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"

int PIN1;
int PIN2;
int PIN3;
int PIN4;
int Push1;
int Push2;
int Push3;
int Push4;
int Push5;
int Push6;
int Push7;
int Push8;
int controls;
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
  pinMode(PA_2,INPUT);
  pinMode(PA_3,INPUT);
  pinMode(PA_4,INPUT);
  pinMode(PB_6,INPUT);
  pinMode(PD_3,INPUT);
  pinMode(PA_7,INPUT);
  pinMode(PE_3,INPUT);
  pinMode(PA_6,INPUT);
  pinMode(PE_2,INPUT);
  pinMode(PA_5,INPUT);
  pinMode(PE_1,INPUT);
  pinMode(PB_4,INPUT);
}

void loop(){
  PIN1 = digitalRead(PA_2);
  PIN2 = digitalRead(PA_3);
  PIN3 = digitalRead(PA_4);
  PIN4 = digitalRead(PB_6);
  Push1 = digitalRead(PD_3);
  Push2 = digitalRead(PA_7);
  Push3 = digitalRead(PE_3);
  Push4 = digitalRead(PA_6);
  Push5 = digitalRead(PE_2);
  Push6 = digitalRead(PA_5);
  Push7 = digitalRead(PE_1);
  Push8 = digitalRead(PB_4);
  controls = Push1 + Push2*2 + Push3*4 + Push4*8 + Push5*16 + Push6*32 + Push7*64 + Push8*128;
  Serial.println(controls);
  if (PIN1 == 1 && PIN2 == 0 && PIN3 == 0 && PIN4 == 0){
    //Prelude
    for(int note = 0; note<256; note++){
      pre = 1100;
      int t = pre/d3[note];
      tone(PC_4, m3[note], t);
      int pause = t*1.3;
      delay(pause);
      noTone(PC_4);
      PIN1 = digitalRead(PA_2);
      Push1 = digitalRead(PD_3);
      Push2 = digitalRead(PA_7);
      Push3 = digitalRead(PE_3);
      Push4 = digitalRead(PA_6);
      Push5 = digitalRead(PE_2);
      Push6 = digitalRead(PA_5);
      Push7 = digitalRead(PE_1);
      Push8 = digitalRead(PB_4);
      controls = Push1 + Push2*2 + Push3*4 + Push4*8 + Push5*16 + Push6*32 + Push7*64 + Push8*128;
      Serial.println(controls);
      if (PIN1 == 0){
        break;
      }
    }
  }
  else if (PIN1 == 0 && PIN2 == 1 && PIN3 == 0 && PIN4 == 0){
    //Terra's Theme
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
      PIN2 = digitalRead(PA_3);
      Push1 = digitalRead(PD_3);
      Push2 = digitalRead(PA_7);
      Push3 = digitalRead(PE_3);
      Push4 = digitalRead(PA_6);
      Push5 = digitalRead(PE_2);
      Push6 = digitalRead(PA_5);
      Push7 = digitalRead(PE_1);
      Push8 = digitalRead(PB_4);
      controls = Push1 + Push2*2 + Push3*4 + Push4*8 + Push5*16 + Push6*32 + Push7*64 + Push8*128;
      Serial.println(controls);
      if (PIN2 == 0){
        break;
      }
    }
  }
  else if (PIN1 == 0 && PIN2 == 0 && PIN3 == 1 && PIN4 == 0){
    //Battle Theme
    for(int note = 0; note<231; note++){
      pre = 1000;
      int t = pre/d2[note];
      tone(PC_4, m2[note], t);
      int pause = t*1.3;
      delay(pause);
      noTone(PC_4);
      PIN3 = digitalRead(PA_4);
      Push1 = digitalRead(PD_3);
      Push2 = digitalRead(PA_7);
      Push3 = digitalRead(PE_3);
      Push4 = digitalRead(PA_6);
      Push5 = digitalRead(PE_2);
      Push6 = digitalRead(PA_5);
      Push7 = digitalRead(PE_1);
      Push8 = digitalRead(PB_4);
      controls = Push1 + Push2*2 + Push3*4 + Push4*8 + Push5*16 + Push6*32 + Push7*64 + Push8*128;
      Serial.println(controls);
      if (PIN3 == 0){
        break;
      }
    }
  }
  else if (PIN1 == 0 && PIN2 == 0 && PIN3 == 0 && PIN4 == 1){
    //Victory Fanfare
    delay(1000);
    for(int note = 0; note<9; note++){
      pre = 1000;
      int t = pre/d4[note];
      tone(PC_4, m4[note], t);
      int pause = t*1.3;
      delay(pause);
      noTone(PC_4);
      PIN4 = digitalRead(PB_6);
      Push1 = digitalRead(PD_3);
      Push2 = digitalRead(PA_7);
      Push3 = digitalRead(PE_3);
      Push4 = digitalRead(PA_6);
      Push5 = digitalRead(PE_2);
      Push6 = digitalRead(PA_5);
      Push7 = digitalRead(PE_1);
      Push8 = digitalRead(PB_4);
      controls = Push1 + Push2*2 + Push3*4 + Push4*8 + Push5*16 + Push6*32 + Push7*64 + Push8*128;
      Serial.println(controls);
      //if (PIN4 == 0){
        //break;
      //}
    }
  }
}
