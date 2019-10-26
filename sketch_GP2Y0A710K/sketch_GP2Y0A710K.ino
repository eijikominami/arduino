void setup() {
  // put your setup code here, to run once:
  // シリアル通信速度
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int a_in;
  int distance;
  a_in = analogRead(0);
  distance = (6762/(a_in-9))-4;
  Serial.println(distance);
  
  delay(1000);
}
