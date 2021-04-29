//***************************************************************************************************************************************
/* Librería para el uso de la pantalla ILI9341 en modo 8 bits
 * Basado en el código de martinayotte - https://www.stm32duino.com/viewtopic.php?t=637
 * Adaptación, migración y creación de nuevas funciones: Pablo Mazariegos y José Morales
 * Con ayuda de: José Guerra
 * IE3027: Electrónica Digital 2 - 2019
 */
//***************************************************************************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "bitmaps.h"
#include "font.h"
#include "lcd_registers.h"

#define LCD_RST PD_0
#define LCD_CS PD_1
#define LCD_RS PD_2
#define LCD_WR PD_3
#define LCD_RD PE_1
int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};

//CONTROLES// 

#define op1a PA_5
#define op1b PA_6
#define op1c PA_7
#define op2a PF_1
#define op2b PE_3
#define op2c PE_2
#define start PE_5

//MUSICA//

#define mus1 PC_7
#define mus2 PD_6
#define mus3 PD_7
#define mus4 PF_4

//***************************************************************************************************************************************
// Functions Prototypes
//***************************************************************************************************************************************

void LCD_Init(void);
void LCD_CMD(uint8_t cmd);
void LCD_DATA(uint8_t data);
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_Clear(unsigned int c);
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void LCD_Print(String text, int x, int y, int fontSize, int color, int background);

void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);


extern uint8_t inicio[];
extern uint8_t fondo[];
//***************************************************************************************************************************************
// Inicialización
//***************************************************************************************************************************************
void setup() {
  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ); 
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  LCD_Init();
  LCD_Clear(0x00);
  
//CONTROLES//
  pinMode(PA_5, INPUT);
  pinMode(PA_6, INPUT);
  pinMode(PA_7, INPUT);
  pinMode(PF_1, INPUT);
  pinMode(PE_3, INPUT);
  pinMode(PE_2, INPUT);
  pinMode(PE_5, INPUT);
  
//MUSICA
  pinMode(PC_7, OUTPUT);
  pinMode(PD_6, OUTPUT);
  pinMode(PD_7, OUTPUT);
  pinMode(PF_4, OUTPUT);

  //MENU
  digitalWrite(mus1, HIGH);
  digitalWrite(mus2, LOW);
  digitalWrite(mus3, LOW);
  digitalWrite(mus4, LOW);
  
  FillRect(0, 0, 320, 240, 0xffff);
  LCD_Bitmap(7, 40, 305, 70, inicio);  
  String text1 = "Presione Start";
  String text2 = "Schneider Cat Studios";
  String text3 = "Derechos reservados, 2021";

  LCD_Print(text1, 40, 160, 2, 0x0000, 0xffff);
  LCD_Print(text2, 60, 210, 1, 0x0000, 0xffff);
  LCD_Print(text3, 60, 225, 1, 0x0000, 0xffff);
  delay (8000);

  //ESCENA
  digitalWrite(mus1, LOW);
  digitalWrite(mus2, HIGH);
  digitalWrite(mus3, LOW);
  digitalWrite(mus4, LOW);

  for(int x= 0; x<320; x++){
    LCD_Bitmap(x, 0, 40, 160, fondo);
    x+=39;
  }
  LCD_Bitmap(0, 0, 40, 160, fondo);
  LCD_Bitmap(0, 160, 10, 80, cuadros1);
  for(int x= 10; x <150; x++){
    LCD_Bitmap(x, 160, 10, 80, cuadros2);
    x +=9;
  }
  LCD_Bitmap(150, 160, 10, 80, cuadros3);
  LCD_Bitmap(160, 160, 10, 80, cuadros1);
  for(int x=170; x<310; x++){
    LCD_Bitmap(x, 160, 10, 80, cuadros2);
    x +=9;
  }  
  LCD_Bitmap(310, 160, 10, 80, cuadros3);
  delay(5000);
}
//***************************************************************************************************************************************
// Loop Infinito
//***************************************************************************************************************************************

