#include <Arduino.h>
#include <math.h>
#include <string.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <U8g2lib.h>
#include <U8x8lib.h>
#include <Adafruit_BMP280.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

#define OLED_RESET -1
#define DHT_PIN 13
#define DHT_TYPE DHT11
#define POWER_PIN  25
#define SIGNAL_PIN 33

const char* ssid = std::getenv("SSID");
const char* password = std::getenv("PASSWORD");

DHT dht(DHT_PIN, DHT_TYPE);
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);
Adafruit_BMP280 bmp;

AsyncWebServer server(80);

String readDHTTemperature() {
  float temperature = dht.readTemperature();
  if (isnan(temperature)) {    
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(temperature);
    return String(temperature);
  }
}

String readDHTHumidity() {
  float humidity = dht.readHumidity();
  if (isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(humidity);
    return String(humidity);
  }
}

String readBMPPressure() {
  float pressure = bmp.readPressure()/100;
  if (isnan(pressure)) {
    Serial.println("Failed to read from BMP sensor!");
    return "--";
  }
  else {
    Serial.println(pressure);
    return String(pressure);
  }
}

String readWaterLevel() {
  digitalWrite(POWER_PIN, HIGH); 
  delay(10);                 
  int value = analogRead(SIGNAL_PIN);
  digitalWrite(POWER_PIN, LOW);
  if (isnan(value)) {
    Serial.println("Failed to read from Water sensor!");
    return "--";
  }
  else {
    Serial.println(value);
    return String(value);
  }
}

void setup() {
  Serial.begin(115200);
  
  dht.begin();
  bmp.begin(0x76);
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  pinMode(POWER_PIN, OUTPUT); 
  digitalWrite(POWER_PIN, LOW);

  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_chroma48medium8_r);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
    u8x8.clearDisplay();
    u8x8.setCursor(0, 0);
    u8x8.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });
  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readBMPPressure().c_str());
  });
  server.on("/waterlevel", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readWaterLevel().c_str());
  });

  server.begin();

}

void loop() {

  delay(5000);

  String temperature = readDHTTemperature();
  String humidity = readDHTHumidity();
  String pressure = readBMPPressure();
  String value = readWaterLevel();

  u8x8.clearDisplay();
  u8x8.setCursor(0, 0);
  u8x8.print("Temp: ");
  u8x8.println(temperature);
  u8x8.print("Humidity: ");
  u8x8.println(humidity);
  u8x8.print("Pressure: ");
  u8x8.println(pressure);
  u8x8.print("Water: ");
  u8x8.println(value);
}