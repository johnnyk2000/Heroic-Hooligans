#include "HackPublisher.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "AM232X.h"
#include <Adafruit_NeoPixel.h>
#include<math.h>
//#include <dhtnew.h>


// #define SCREEN_WIDTH 128 // OLED display width,  in pixels
// #define SCREEN_HEIGHT 64 // OLED display height, in pixels
// declare an SSD1306 display object connected to I2C


//DHTNEW tempHumSensor(39);


const char *ssid = "ASUS-F8";
const char *password = "K33pi7$@f3%";
const int gasDetectorAnalogPin = 34; // pins for gas detector
const int gasDetectorDigitalPin = 33;
//const int gasPin = 39;
const int trigPin = 13; // pins for ultrasonic
const int echoPin = 12;
const int neopixelPin = 26;
const int numPixels = 12;
const int neopixelButton = 17;
const int SCREEN_WIDTH   = 128;
const int SCREEN_HEIGHT = 64;

Adafruit_NeoPixel neopixel = Adafruit_NeoPixel(numPixels, neopixelPin, NEO_GRB + NEO_KHZ800);
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
HackPublisher publisher("HeroicHooligans");  // publisher instance for team "hackers"
AM232X AM2320;

int gasReading = 0;
float temperature = 0;
float humidity = 0;
float duration = 0;
float distance = 0;
bool npButtonState = false; // for ring led toggle
//string fire = ""; // or bool -- discuss with lyra

// hazard functions MAY NEED TO CHANGE VALUES AFTER TESTING AND CALIBRATION
bool isFire(const float& temp, const float& hum) {
  if (temp > 32.0 && hum < 45) {
    return true;
  }
  return false;
}
bool isGasLeak(const int& gas) {
  if (gas > 950) {
    return true;
  }
  return false;
}
bool isSafe (const int& temp, const float& hum, const int& gas) {
  if (temp < 29 && hum > 50 && gas < 900) {
    return true;
  }
  return false;
}
void oledPrintData() { // function to print data to oled
  oled.clearDisplay();
  oled.setCursor(0, 30);
  oled.print(temperature, 1);
  oled.setCursor(0, 20);
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.println("Temp");
  oled.setCursor(29, 20);
  oled.println("Hum");
  oled.setCursor(29, 30);
  oled.print(humidity, 1);
  oled.setCursor(59, 20);
  oled.println("Gas");
  oled.setCursor(59, 30);
  oled.print(gasReading);
  oled.setCursor(85, 20);
  oled.print("Dist");
  oled.setCursor(85, 30);
  oled.print(distance, 1);
  oled.display();

}

void neopixelOn() { // functions to toggle neopixel
    for(int i = 0; i < numPixels; i++) {
    neopixel.setPixelColor(i, neopixel.Color(255, 255, 255));
    neopixel.show();
    delay(50);
  }
}
void neopixelOff() {
    for(int i = 0; i < numPixels; i++) {
    neopixel.setPixelColor(i, neopixel.Color(0, 0, 0));
    neopixel.show();
    delay(50);
  }
}

void setup() {
  // put your setup code here, to run once:
  //setType(22);
  Serial.begin(115200);
  pinMode(gasDetectorAnalogPin, INPUT); // set gas detector pins
  pinMode(gasDetectorDigitalPin, INPUT);
  //pinMode(gasPin, INPUT);
  pinMode(trigPin, OUTPUT); // pin modes for ultrasonic
  pinMode(echoPin, INPUT); 
  pinMode(neopixelButton, INPUT_PULLUP);
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
  oled.clearDisplay();
  oled.display();

  if (!AM2320.begin()) {
    Serial.println("Temp / Hum sensor not found");
    while (1);
  }
  AM2320.wakeUp();

  neopixel.begin();

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



  int status = AM2320.read();
  if (status) { // if data is read, set the temperature and humidity
    temperature = AM2320.getTemperature();
    humidity = AM2320.getHumidity();
  }

  // ring led button toggle
  if(digitalRead(neopixelButton) == 0) { // if button gets pressed
    if(npButtonState == false){ // and if ring led is currently off, turn led on
      npButtonState = true; 
      neopixelOn(); 

    }
    else if(npButtonState == true) { // and if ring led is currently on, turn led off
      npButtonState = false;
      neopixelOff();
    }
  }



  oledPrintData();

  Serial.print("neopixel button: ");  // print data to serial monitor
  Serial.println(npButtonState);
  Serial.print("Gas: "); 
  Serial.println(gasReading);
  Serial.print("distance: ");
  Serial.println(distance);
  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Humidity ");
  Serial.println(humidity);
  Serial.println();

  publisher.store("gasReading", gasReading); // send data through blackbox
  publisher.store("distance", static_cast<int>(distance));
  publisher.store("temperature", static_cast<int>(temperature));
  publisher.store("humidity", static_cast<int>(humidity));
  publisher.send();

  delay(500);






















}
