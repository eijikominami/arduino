// ライブラリの読み込み
#include <Adafruit_NeoPixel.h>
#include <avr/power.h> // 電力制御
// テープLEDの数
#define NUMPIXELS 8
// ピンポート
const int gp2y0a710kPin  = 0; // アナログ
const int neoPixelPin    = 6;// デジタル
 
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, neoPixelPin, NEO_GRB + NEO_KHZ800);
 
void setup() {
  // put your setup code here, to run once:
   
  #if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
   
  pixels.begin();
}
 
void loop() {
  // put your main code here, to run repeatedly:
 
  // 距離の測定
  int GP2Y0A710K = getGP2Y0A710K(gp2y0a710kPin);
   
  // LEDの制御
  int green = GP2Y0A710K * 255 / 60;
  if(green < 0 || green > 255) green = 0;
  int red = (255-green)/2;
  ctrlLED(red, green, 0); // 距離が近づくほどより赤く点灯する
 
  delay(1000);
}
 
// 距離の測定
int getGP2Y0A710K(int pin){
  int a_in = analogRead(pin);
  return (6762/(a_in-9))-4;
}
 
// LEDの制御
void ctrlLED(int red, int green, int blue){
   
  for(int i=0;i<NUMPIXELS;i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(red,green,blue)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(100); // Delay for a period of time (in milliseconds).
  }
   
}
