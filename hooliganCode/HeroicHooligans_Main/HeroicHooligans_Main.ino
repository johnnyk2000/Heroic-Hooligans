#include "HackPublisher.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "AM232X.h"


#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


const char *ssid = "ASUS-F8";
const char *password = "K33pi7$@f3%";

HackPublisher publisher("HeroicHooligans");  // publisher instance for team "hackers"

const int gasDetectorAnalogPin = 26; // pins for gas detector
const int gasDetectorDigitalPin = 33;
const int trigPin = 13; // pins for ultrasonic
const int echoPin = 12;

AM232X AM2320;
float gasReading = 0;
float temperature = 0;
float humidity = 0;
long duration = 0;
long distance = 0;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  pinMode(gasDetectorAnalogPin, INPUT); // set gas detector pins
  pinMode(gasDetectorDigitalPin, INPUT);
  pinMode(trigPin, OUTPUT); // pin modes for ultrasonic
  pinMode(echoPin, INPUT); 
  // while (!Serial) continue;
  // // Connect to wifi
  // WiFi.begin(ssid);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }

  //// Initialize publisher
  //publisher.begin();

  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // check is oled is connected
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

    Serial.println(__FILE__); // check if temp / hum sensor is connected
  Serial.print("LIBRARY VERSION: ");
  Serial.println(AM232X_LIB_VERSION);
  Serial.println();
  //  if (! AM2320.begin() )
  // {
  //   Serial.println("Temp / Hum sensor not found");
  //   while (1);
  // }
  // AM2320.wakeUp();

  oled.clearDisplay();
  oled.display();








Serial.println("going into loop");






}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trigPin, LOW); // clear trigpin
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); // set trigpin to high for 10 microsec
  delayMicroseconds(10);
  duration = pulseIn(echoPin, HIGH); // sound wave travel time in microseconds
  distance = (3.43 * pow(10, -2) * duration) / 2; // distance in cm
  Serial.print("distance: ");
  Serial.println(distance);

  //gasReading = analogRead(gasDetectorAnalogPin);
  // temperature = AM2320.getTemperature();
  // humidity = AM2320.getHumidity();
  // Serial.print("Gas reading: ");
  // Serial.println(gasReading);
  // Serial.print("Temperature: ");
  // Serial.println(temperature);
  // Serial.print("Humidity: ");
  // Serial.println(humidity);
  delay(300);

  // oled.setCursor(30, 30);
  // oled.drawRect(32, 32, 30, 30, WHITE);
  // oled.println("Hello");
  // oled.display();
  

























}
