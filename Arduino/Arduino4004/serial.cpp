#include <Arduino.h>
#include <Wire.h>

int startstring = 0;
char incomingByte = 0;


void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()>0){
      incomingByte = Serial.read();
      if ((incomingByte == '~') && startstring == 1)){
          startstring = 0;
          delay(5000);
      }
}
incomingByte = Serial.read();
Serial.print(val+"ioajsdfiojio");


}