#include <Arduino.h>
#include <math.h>
#include <string.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <U8g2lib.h>
#include <U8x8lib.h>

#define OLED_RESET -1
#define DHT_PIN 13
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
}

void loop() {
  delay(5000);
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  u8x8.clearDisplay();
  u8x8.setCursor(0, 0);
  u8x8.print("Temp: ");
  u8x8.println(temperature);
  u8x8.print("Humidity: ");
  u8x8.println(humidity);
}