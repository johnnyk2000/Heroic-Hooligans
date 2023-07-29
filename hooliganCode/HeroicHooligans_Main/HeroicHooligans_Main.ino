#include "HackPublisher.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "AM232X.h"


#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// declare an SSD1306 display object connected to I2C

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
HackPublisher publisher("HeroicHooligans");  // publisher instance for team "hackers"
AM232X AM2320;

const char *ssid = "ASUS-F8";
const char *password = "K33pi7$@f3%";
const int gasDetectorAnalogPin = 34; // pins for gas detector
const int gasDetectorDigitalPin = 33;
const int trigPin = 13; // pins for ultrasonic
const int echoPin = 12;

int gasReading = 0;
float temperature = 0;
float humidity = 0;
float duration = 0;
float distance = 0;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  pinMode(gasDetectorAnalogPin, INPUT); // set gas detector pins
  pinMode(gasDetectorDigitalPin, INPUT);
  pinMode(trigPin, OUTPUT); // pin modes for ultrasonic
  pinMode(echoPin, INPUT); 
  while (!Serial) continue;
  // Connect to wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Initialize publisher
  publisher.begin();

  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // check is oled is connected
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }


   if (! AM2320.begin() )
  {
    Serial.println("Temp / Hum sensor not found");
    while (1);
  }
  AM2320.wakeUp();

  oled.clearDisplay();
  oled.display();








Serial.println("going into loop");






}

void loop() {
  // put your main code here, to run repeatedly:

  // gas module
  gasReading = analogRead(gasDetectorAnalogPin);

  // temperature and humidity
  digitalWrite(trigPin, LOW); // clear trigpin
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); // set trigpin to high for 10 microsec
  delayMicroseconds(10);
  duration = pulseIn(echoPin, HIGH); // sound wave travel time in microseconds
  distance = (3.43 * pow(10, -2) * duration) / 2; // distance in cm


  // temperature = AM2320.getTemperature();
  // humidity = AM2320.getHumidity();
  // Serial.print("Gas reading: ");
  // Serial.println(gasReading);
  // Serial.print("Temperature: ");
  // Serial.println(temperature);
  // Serial.print("Humidity: ");
  // Serial.println(humidity);
  Serial.print("AM2320\t");
  int status = AM2320.read();
  if (status) { // if data is read, set the temperature and humidity
    temperature = AM2320.getTemperature();
    humidity = AM2320.getHumidity();
  }



  // oled.setCursor(30, 30);
  // oled.drawRect(32, 32, 30, 30, WHITE);
  // oled.println("Hello");
  // oled.display();
  

  Serial.print("Gas: ");
  Serial.println(gasReading);
  Serial.print("distance: ");
  Serial.println(distance);
  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Humidity ");
  Serial.println(humidity);
  Serial.println();

  delay(300);






















}
