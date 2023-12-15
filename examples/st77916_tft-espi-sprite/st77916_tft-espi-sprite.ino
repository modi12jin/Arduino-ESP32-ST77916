#include "ST77916_LCD.h"
#include <TFT_eSPI.h>

#define QSPI_SCK 4
#define QSPI_0 3
#define QSPI_1 10
#define QSPI_2 9
#define QSPI_3 6
#define QSPI_CS 5
#define LCD_RST 0

#define LED 1  // 背光

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);
ST77916_LCD lcd = ST77916_LCD(QSPI_SCK, QSPI_0, QSPI_1, QSPI_2, QSPI_3, QSPI_CS, LCD_RST);

void setup()
{  
  lcd.begin();
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);  // 高电平
  //lcd_setRotation(1);
  sprite.createSprite(360, 360);
  sprite.setSwapBytes(1);
}

void draw()
{
 sprite.fillSprite(TFT_BLACK);
 sprite.drawString("Hello World",20,20,4);
 sprite.fillRect(10,100,60,60,TFT_RED);
 sprite.fillRect(80,100,60,60,TFT_GREEN);
 sprite.fillRect(150,100,60,60,TFT_BLUE);
 
 lcd.lcd_PushColors(0, 0, 360, 360, (uint16_t*)sprite.getPointer());
}


void loop()
{
  draw();
}
