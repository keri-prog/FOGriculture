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
#define BUZZER_PIN 32

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

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>FOGriculture Edge Server</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">&percnt;</sup>
  </p>
  <p>
    <i class="fas fa-tachometer-alt" style="color:##059e8a;"></i> 
    <span class="dht-labels">Pressure</span>
    <span id="pressure">%PRESSURE%</span>
    <sup class="units">Pa</sup>
  </p>
  <p>
    <i class="fa-solid fa-water" style="color:#00add6;"></i> 
    <span class="dht-labels">Water Level</span>
    <span id="waterlevel">%WATERLEVEL%</span>
    <sup class="units">mm</sup>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("pressure").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/pressure", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("waterlevel").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/waterlevel", true);
  xhttp.send();
}, 10000 ) ;
</script>
</html>)rawliteral";

// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return readDHTTemperature();
  }
  else if(var == "HUMIDITY"){
    return readDHTHumidity();
  }
  else if(var == "PRESSURE"){
    return readBMPPressure();
  }
  else if(var == "WATERLEVEL"){
    return readWaterLevel();
  }
  return String();
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
  pinMode(BUZZER_PIN, OUTPUT); 
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
  server.on("/buzzer", HTTP_GET, [] (AsyncWebServerRequest *request) {
    if (request->hasParam("state"))
      digitalWrite(BUZZER_PIN, request->getParam("state")->value().toInt());
    request->send(200, "text/plain", "OK");
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
  u8x8.println(WiFi.localIP());

}