#include <esp_now.h>
#include <WiFi.h>
#include "ssFont.h"
#include "sFont.h"
#include "mFont.h"

#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

//COLORS
unsigned short c1=0x669F;
unsigned short c2=0x347A;
unsigned short red=0x9001;
unsigned short needle_col=TFT_ORANGE;
unsigned short bck=TFT_BLACK;
unsigned short grays[13];

int data=0;
int digits[3]={0,0,0};
int vol;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&data, incomingData, sizeof(data));
  draw();
}


void setup() {
 
  pinMode(15,OUTPUT);
  digitalWrite(15,1);

  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);
  tft.drawString("WAITING...",10,10,4);
  sprite.createSprite(320,170);

     ledcSetup(0, 10000, 8);
     ledcAttachPin(38, 0);
     ledcWrite(0, 80);
    
  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_recv_cb(OnDataRecv);

     int co=220;
     for(int i=0;i<13;i++)
     {grays[i]=tft.color565(co, co, co);
     co=co-20;}
}


void draw()
  {
    vol = (analogRead(4) * 2 * 3.3 * 1000) / 4096; 
    sprite.fillSprite(TFT_BLACK); 
    sprite.setTextDatum(4);

    sprite.loadFont(mFont);
   
    sprite.setTextColor(grays[0],0x020E);
    
    sprite.fillRect(70,30,180,80,0x020E);  
    sprite.fillRect(70,110,180,5,red);  
    sprite.drawString(String(data),160,76);
  
    sprite.unloadFont();

    sprite.loadFont(ssFont);
    sprite.setTextColor(grays[1],grays[9]);
    
      sprite.unloadFont();
      sprite.loadFont(sFont);
      sprite.fillRect(4,130,60,17,red);
      sprite.setTextColor(grays[1],red);
      sprite.drawString("ESP",34,141);

      sprite.fillRect(4,149,60,18,grays[1]);
      sprite.setTextColor(TFT_BLACK,grays[1]);
      sprite.drawString("NOW",34,160);

      sprite.setTextColor(grays[3],TFT_BLACK);
      sprite.drawString("...RECEIVED...",160,10);
      sprite.unloadFont();

      sprite.setTextColor(TFT_DARKGREEN,TFT_BLACK);
      sprite.drawString(String(vol)+" mV",94,164);

      sprite.drawRect(72,142,26,14,grays[4]);
      sprite.fillRect(98,146,3,6,grays[4]);
      sprite.fillRect(76,146,18,6,TFT_DARKGREEN);

      sprite.pushSprite(0,0);
  }

void loop() {


}