void loop() {

  digitalWrite(mus1, LOW);
  digitalWrite(mus2, LOW);
  digitalWrite(mus3, HIGH);
  digitalWrite(mus4, LOW);

LCD_Sprite( 250, 100, 30, 47, cloud, 1, 1, 1, 0);
LCD_Sprite( 220, 45, 30, 48, tifa, 2, 1, 1, 0); 
LCD_Sprite( 40, 60, 62, 63, bomb, 1, 1, 0 ,0);
LCD_Print("Cloud", 72, 165, 2, 0xffff, 0x0019);
LCD_Print("Tifa", 246, 165, 2, 0xffff, 0x0019); 
String opa = "Ataque";
String opb = "Limite";
String opt = "Disminuir Defensa";
String opc = "Potenciar Ataque";

if (digitalRead(op1a) == 1){
  LCD_Print(opa, 15, 185, 1, 0xf800, 0x0019);
  LCD_Print(opb, 15, 200, 1, 0xffff, 0x0019);
  LCD_Print(opc, 15, 215, 1, 0xffff, 0x0019);
  if(digitalRead(op2a) == 1){   
    LCD_Print(opa, 175, 185, 1, 0xf800, 0x0019);
    LCD_Print(opb, 175, 200, 1, 0xffff, 0x0019);
    LCD_Print(opt, 175, 215, 1, 0xffff, 0x0019);
  }
  else if(digitalRead(op2b) == 1){
    LCD_Print(opa, 175, 185, 1, 0xffff, 0x0019);
    LCD_Print(opb, 175, 200, 1, 0xf800, 0x0019);
    LCD_Print(opt, 175, 215, 1, 0xffff, 0x0019);
  }
  else if(digitalRead(op2c) == 1){
    LCD_Print(opa, 175, 185, 1, 0xffff, 0x0019);
    LCD_Print(opb, 175, 200, 1, 0xffff, 0x0019);
    LCD_Print(opt, 175, 215, 1, 0xf800, 0x0019);
  }
  else{
    LCD_Print(opa, 175, 185, 1, 0xffff, 0x0019);
    LCD_Print(opb, 175, 200, 1, 0xffff, 0x0019);
    LCD_Print(opt, 175, 215, 1, 0xffff, 0x0019);     
  } 
}
if (digitalRead(op1b) == 1){
  LCD_Print(opa, 15, 185, 1, 0xffff, 0x0019);
  LCD_Print(opb, 15, 200, 1, 0xf800, 0x0019);
  LCD_Print(opc, 15, 215, 1, 0xffff, 0x0019);
  if(digitalRead(op2a) == 1){   
    LCD_Print(opa, 175, 185, 1, 0xf800, 0x0019);
    LCD_Print(opb, 175, 200, 1, 0xffff, 0x0019);
    LCD_Print(opt, 175, 215, 1, 0xffff, 0x0019);
  }
  else if(digitalRead(op2b) == 1){
    LCD_Print(opa, 175, 185, 1, 0xffff, 0x0019);
    LCD_Print(opb, 175, 200, 1, 0xf800, 0x0019);
    LCD_Print(opt, 175, 215, 1, 0xffff, 0x0019);
  }
  else if(digitalRead(op2c) == 1){
    LCD_Print(opa, 175, 185, 1, 0xffff, 0x0019);
    LCD_Print(opb, 175, 200, 1, 0xffff, 0x0019);
    LCD_Print(opt, 175, 215, 1, 0xf800, 0x0019);
  }
  else{
    LCD_Print(opa, 175, 185, 1, 0xffff, 0x0019);
    LCD_Print(opb, 175, 200, 1, 0xffff, 0x0019);
    LCD_Print(opt, 175, 215, 1, 0xffff, 0x0019);     
  } 
}
if (digitalRead(op1c) == 1){
  LCD_Print(opa, 15, 185, 1, 0xffff, 0x0019);
  LCD_Print(opb, 15, 200, 1, 0xffff, 0x0019);
  LCD_Print(opc, 15, 215, 1, 0xf800, 0x0019);
  if(digitalRead(op2a) == 1){   
    LCD_Print(opa, 175, 185, 1, 0xf800, 0x0019);
    LCD_Print(opb, 175, 200, 1, 0xffff, 0x0019);
    LCD_Print(opt, 175, 215, 1, 0xffff, 0x0019);
  }
  else if(digitalRead(op2b) == 1){
    LCD_Print(opa, 175, 185, 1, 0xffff, 0x0019);
    LCD_Print(opb, 175, 200, 1, 0xf800, 0x0019);
    LCD_Print(opt, 175, 215, 1, 0xffff, 0x0019);
  }
  else if(digitalRead(op2c) == 1){
    LCD_Print(opa, 175, 185, 1, 0xffff, 0x0019);
    LCD_Print(opb, 175, 200, 1, 0xffff, 0x0019);
    LCD_Print(opt, 175, 215, 1, 0xf800, 0x0019);
  }
  else{
    LCD_Print(opa, 175, 185, 1, 0xffff, 0x0019);
    LCD_Print(opb, 175, 200, 1, 0xffff, 0x0019);
    LCD_Print(opt, 175, 215, 1, 0xffff, 0x0019);     
  } 
}else{
  LCD_Print(opa, 15, 185, 1, 0xffff, 0x0019);
  LCD_Print(opb, 15, 200, 1, 0xffff, 0x0019);
  LCD_Print(opc, 15, 215, 1, 0xffff, 0x0019);
  LCD_Print(opa, 175, 185, 1, 0xffff, 0x0019);
  LCD_Print(opb, 175, 200, 1, 0xffff, 0x0019);
  LCD_Print(opt, 175, 215, 1, 0xffff, 0x0019);
}
}
//***************************************************************************************************************************************
// Función para inicializar LCD
//***************************************************************************************************************************************
void LCD_Init(void) {
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_RD, OUTPUT);
  for (uint8_t i = 0; i < 8; i++){
    pinMode(DPINS[i], OUTPUT);
  }
  //****************************************
  // Secuencia de Inicialización
  //****************************************
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_RST, HIGH);
  delay(5);
  digitalWrite(LCD_RST, LOW);
  delay(20);
  digitalWrite(LCD_RST, HIGH);
  delay(150);
  digitalWrite(LCD_CS, LOW);
  //****************************************
  LCD_CMD(0xE9);  // SETPANELRELATED
  LCD_DATA(0x20);
  //****************************************
  LCD_CMD(0x11); // Exit Sleep SLEEP OUT (SLPOUT)
  delay(100);
  //****************************************
  LCD_CMD(0xD1);    // (SETVCOM)
  LCD_DATA(0x00);
  LCD_DATA(0x71);
  LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0xD0);   // (SETPOWER) 
  LCD_DATA(0x07);
  LCD_DATA(0x01);
  LCD_DATA(0x08);
  //****************************************
  LCD_CMD(0x36);  // (MEMORYACCESS)
  LCD_DATA(0x40|0x80|0x20|0x08); // LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0x3A); // Set_pixel_format (PIXELFORMAT)
  LCD_DATA(0x05); // color setings, 05h - 16bit pixel, 11h - 3bit pixel
  //****************************************
  LCD_CMD(0xC1);    // (POWERCONTROL2)
  LCD_DATA(0x10);
  LCD_DATA(0x10);
  LCD_DATA(0x02);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC0); // Set Default Gamma (POWERCONTROL1)
  LCD_DATA(0x00);
  LCD_DATA(0x35);
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC5); // Set Frame Rate (VCOMCONTROL1)
  LCD_DATA(0x04); // 72Hz
  //****************************************
  LCD_CMD(0xD2); // Power Settings  (SETPWRNORMAL)
  LCD_DATA(0x01);
  LCD_DATA(0x44);
  //****************************************
  LCD_CMD(0xC8); //Set Gamma  (GAMMASET)
  LCD_DATA(0x04);
  LCD_DATA(0x67);
  LCD_DATA(0x35);
  LCD_DATA(0x04);
  LCD_DATA(0x08);
  LCD_DATA(0x06);
  LCD_DATA(0x24);
  LCD_DATA(0x01);
  LCD_DATA(0x37);
  LCD_DATA(0x40);
  LCD_DATA(0x03);
  LCD_DATA(0x10);
  LCD_DATA(0x08);
  LCD_DATA(0x80);
  LCD_DATA(0x00);
  //****************************************
  LCD_CMD(0x2A); // Set_column_address 320px (CASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x3F);
  //****************************************
  LCD_CMD(0x2B); // Set_page_address 480px (PASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0xE0);
