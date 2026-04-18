# ESP8266 WiFi Weather Station

An IoT weather station built on a Wemos D1 Mini (ESP8266) that displays real-time NTP time and live outdoor weather data on an SSD1306 OLED screen. Weather data is pulled from the OpenWeatherMap API and refreshes every 10 minutes.

---

## Features

- 12-hour NTP clock with AM/PM indicator
- Outdoor temperature pulled from OpenWeatherMap (°F)
- Current weather condition with matching bitmap icon (sunny, cloudy, rain, snow, thunderstorm)
- City name display
- Weather auto-refreshes every 10 minutes
- Automatic WiFi reconnection if connection is lost

---

## Hardware

| Component | Details |
|---|---|
| Microcontroller | Wemos D1 Mini (ESP8266) |
| Display | SSD1306 128x64 OLED |

**Wiring:**
- SDA → D6
- SCL → D7
---

## Libraries Required

Install the following libraries via the Arduino Library Manager:

- `ArduinoJson` (v7+)
- `NTPClient`
- `Adafruit SSD1306`
- `Adafruit GFX`
- `ESP8266WiFi` (included with ESP8266 board package)
- `ESP8266HTTPClient` (included with ESP8266 board package)

---

## Setup

1. Clone or download this repository
2. Open the `.ino` file in Arduino IDE
3. Install all required libraries listed above
4. Fill in your credentials in the Personal section of the sketch (see Configuration below)
5. Select **LOLIN(WEMOS) D1 R2 & mini** as your board
6. Load onto your D1 Mini

---

## Configuration

Find this block near the top of the sketch and fill in your own values:

```cpp
const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* apiKey   = "YOUR_OPENWEATHERMAP_API_KEY";
const char* city     = "Mankato,MN,US"; // Change to your city
```

An OpenWeatherMap API key can be obtained for free at [openweathermap.org](https://openweathermap.org/api).

> **Note:** City names longer than ~8 characters may overlap with the time display on the OLED. 
> Short city names like "Mankato" work best.If your city name is long, consider using a 
> nearby shorter-named city that shares the same weather data.

