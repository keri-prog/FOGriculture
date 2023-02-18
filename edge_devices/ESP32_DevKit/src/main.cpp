#include <Arduino.h>
#include <math.h>
#include <string.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHT_PIN 13
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  delay(5000);
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
}