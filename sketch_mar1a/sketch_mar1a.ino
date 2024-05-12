#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <FirebaseESP8266.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

// #define WIFI_SSID "thang2.4"
// #define WIFI_PASSWORD "thangcachep"


#define TFT_CS 2
#define TFT_DC 0
#define TFT_RST 16 
#define Display_Color_Black 0x0000
#define Display_Color_White 0xFFFF
#define TextSize 1.5
#define REPORTING_PERIOD_MS 1000


// #define API_KEY "AIzaSyCL4TIA8Nmfqvgj-VKpk8zOBH0u3DcqzMw"
// #define DATABASE_URL "https://thang-75f12-default-rtdb.asia-southeast1.firebasedatabase.app/"
// #define USER_EMAIL "ducthang906@gmail.com" // Thay đổi thành địa chỉ email Firebase của bạn
// #define USER_PASSWORD "lelybuithuonghien26032002" // Thay đổi thành mật khẩu Firebase của bạn

// FirebaseData fbdo;
// FirebaseData stream;

// FirebaseAuth auth;
// FirebaseConfig config;


Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

PulseOximeter pox;

uint32_t tsLastReport = 0;
const int BUZZE = 12;
// String path = "/data";
// int last = 0;


void onBeatDetected()
{
  Serial.println("Beat!");
}


void setup(){
  Serial.begin(115200);
  // WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  // Serial.print("Connecting to WiFi");
  // unsigned long ms = millis();
  // while(WiFi.status() != WL_CONNECTED){
  //   Serial.print(".");
  //   delay(300);
  // }
  // Serial.println();
  // Serial.print("Connected with IP: ");
  // Serial.println(WiFi.localIP());
  // Serial.println();

  // config.api_key = API_KEY;
  // auth.user.email = USER_EMAIL;
  // auth.user.password = USER_PASSWORD;
  // config.database_url = DATABASE_URL;
  // config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
  // Firebase.reconnectNetwork(true);
  // fbdo.setBSSLBufferSize(4096 /* Kích thước bộ đệm Rx trong byte từ 512 - 16384 */, 1024 /* Kích thước bộ đệm Tx trong byte từ 512 - 16384 */);
  // Firebase.begin(&config, &auth);
  // if (!Firebase.beginStream(stream, path)){
  //   Serial.printf("Stream begin error: %s\n\n", stream.errorReason().c_str());
  // }

  pinMode(BUZZE, OUTPUT);

  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(Display_Color_Black);
  tft.setTextColor(Display_Color_White);
  tft.setTextSize(TextSize);

  if (!pox.begin()) {
    Serial.println("FAILED");
    for(;;);
  }
  else{
    Serial.println("SUCCESS");
  }
  pox.setOnBeatDetectedCallback(onBeatDetected);

}

void loop(){
  pox.update();
  if (millis() - tsLastReport > 1000) {
    float heart_rate = pox.getHeartRate();
    float spo2 = pox.getSpO2();
    if(heart_rate > 0 & spo2 > 0){
      digitalWrite(BUZZE, LOW);
    }
    Serial.print("Heart rate:");
    Serial.print(heart_rate);
    Serial.print("bpm / SpO2:");
    Serial.print(spo2);
    Serial.println("%");

    tft.fillRect(1, 1, 160, 20, Display_Color_Black);
    tft.setCursor(1, 1);
    tft.setTextSize(TextSize);
    tft.setTextColor(Display_Color_White);
    tft.print("HeartRate: ");
    tft.print(heart_rate);
    tft.print(" bpm");
    
    tft.fillRect(1, 30, 160, 20, Display_Color_Black);
    tft.setCursor(1, 30);
    tft.print("SpO2: ");
    tft.print(spo2);
    tft.print(" %");
    tsLastReport = millis();

  }
  digitalWrite(BUZZE, HIGH);
}
