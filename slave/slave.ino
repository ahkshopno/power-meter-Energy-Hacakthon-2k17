#include <Wire.h>
String data;
char buf[20];
char swi[4]="0000";
void setup() {
  Serial.begin(9600);
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  Wire.onReceive(receiveEvent); // register event
}

//map(analogRead(0), 0, 1023, 0, 999)





void loop() {
  data=String(map(analogRead(0), 0, 1023, 0, 999), DEC)+String(map(analogRead(0), 0, 1023, 0, 999), DEC)+String(digitalRead(4), DEC)+String(digitalRead(5), DEC);
  data.toCharArray(buf, data.length()+1);
  Serial.print("buf=");
  Serial.println(buf);
  delay(500);
  digitalWrite(4,swi[1]-48);
  digitalWrite(5,swi[3]-48);
}


void requestEvent() {
  Wire.write(buf); // respond with message of 6 bytes
  // as expected by master
}


void receiveEvent(int howMany) {
  int i=0;
  while (Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
    swi[i]=c;
    i++;
    }
 }
