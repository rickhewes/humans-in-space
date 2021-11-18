#include "HCuOLED.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>
#include <WiFiManager.h>

#include "ArduinoJson.h"

ESP8266WiFiMulti WiFiMulti;

/* Include the standard wire library */
#include <Wire.h>

const PROGMEM byte Earth[4][72]= {{
0x80, 0x60, 0x10, 0x08, 0x04, 0x04, 0x82, 0xc2, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x41,
  0x81, 0x3a, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xfe, 0x01, 0x00, 0x80,
  0x81, 0x42, 0x3c, 0x00, 0x00, 0x02, 0x85, 0x42, 0x40, 0x41, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xff,
  0x00, 0x03, 0x0c, 0x10, 0x20, 0x21, 0x43, 0x44, 0x88, 0x88, 0x88, 0x8c, 0x8a, 0x89, 0x88, 0x84,
  0x82, 0x41, 0x20, 0x20, 0x10, 0x08, 0x06, 0x01
}, {
0x00, 0xc0, 0x30, 0x08, 0x04, 0x04, 0x82, 0xc2, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x41,
  0x81, 0x02, 0x02, 0x04, 0x08, 0x30, 0xc0, 0x00, 0xff, 0x00, 0x00, 0x00, 0xfe, 0x01, 0x00, 0x80,
  0x81, 0x42, 0x3c, 0x00, 0x00, 0x02, 0x85, 0x42, 0x40, 0x41, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xff,
  0x01, 0x02, 0x0c, 0x10, 0x20, 0x41, 0x43, 0x84, 0x88, 0x88, 0x88, 0x8c, 0x8a, 0x89, 0x88, 0x84,
  0x02, 0x1d, 0x22, 0x22, 0x22, 0x1c, 0x02, 0x01
}, {
0x80, 0x60, 0x10, 0x08, 0x04, 0x04, 0x82, 0xc1, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x41,
  0x82, 0x02, 0x04, 0x04, 0x08, 0x30, 0xc0, 0x00, 0xff, 0x00, 0x00, 0x00, 0xfe, 0x01, 0x00, 0x80,
  0x81, 0x42, 0x3c, 0x00, 0x00, 0x02, 0x85, 0x42, 0x40, 0x41, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xff,
  0x00, 0x00, 0x1c, 0x22, 0x22, 0x21, 0x5b, 0x84, 0x88, 0x88, 0x88, 0x8c, 0x8a, 0x89, 0x88, 0x84,
  0x42, 0x41, 0x20, 0x20, 0x10, 0x08, 0x06, 0x01
}, {
0x80, 0x40, 0x38, 0x44, 0x44, 0x44, 0xb8, 0xc0, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x41,
  0x81, 0x02, 0x02, 0x04, 0x08, 0x30, 0x40, 0x80, 0xff, 0x00, 0x00, 0x00, 0xfe, 0x01, 0x00, 0x80,
  0x81, 0x42, 0x3c, 0x00, 0x00, 0x02, 0x85, 0x42, 0x40, 0x41, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xff,
  0x00, 0x03, 0x0c, 0x10, 0x20, 0x41, 0x43, 0x84, 0x88, 0x88, 0x88, 0x8c, 0x8a, 0x89, 0x88, 0x84,
  0x42, 0x41, 0x20, 0x20, 0x10, 0x0c, 0x03, 0x00
}};


/* I2C address of the display */
#define I2C_ADD 0x3C

/* Create an instance of the library for the WeMos D1 OLED shield */
HCuOLED HCuOLED(WEMOS_D1_MINI_OLED, I2C_ADD);

DynamicJsonDocument doc(1024);

unsigned int loopCounter = 0;
long numberInSpace = 0;
String astronautText = "??";
String craftText = "??";

void setup()
{
  /* Initialise the I2C wire library. For WeMos D1 I2C port is on pins D4 & D5 */
  Wire.begin(4, 5);

  /* Reset the display */
  HCuOLED.Reset();
  HCuOLED.SetFont(sharpsharp_5pt);
  HCuOLED.Cursor(0,0);
  HCuOLED.Print("   WiFi setup...");
  HCuOLED.Cursor(20,15);
  HCuOLED.Bitmap(24, 3, Earth[0]);
  HCuOLED.Refresh();

  WiFi.mode(WIFI_STA);
  WiFiManager wifiManager;
  wifiManager.autoConnect("HumansInSpace");

  HCuOLED.ClearBuffer();
  HCuOLED.SetFont(sharpsharp_5pt);
  HCuOLED.Cursor(0,0);
  HCuOLED.Print("   Initialising...");
  HCuOLED.Cursor(20,15);
  HCuOLED.Bitmap(24, 3, Earth[0]);

  /* Write the display buffer to the screen */
  HCuOLED.Refresh();
  for (uint8_t t = 4; t > 0; t--) {
    delay(1000);
  }
}

void loop()
{
  if (loopCounter % 3600 == 0 || numberInSpace == 0) {
      if ((WiFiMulti.run() == WL_CONNECTED)) {
        WiFiClient client;
        HTTPClient http;

        if (http.begin(client, "http://api.open-notify.org/astros.json")) {

          int httpCode = http.GET();

          if (httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            deserializeJson(doc, payload);
            numberInSpace = doc["number"];
        }
      }
    }
  }

  if (numberInSpace > 0) {
    long astronautNumber = loopCounter % numberInSpace;

    astronautText = String((const char *) doc["people"][astronautNumber]["name"]);
    craftText = String((const char *) doc["people"][astronautNumber]["craft"]);
  }

  updateDisplay(loopCounter);

  delay(10000);
  loopCounter++;
}

void updateDisplay(unsigned int loopCounter)
{
  HCuOLED.ClearBuffer();

  HCuOLED.SetFont(sharpsharp_5pt);
  HCuOLED.Cursor(0,2);
  HCuOLED.Print("In space:");

  HCuOLED.SetFont(MedProp_11pt);
  HCuOLED.Cursor(8,9);
  HCuOLED.Print(numberInSpace);

  HCuOLED.SetFont(sharpsharp_5pt);
  HCuOLED.Cursor(0, 32);
  HCuOLED.Print(astronautText.c_str());

  HCuOLED.SetFont(sharpsharp_6pt);
  HCuOLED.Cursor(0, 41);
  HCuOLED.Print(craftText.c_str());

  drawEarth(loopCounter % 4);

  HCuOLED.Refresh();
}

void drawEarth(unsigned int image)
{
  HCuOLED.Cursor(40,0);
  HCuOLED.Bitmap(24, 3, Earth[image]);
}
