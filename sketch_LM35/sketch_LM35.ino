// Analog
const int lm35Pin        = 1;

void setup() {
  // put your setup code here, to run once:
  
  // シリアル通信速度
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(getLM35(lm35Pin)); 
  delay(1000);
}

float getLM35(int pin){
  float a_in = analogRead(pin);
  return 5*a_in/1024*100;
}


