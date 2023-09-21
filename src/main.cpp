#include <Arduino.h>
#include "DHT.h"
#include <U8g2lib.h>
#include <Wire.h>
#include <ESP8266WiFi.h>

#define DHT_TYPE DHT22
#define DHT_PIN D7

DHT dht(DHT_PIN, DHT_TYPE);

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, SCL, SDA, U8X8_PIN_NONE);

void scrollData (float temperature, float humidity)
{
  char temperatureStr[10];
  char humidityStr[10];

  u8g2.setFont(u8g2_font_inb30_mr);

  dtostrf(temperature, 0, 2, temperatureStr);
  
  u8g2_uint_t tempSize = u8g2.getUTF8Width(temperatureStr);
  dtostrf(humidity, 0, 2, humidityStr);

  u8g2_uint_t humSize = u8g2.getUTF8Width(humidityStr);

  u8g2_uint_t x = 0;
  u8g2_uint_t offset = 0;

  // tempSize + space + circle size + space + humidity size + percentage size + space (30 approximately)
  u8g2_uint_t totalWidth = tempSize + 5 + 3 + 20 + humSize + 40;
  do {
    u8g2.firstPage();
    do {
      x = offset;
      u8g2.setFont(u8g2_font_inb30_mr);
      do {
        u8g2.drawUTF8(x, 45, temperatureStr);
        u8g2.drawCircle(x + tempSize + 5, 16, 3, U8G2_DRAW_ALL);

        u8g2.drawUTF8(x + tempSize + 5 + 3 + 20, 45, humidityStr);
        u8g2.drawUTF8(x + tempSize + 5 + 3 + 20 + humSize, 45, "%");


        x += tempSize + 5 + 3 + 20 + humSize + 40;
      } while (x < u8g2.getDisplayWidth());
    } while (u8g2.nextPage());

    offset-=5;
    delay(2);
  } while ( (u8g2_uint_t) offset >  (u8g2_uint_t) -(totalWidth));
}

void swapData(float temperature, float humidity)
{
  u8g2.setFont(u8g2_font_logisoso32_tf);

  char buffer[10];

  // clear the screen
  u8g2.clearBuffer();
  
  // draw temperature
  dtostrf(temperature, 0, 2, buffer);
  u8g2.drawStr(0, 45, buffer);

  // draw degree sign
  u8g2_uint_t tempSize = u8g2.getStrWidth(buffer);
  u8g2.drawCircle(0 + tempSize + 5, 16, 3, U8G2_DRAW_ALL);
  
  u8g2.sendBuffer();

  // Wait for 10 seconds
  delay(10000);
  
  // clear the screen
  u8g2.clearBuffer();
  
  // draw humidity
  dtostrf(humidity, 0, 2, buffer);
  u8g2_uint_t humSize = u8g2.getStrWidth(buffer);
  u8g2.drawStr(0, 45, buffer);

  // draw percentage sign
  u8g2.drawStr(0 + humSize + 5, 45, "%");

  u8g2.sendBuffer();

  // Wait for 10 seconds
  delay(10000);
}

void showAllData(float temperature, float humidity)
{
  u8g2.setFont(u8g2_font_inr19_mf);

  char temperatureStr[10];
  char humidityStr[10];

  // clear the screen
  u8g2.clearBuffer();
  
  // draw temperature
  dtostrf(temperature, 0, 2, temperatureStr);
  u8g2.drawStr(10, 25, temperatureStr);

  // draw degree sign
  u8g2_uint_t tempSize = u8g2.getStrWidth(temperatureStr);
  u8g2.drawCircle(10 + tempSize + 5, 10, 3, U8G2_DRAW_ALL);
   
  // draw humidity
  dtostrf(humidity, 0, 2, humidityStr);
  u8g2_uint_t humSize = u8g2.getStrWidth(humidityStr);
  u8g2.drawStr(10, 55, humidityStr);

  // draw percentage sign
  u8g2.drawStr(10 + humSize + 5, 55, "%");

  u8g2.sendBuffer();

  // Wait for 10 seconds
  delay(10000);
}

void setup()
{
  WiFi.mode(WIFI_OFF);
  dht.begin();
  u8g2.begin();
  // u8g2.setFont(u8g2_font_inr16_mf);
  // u8g2.setFont(u8g2_font_logisoso32_tf);
  u8g2.setFontMode(0);
}

void loop()
{
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  showAllData(temperature, humidity);
  swapData(temperature, humidity);
  scrollData(temperature, humidity);
}