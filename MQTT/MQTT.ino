#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6F-BV1vtn"
#define BLYNK_TEMPLATE_NAME "BTL"
#define BLYNK_AUTH_TOKEN "Ni8wY7KfFcgoXZhs__cyJUubSzKAgTme"


#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <BlynkSimpleEsp8266.h>


#define TFT_CS 2
#define TFT_DC 0
#define TFT_RST 16 
#define Display_Color_Black 0x0000
#define Display_Color_White 0xFFFF
#define TextSize 1.5
#define REPORTING_PERIOD_MS 1000


Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
PulseOximeter pox;


uint32_t tsLastReport = 0;
const int BUZZE = 12;

const char* ssid = "thang2.4";
const char* password = "thangcachep";

const char* mqtt_server = "maqiatto.com";
const int mqtt_port = 1883;
const char* mqtt_user = "ducthang906@gmail.com"; 
const char* mqtt_password = "ducthang08052002";

const char* topicHeart_rate = "ducthang906@gmail.com/heart_rate";
const char* topicSpO2 = "ducthang906@gmail.com/spo2";

const char* mqtt_id = "esp8266";

WiFiClient client;
PubSubClient espClient(client);


void onBeatDetected()
{
  Serial.println("Beat!");
}


void setup_wifi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: " + WiFi.localIP().toString());
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


void reconnect() {
  // Loop until we're reconnected
  while (!espClient.connected()) {
    if (espClient.connect(mqtt_id, mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
    }
  }
}


void setup(){
  Serial.begin(9600);

  setup_wifi();

  espClient.setServer(mqtt_server, mqtt_port);
  espClient.setCallback(callback);
  Serial.println("Connecting to mqtt");
  while(!espClient.connect(mqtt_id, mqtt_user, mqtt_password))
  {
    delay(500); 
    Serial.print("connecting ");
  }
  Serial.println("Connected to mqtt "); 

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);


  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(Display_Color_Black);
  tft.setTextColor(Display_Color_White);
  tft.setTextSize(TextSize);

  pinMode(BUZZE, OUTPUT);

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
  if (!espClient.connected()) {
    reconnect();
  }
  espClient.loop(); 

  Blynk.run();

  pox.update();

  if (millis() - tsLastReport >= 1000) {
    //pox.update();
    float heart_rate = pox.getHeartRate();
    float spo2 = pox.getSpO2();
    
    String payload_heart_rate = String(heart_rate);
    String payload_spo2 = String(spo2);

    if(heart_rate > 0 && spo2 > 0){
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

    Blynk.virtualWrite(V7, heart_rate); // V1 là ID của nút Gauge 1 trong ứng dụng Blynk
    Blynk.virtualWrite(V8, spo2); // V2 là ID của nút Gauge 2 trong ứng dụng Blynk


    if(espClient.connected()){
      espClient.publish(topicHeart_rate, payload_heart_rate.c_str());
      espClient.publish(topicSpO2, payload_spo2.c_str());
    }else{
      Serial.println("MQTT not connected!");
    }

    tsLastReport = millis();
  }
  digitalWrite(BUZZE, HIGH);
}