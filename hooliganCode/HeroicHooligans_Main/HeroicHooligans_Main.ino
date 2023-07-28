#include "HackPublisher.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


const int gasDetector = 0;



void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  pinMode(gasDetector, INPUT);

  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }
  oled.clearDisplay();
  oled.display();
















}

void loop() {
  // put your main code here, to run repeatedly:


  int gasReading = analogRead(gasDetector);
  Serial.print("Gas reading: ");
  Serial.println(gasReading);
  delay(100);

  oled.setCursor(30, 30);
  oled.drawRect(32, 32, 30, 30, WHITE);
  oled.println("Hello");
  oled.display();
  

























}
