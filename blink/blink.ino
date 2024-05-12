#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL6F-BV1vtn"
#define BLYNK_TEMPLATE_NAME "BTL"
#define BLYNK_AUTH_TOKEN "Ni8wY7KfFcgoXZhs__cyJUubSzKAgTme"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char ssid[] = "thang2.4";
char pass[] = "thangcachep";

void setup()
{
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop()
{
  Blynk.run();
  sendSensorData();
}

void sendSensorData()
{
  float value1 = random(0, 100); // Giá trị cho nút Gauge 1
  float value2 = random(0, 100); // Giá trị cho nút Gauge 2

  // Gửi giá trị đến các nút Gauge
  Blynk.virtualWrite(V7, value1); // V1 là ID của nút Gauge 1 trong ứng dụng Blynk
  Blynk.virtualWrite(V8, value2); // V2 là ID của nút Gauge 2 trong ứng dụng Blynk

  // Tùy chỉnh thời gian gửi dữ liệu
  delay(1000); // Đợi 1 giây trước khi gửi lại dữ liệu
}
