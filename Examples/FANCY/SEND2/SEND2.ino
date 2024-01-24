#include <esp_now.h>
#include <WiFi.h>
#include "ssFont.h"
#include "sFont.h"
#include "mFont.h"

#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

uint8_t receiverAdd[] = {0x34, 0x85, 0x18, 0xAB, 0x35, 0xF4};
String add="34,85,18,AB,35,F4";
esp_now_peer_info_t peerInfo;

//COLORS
unsigned short c1=0x669F;
unsigned short c2=0x347A;
unsigned short red=0x9001;
unsigned short needle_col=TFT_ORANGE;
unsigned short bck=TFT_BLACK;
unsigned short grays[13];

int data=12;
String sta;

double rad=0.01745;
float xpos[3][21];
float ypos[3][21];
float xpos2[3][21];
float ypos2[3][21];
float tx[3][21];
float ty[3][21];

float nx[3][21];
float ny[3][21];

int sx[3]={62,160,258};
int sy[3]={62,122,62};
int r[3]={38,22,38};

int digits[3]={2,7,0};
int chosen,vol,deb,deb2=0;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if(status == ESP_NOW_SEND_SUCCESS) 
  sta="Success"; else sta="Fail";
  
}

void setup() {
 
  pinMode(15,OUTPUT);
  digitalWrite(15,1);

  pinMode(0,INPUT_PULLUP);
  pinMode(14,INPUT_PULLUP);

  tft.init();
  tft.fillScreen(TFT_ORANGE);
  tft.setRotation(3);
  sprite.createSprite(320,170);

     ledcSetup(0, 10000, 8);
     ledcAttachPin(38, 0);
     ledcWrite(0, 80);
    
  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, receiverAdd, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);

     int st;
     for(int j=0;j<3;j++){
     st=270;
      for(int i=0;i<20;i++)
    {
       xpos[j][i]=(r[j]*cos(rad*(st)))+sx[j];
       ypos[j][i]=(r[j]*sin(rad*(st)))+sy[j];
       xpos2[j][i]=((r[j]-5)*cos(rad*(st)))+sx[j];
       ypos2[j][i]=((r[j]-5)*sin(rad*(st)))+sy[j];
       tx[j][i]=((r[j]+12)*cos(rad*(st)))+sx[j];
       ty[j][i]=((r[j]+12)*sin(rad*(st)))+sy[j];
      nx[j][i]=((r[j]-10)*cos(rad*(st)))+sx[j];
       ny[j][i]=((r[j]-10)*sin(rad*(st)))+sy[j];
       st=st+18;
       if(st>=360) st=0;
    }}

     int co=220;
     for(int i=0;i<13;i++)
     {grays[i]=tft.color565(co, co, co);
     co=co-20;}
}


void draw()
  {
    sprite.fillSprite(TFT_BLACK); 
    sprite.setTextDatum(4);

    sprite.loadFont(mFont);
   
    sprite.setTextColor(grays[0],0x020E);
    for(int i=0;i<3;i++){
    sprite.fillRect(133+(i*20),22,16,32,0x020E);  
    sprite.fillRect(133+(i*20),54,16,4,red);  
    sprite.drawString(String(digits[i]),140+(i*20),40);
    }
    sprite.unloadFont();

    sprite.loadFont(ssFont);
    // sprite.drawString(sta,160,20,4);
    //sprite.drawString(String(data),160,65,7);

     sprite.setTextColor(grays[1],grays[9]);
      for(int j=0;j<3;j++){
        
        if(chosen==j)
        sprite.fillSmoothCircle(sx[j],sy[j], r[j]+23,c2, TFT_BLACK);
        else
        sprite.fillSmoothCircle(sx[j],sy[j], r[j]+23,grays[8], TFT_BLACK);
        sprite.fillSmoothCircle(sx[j],sy[j], r[j]+21,grays[10], grays[8]);
      for(int i=0;i<20;i++){ 
      if(i%2==0)  
      {sprite.drawWedgeLine(xpos[j][i],ypos[j][i],xpos2[j][i],ypos2[j][i],1,1,c2,grays[9]);
      sprite.drawString(String(i/2),tx[j][i],ty[j][i]);
      }
      else
      {sprite.fillSmoothCircle(xpos[j][i],ypos[j][i], 1, c1,grays[9]);}
      sprite.drawWedgeLine(sx[j],sy[j],nx[j][digits[j]*2],ny[j][digits[j]*2],6,1,needle_col,grays[9]);
      
      }
      sprite.fillSmoothCircle(sx[j],sy[j], 3,TFT_BLACK, needle_col);
      }
     
      sprite.unloadFont();
      sprite.loadFont(sFont);
      sprite.fillRect(4,130,60,17,red);
      sprite.setTextColor(grays[1],red);
      sprite.drawString("ESP",34,141);

      sprite.fillRect(4,149,60,18,grays[1]);
      sprite.setTextColor(TFT_BLACK,grays[1]);
      sprite.drawString("NOW",34,160);

      if(chosen==3)
      sprite.setTextColor(c1,TFT_BLACK);
      else
      sprite.setTextColor(grays[1],TFT_BLACK);
      sprite.drawString("SEND >>>>",276,148);
     
      sprite.setTextColor(grays[3],TFT_BLACK);
      sprite.drawString("...SENDER...",160,10);
      sprite.unloadFont();

      sprite.drawString(sta,160,68);
      sprite.drawString(add,264,164);

      sprite.setTextColor(TFT_DARKGREEN,TFT_BLACK);
      sprite.drawString(String(vol)+" mV",94,164);

      sprite.drawRect(72,142,26,14,grays[4]);
      sprite.fillRect(98,146,3,6,grays[4]);
      sprite.fillRect(76,146,18,6,TFT_DARKGREEN);

      sprite.pushSprite(0,0);
  }

void loop() {

vol = (analogRead(4) * 2 * 3.3 * 1000) / 4096; 

if(digitalRead(14)==0)
{
if(deb==0)
{deb=1; chosen++; if(chosen>3) chosen=0;}
}else deb=0;

if(digitalRead(0)==0)
{
if(deb2==0)
{deb2=1; 
if(chosen<3) {digits[chosen]++; if(digits[chosen]>9) digits[chosen]=0;}
if(chosen==3){ data=(digits[0]*100)+(digits[1]*10)+digits[2];    esp_now_send(receiverAdd, (uint8_t *) &data, sizeof(data));}
}  
}else deb2=0;

draw();
}



