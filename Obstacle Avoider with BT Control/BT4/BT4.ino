// include the SoftwareSerial library so you can use its functions:
#include <SoftwareSerial.h>

#define rxPin 11
#define txPin 12

// set up a new serial port
SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);

void setup()  {
  // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  pinMode(13,OUTPUT);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
   Serial.begin(9600);
}

void loop() {
  if (mySerial.available()>0){
     char c= mySerial.read();
     Serial.print(c);
  }
    if (Serial.available()>0){
     char c= Serial.read();
     mySerial.println(c);
     }
}
