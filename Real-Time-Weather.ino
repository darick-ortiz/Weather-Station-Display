// --------------------- BITMAPS ----------------------------------------
// 'weather_sunny_16x16'
const unsigned char bitmapSunny [] PROGMEM = {
  0x00, 0x00, 0x01, 0x80, 0x21, 0x84, 0x40, 0x02, 0x07, 0xe0, 0x08, 0x10, 0x10, 0x08, 0x10, 0x08,
  0x10, 0x08, 0x10, 0x08, 0x08, 0x10, 0x07, 0xe0, 0x40, 0x02, 0x21, 0x84, 0x01, 0x80, 0x00, 0x00
};

// 'weather_rain_16x16'
const unsigned char bitmapRain [] PROGMEM = {
  0x00, 0x00, 0x0f, 0xf0, 0x10, 0x08, 0x20, 0x04, 0x40, 0x02, 0x40, 0x02, 0x3f, 0xfc, 0x00, 0x00,
  0x04, 0x40, 0x04, 0x40, 0x08, 0x80, 0x08, 0x80, 0x11, 0x10, 0x11, 0x10, 0x00, 0x00, 0x00, 0x00
};

// 'weather_snow_16x16'
const unsigned char bitmapSnow [] PROGMEM = {
  0x01, 0x80, 0x01, 0x80, 0x13, 0xc8, 0x0b, 0xd0, 0x3f, 0xfc, 0x0b, 0xd0, 0x13, 0xc8, 0x01, 0x80,
  0x01, 0x80, 0x13, 0xc8, 0x0b, 0xd0, 0x3f, 0xfc, 0x0b, 0xd0, 0x13, 0xc8, 0x01, 0x80, 0x01, 0x80
};

// 'weather_lightning_16x16'
const unsigned char bitmapLightning [] PROGMEM = {
  0x00, 0x00, 0x0f, 0xf0, 0x10, 0x08, 0x20, 0x04, 0x40, 0x02, 0x3f, 0xfc, 0x00, 0x80, 0x01, 0x00,
  0x03, 0x00, 0x06, 0x00, 0x04, 0x00, 0x08, 0x00, 0x10, 0x00, 0x20, 0x00, 0x40, 0x00, 0x00, 0x00
};

// 'alarm_16x16' (Clock icon)
const unsigned char bitmapClock [] PROGMEM = {
  0x07, 0xe0, 0x18, 0x18, 0x20, 0x04, 0x41, 0x02, 0x41, 0x02, 0x81, 0x01, 0x81, 0x31, 0x81, 0x61,
  0x80, 0xc1, 0x81, 0x81, 0x41, 0x02, 0x41, 0x02, 0x20, 0x04, 0x18, 0x18, 0x07, 0xe0, 0x00, 0x00
};

// 'cloud_filled_16x16'
const unsigned char bitmapCloud [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x06, 0x30, 0x0f, 0xf8, 0x1f, 0xfc, 0x3f, 0xfe, 0x7f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xfe, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00
};

// 'thermometer_16x16', 16x16px
const unsigned char bitmapTemp [] PROGMEM = {
	0x01, 0x80, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 
	0x02, 0x40, 0x04, 0x20, 0x09, 0x10, 0x09, 0x10, 0x09, 0x10, 0x09, 0x10, 0x04, 0x20, 0x03, 0xc0
};

// Array of all bitmaps in case I need later
const unsigned char* allBitmapArray[7] = {
	bitmapClock,
	bitmapLightning,
	bitmapRain,
	bitmapSnow,
	bitmapSunny,
	bitmapTemp,
  bitmapCloud
};

// ----------------------------- Includes ----------------------------------
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiUDP.h>
// ------------------------------ Definitions --------------------------------
#define DHTPIN D5
#define DHTTYPE DHT11
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 
#define BUTTON_PIN D3

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -18000, 60000); // Website, Timezone to mankato, sync time 1 minute
DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,&Wire, OLED_RESET);

float currentTemp = 0;
String currentCity = "";
String weatherCondition = "";
String currentCondition = "";

// ------------------------------- Personal --------------------------------
const char* ssid = "SSID"; 
const char* password = "PSWRD";
const char* apiKey = "API";
const char* city = "Mankato,MN,US";
// ------------------------------- Functions -------------------------------

//  Connect to wifi and lets know if successful.
void connectWiFi(){  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected.");
}

