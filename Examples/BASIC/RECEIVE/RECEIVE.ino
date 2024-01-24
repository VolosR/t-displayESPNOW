#include <esp_now.h>
#include <WiFi.h>

#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

int data=0;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&data, incomingData, sizeof(data));
  draw();
}

void setup() {
 
  pinMode(15,OUTPUT);
  digitalWrite(15,1);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.drawString("WAITING...",10,10,4);
  sprite.createSprite(320,170);

     ledcSetup(0, 10000, 8);
     ledcAttachPin(38, 0);
     ledcWrite(0, 110);
    
  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_recv_cb(OnDataRecv);
}

void draw()
  {
    sprite.fillSprite(TFT_BLACK); 
    sprite.setTextDatum(4);
     sprite.drawString("RECEIVED",160,30,4);
    sprite.drawString(String(data),160,85,6);
    sprite.pushSprite(0,0);
  }

void loop() {

}



