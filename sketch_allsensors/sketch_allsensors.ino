// Timer
#include <Metro.h>
// DHT
#include <DHT.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
// JSON
#include <ArduinoJson.h>
// Bluetooth
#include <SPP.h>
#include <usbhub.h>
// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

// LED
#define NUMPIXELS 8
// DHT
#define DHTPIN 4
#define DHTTYPE DHT11

// Analog
const int gp2y0a710kPin  = 0;
const int lm35Pin        = 1;
const int m045231Pin     = 2;
const int m045232Pin     = 3;
// Digital
const int se10Pin        = 22;
const int dht11Pin       = 24;
const int neoPixelPin    = 26;
const int amn31112Pin    = 28;
const int dfr0028Pin     = 30;
const int ledPin = 13;

// Bluetooth
USB Usb;
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
SPP SerialBT(&Btd); 

int delayval = 100;

Metro metro = Metro(1000);

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, neoPixelPin, NEO_GRB + NEO_KHZ800);
DHT dht(dht11Pin, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  
  #if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  
  pixels.begin();
  dht.begin();
  
  // シリアル通信速度
  Serial.begin(9600);
  
  if (Usb.Init() == -1) {
    while (1); //halt
  }
  
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int GP2Y0A710K = getGP2Y0A710K(gp2y0a710kPin);
  
  // LED
  int green = GP2Y0A710K * 255 / 60;
  if(green < 0 || green > 255) green = 0;
  int red = (255-green)/2;
  ctrlLED(red, green, 0);
  
  // JSON
  String json;
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["detect_1"]      = getSE10(se10Pin);
  root["detect_2"]      = getAMN31112(amn31112Pin);
  root["distance"]      = GP2Y0A710K;
  root["humidity"]      = dht.readHumidity();
  root["temparture_1"]  = dht.readTemperature();
  root["temparture_2"]  = getLM35(lm35Pin);
  root["tilt"]          = getDFR0028(dfr0028Pin);
  root["gyro_1"]        = analogRead(m045231Pin);
  root["gyro_2"]        = analogRead(m045232Pin);
  root.printTo(json);
  
  // Bluetooth
  Usb.Task(); // The SPP data is actually not send until this is called, one could call SerialBT.send() directly as well
  
  // 定期実行
  if(metro.check() == 1){
    // シリアル送信
    Serial.println(json); 
    if (SerialBT.connected) {
    //if (SerialBT.connected && SerialBT.available()) {
      // Bluetooth送信
      SerialBT.println(json);
    }
  }
  //delay(1000);
}

int getSE10(int pin){
  if (digitalRead(pin) == LOW) {
    //digitalWrite(ledPin, HIGH);
    return 1;
  } else {
    //digitalWrite(ledPin, LOW);
    return 0;
  }
}

int getAMN31112(int pin){
    if (digitalRead(pin) == HIGH) {
    //digitalWrite(ledPin, HIGH);
    return 1;
  } else {
    //digitalWrite(ledPin, LOW);
    return 0;
  }
}

int getDFR0028(int pin){
  if (digitalRead(pin) == HIGH) {
    //digitalWrite(ledPin, HIGH);
    return 1;
  } else {
    //digitalWrite(ledPin, LOW);
    return 0;
  }
}

int getGP2Y0A710K(int pin){
  int a_in = analogRead(pin);
  return (6762/(a_in-9))-4;
}

float getLM35(int pin){
  float a_in = analogRead(pin);
  return 5*a_in/1024*100;
}

void ctrlLED(int red, int green, int blue){
  
  for(int i=0;i<NUMPIXELS;i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(red,green,blue)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    //delay(delayval); // Delay for a period of time (in milliseconds).
  }
  
}


