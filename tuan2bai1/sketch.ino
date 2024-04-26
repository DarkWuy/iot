#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  Serial.println("Connecting to WiFi");
  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Connected!");

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("https://api.openweathermap.org/data/2.5/weather?id=1562693&appid=f58244e146cbb127a4ba9d8df4e44338");
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();

      // Parse JSON
      StaticJsonDocument
      <200> doc;
      deserializeJson(doc, payload);

      // Extract weather information
      JsonObject main = doc["main"];
      double temp = main["temp"];
      temp -= 273.15; // Convert temperature from Kelvin to Celsius

      // Display weather information on OLED
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("Nhiet do: ");
      display.print(temp);
      display.println(" do C");
      display.display();
    }
    else {
      Serial.println("Error on HTTP request");
    }
    http.end();
  }
  delay(10000);
}