//  LCD_DATA(0x8F);
  LCD_CMD(0x29); //display on 
  LCD_CMD(0x2C); //display on

  LCD_CMD(ILI9341_INVOFF); //Invert Off
  delay(120);
  LCD_CMD(ILI9341_SLPOUT);    //Exit Sleep
  delay(120);
  LCD_CMD(ILI9341_DISPON);    //Display on
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar comandos a la LCD - parámetro (comando)
//***************************************************************************************************************************************
void LCD_CMD(uint8_t cmd) {
  digitalWrite(LCD_RS, LOW);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = cmd;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar datos a la LCD - parámetro (dato)
//***************************************************************************************************************************************
void LCD_DATA(uint8_t data) {
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = data;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para definir rango de direcciones de memoria con las cuales se trabajara (se define una ventana)
//***************************************************************************************************************************************
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  LCD_CMD(0x2a); // Set_column_address 4 parameters
  LCD_DATA(x1 >> 8);
  LCD_DATA(x1);   
  LCD_DATA(x2 >> 8);
  LCD_DATA(x2);   
  LCD_CMD(0x2b); // Set_page_address 4 parameters
  LCD_DATA(y1 >> 8);
  LCD_DATA(y1);   
  LCD_DATA(y2 >> 8);
  LCD_DATA(y2);   
  LCD_CMD(0x2c); // Write_memory_start
}
//***************************************************************************************************************************************
// Función para borrar la pantalla - parámetros (color)
//***************************************************************************************************************************************
void LCD_Clear(unsigned int c){  
  unsigned int x, y;
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);   
  SetWindows(0, 0, 319, 239); // 479, 319);
  for (x = 0; x < 320; x++)
    for (y = 0; y < 240; y++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
    }
  digitalWrite(LCD_CS, HIGH);
} 
//***************************************************************************************************************************************
// Función para dibujar una línea horizontal - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + x;
  SetWindows(x, y, l, y);
  j = l;// * 2;
  for (i = 0; i < l; i++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea vertical - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i,j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + y;
  SetWindows(x, y, x, l);
  j = l; //* 2;
  for (i = 1; i <= j; i++) {
    LCD_DATA(c >> 8); 
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);  
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  H_line(x  , y  , w, c);
  H_line(x  , y+h, w, c);
  V_line(x  , y  , h, c);
  V_line(x+w, y  , h, c);
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo relleno - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
/*void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  unsigned int i;
  for (i = 0; i < h; i++) {
    H_line(x  , y  , w, c);
    H_line(x  , y+i, w, c);
  }
}
*/

void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+w;
  y2 = y+h;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = w*h*2-1;
  unsigned int i, j;
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      LCD_DATA(c >> 8);
      LCD_DATA(c);
      
      //LCD_DATA(bitmap[k]);    
      k = k - 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar texto - parámetros ( texto, coordenada x, cordenada y, color, background) 
//***************************************************************************************************************************************
void LCD_Print(String text, int x, int y, int fontSize, int color, int background) {
  int fontXSize ;
  int fontYSize ;
  
  if(fontSize == 1){
    fontXSize = fontXSizeSmal ;
    fontYSize = fontYSizeSmal ;
  }
  if(fontSize == 2){
    fontXSize = fontXSizeBig ;
    fontYSize = fontYSizeBig ;
  }
  
  char charInput ;
  int cLength = text.length();
 // Serial.println(cLength,DEC);
  int charDec ;
  int c ;
  int charHex ;
  char char_array[cLength+1];
  text.toCharArray(char_array, cLength+1) ;
  for (int i = 0; i < cLength ; i++) {
    charInput = char_array[i];
//    Serial.println(char_array[i]);
    charDec = int(charInput);
    digitalWrite(LCD_CS, LOW);
    SetWindows(x + (i * fontXSize), y, x + (i * fontXSize) + fontXSize - 1, y + fontYSize );
    long charHex1 ;
    for ( int n = 0 ; n < fontYSize ; n++ ) {
      if (fontSize == 1){
        charHex1 = pgm_read_word_near(smallFont + ((charDec - 32) * fontYSize) + n);
      }
      if (fontSize == 2){
        charHex1 = pgm_read_word_near(bigFont + ((charDec - 32) * fontYSize) + n);
      }
      for (int t = 1; t < fontXSize + 1 ; t++) {
        if (( charHex1 & (1 << (fontXSize - t))) > 0 ) {
          c = color ;
        } else {
          c = background ;
        }
        LCD_DATA(c >> 8);
        LCD_DATA(c);
      }
    }
    digitalWrite(LCD_CS, HIGH);
  }
}
//***************************************************************************************************************************************
// Función para dibujar una imagen a partir de un arreglo de colores (Bitmap) Formato (Color 16bit R 5bits G 6bits B 5bits)
//***************************************************************************************************************************************
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]){  
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+width;
  y2 = y+height;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = 0;
  unsigned int i, j;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      //LCD_DATA(bitmap[k]);    
      k = k + 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una imagen sprite - los parámetros columns = número de imagenes en el sprite, index = cual desplegar, flip = darle vuelta
//***************************************************************************************************************************************
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset){
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 

  unsigned int x2, y2;
  x2 =   x+width;
  y2=    y+height;
  SetWindows(x, y, x2-1, y2-1);
  int k = 0;
  int ancho = ((width*columns));
  if(flip){
  for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width -1 - offset)*2;
      k = k+width*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k - 2;
     } 
  }
  }else{
     for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width + 1 + offset)*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k + 2;
     } 
  }
    
    
    }
  digitalWrite(LCD_CS, HIGH);
}