// Obtain a big weather string value from Open Weather Map website
String getWeather(){
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("WiFi lost, reconnecting...");
    WiFi.disconnect();
    WiFi.begin(ssid, password);

    int attempts = 0;
    while(WiFi.status() != WL_CONNECTED && attempts < 20){
      delay(500);
      Serial.println("Reconnect Failed.");
      return "";
    }
    Serial.println("Reconnected.");

  }

  WiFiClient client;
  HTTPClient http;

  String url = "http://api.openweathermap.org/data/2.5/weather?q=";
  url += city;
  url += "&appid=";
  url += apiKey;
  url += "&units=imperial";

  http.begin(client, url);
  int httpCode = http.GET();

  if(httpCode != 200){
    Serial.print("HTTP Error: ");
    Serial.println(httpCode);
    http.end();
    return "";
  }
  String weatherValue = http.getString();
  http.end();
  return weatherValue;
}

// Draws all the images and the values to go with them
void screenDrawer(String weatherCondition, float temp, String currentTime, String cityName){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Clock drawer -- YELLOW PART OF OLED --
  display.drawBitmap(0, 0, bitmapClock, 16, 16, 1);
  display.setCursor(20, 4);
  display.print(currentTime); // HH:MM

  // City name
  display.setCursor(75, 4);
  display.print(currentCity);

  // Temperature drawer -- BLUE PART OF OLED STARTS --
  display.drawBitmap(2, 20, bitmapTemp, 16, 16, 1); // Draw thermometer
  display.setCursor(25, 24); // Move to the right of drawing
  display.print(temp, 1); // Print temperature
  display.print(" F");

  // Icon picker
  const unsigned char* selectedIcon = bitmapSunny; // Default

  if (weatherCondition == "Rain")selectedIcon = bitmapRain;
  else if(weatherCondition == "Snow")selectedIcon = bitmapSnow;
  else if(weatherCondition == "Thunderstorm")selectedIcon = bitmapLightning;
  else if(weatherCondition == "Clouds")selectedIcon = bitmapCloud;


  // Finalizing drawings
  display.drawBitmap(2, 42, selectedIcon, 16, 16, 1);
  display.setCursor(25,46);
  display.print(weatherCondition);
  display.display();
}

// Filter so that we only grab the temp, weather, and city name from the Json
void filterWeather(String jsonResponse){
  // Filtering
  JsonDocument filter;
  filter["main"]["temp"] = true; // temp 
  filter["weather"][0]["main"] = true; // weather
  filter["name"] = true; // city

  // Doc to hold values
  JsonDocument doc;

  // Deserialize so we can turn into data
  DeserializationError error = deserializeJson(doc, jsonResponse, DeserializationOption::Filter(filter));

  if(error){
    Serial.print("filter failed: ");
    Serial.print(error.f_str());
    return;
  }

  // Grab and update values
  currentTemp = doc["main"]["temp"];  // temp is in main
  const char* weather = doc["weather"][0]["main"]; // weather is an array
  const char* cityName = doc["name"]; // City
  currentCity = String(cityName);
  currentCondition = String(weather);

}

unsigned long lastWeatherUpdate = 0; // 
const unsigned long weatherInterval = 600000; // 10 minutes

// ------------------------- Setup --------------------------------------
void setup() {
  Serial.begin(115200);
  delay(2000);
  connectWiFi();
  Wire.begin(D6, D7); // SDA, SCL
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Start up OLED
  dht.begin(); // Start up DHT
  timeClient.begin(); // Start up time
  timeClient.update();

  String weather = getWeather();
  Serial.println(weather);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // ----------------------- Display Setup ----------------------------------- //
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.display();
}

void loop() {

  unsigned long currentMillis = millis();

  // Update time
  timeClient.update();
  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();

  // turn time from military to standard
  bool isPM = (hours >= 12);

  if(hours > 12){
    hours -=  12;
  } else if (hours == 0){
    hours = 12;
  }

  // combine and format hours + minutes
  String hourStr = String(hours);
  String minuteStr = (minutes < 10) ? "0" + String(minutes) : String(minutes);
  String currentTime = hourStr + ":" + minuteStr + (isPM ? " PM" : " AM");
  
  // Update weather, 10 min intervals
  if (currentMillis - lastWeatherUpdate >= weatherInterval || lastWeatherUpdate == 0){
    lastWeatherUpdate = currentMillis;
    String rawJson = getWeather(); // grabs the Json value from website 
    if (rawJson != ""){
      filterWeather(rawJson); // Grab & update currentTemp and currentCondition
    }
  }
  
  // Refresh OLED
  screenDrawer(currentCondition, currentTemp, currentTime, currentCity);

  // small delay in case of OLED flicker
  delay(100); 
}
