#include <Arduino.h>
#include <math.h>
#include <string.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <Adafruit_BMP280.h>

#define OLED_RESET -1
#define DHT_PIN 11
#define DHT_TYPE DHT11
#define POWER_PIN  12
#define SIGNAL_PIN A2

DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_SH1106 display(OLED_RESET);
Adafruit_BMP280 bmp;

void setup() {

  Serial.begin(115200);
  bmp.begin(0x76);
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  dht.begin();
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  pinMode(POWER_PIN, OUTPUT); 
  digitalWrite(POWER_PIN, LOW);
  display.setsetTextSize(1);
  display.setTextColor(WHITE);

}

void loop() {

  delay(5000);
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  float pressure = bmp.readPressure();
  digitalWrite(POWER_PIN, HIGH);  // turn the sensor ON
  delay(10);                      // wait 10 milliseconds
  int value = analogRead(SIGNAL_PIN); // read the analog value from sensor
  digitalWrite(POWER_PIN, LOW);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Temp: ");
  display.println(temperature);
  display.print("Humidity: ");
  display.println(humidity);
  display.print("Pressure: ");
  display.println(pressure/100);
  display.print("Water: ");
  display.println(value);
  display.display();

}