#include <esp_now.h>
#include <WiFi.h>

#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

uint8_t receiverAdd[] = {0x34 0x85, 0x18, 0xAB, 0x35, 0xF4};
esp_now_peer_info_t peerInfo;

int data=12;
String sta;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if(status == ESP_NOW_SEND_SUCCESS) 
  sta="Delivery Success"; else sta="Delivery Fail";
  draw();
}

void setup() {
 
  pinMode(15,OUTPUT);
  digitalWrite(15,1);

  tft.init();
  tft.fillScreen(TFT_ORANGE);
  tft.setRotation(1);
  sprite.createSprite(320,170);

     ledcSetup(0, 10000, 8);
     ledcAttachPin(38, 0);
     ledcWrite(0, 120);
    
  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, receiverAdd, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);
 
}


void draw()
  {
    sprite.fillSprite(TFT_BLACK); 
    sprite.setTextDatum(4);
    sprite.drawString(sta,160,20,4);
    sprite.drawString(String(data),160,65,7);
    sprite.pushSprite(0,0);
  }

void loop() {
data++;
esp_now_send(receiverAdd, (uint8_t *) &data, sizeof(data));

delay(2000);
}



