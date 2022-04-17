#include "EasyNextionLibrary.h"  // Include EasyNextionLibrary 

EasyNex myNex(Serial1); // Create an object of EasyNex class with the name < myNex >
                       // Set as parameter the Hardware Serial you are going to use
uint32_t number = 0;
uint32_t number1 = 0;
uint32_t lastnumber = 0;

#define LOOP_TIME 2000
unsigned long timer ;

void setup(){
  Serial.begin(9600);
  myNex.begin(9600); // Begin the object with a baud rate of 9600
                     // If no parameter was given in the begin(), the default baud rate of 9600 will be used
  delay(500);        // Wait for Nextion to start
  
  timer = millis();
}

void loop(){

  if((millis() - timer) > LOOP_TIME){
      Serial1.print("add 3,0,"+String(10));
      Serial1.write(0xff);
      Serial1.write(0xff);
      Serial1.write(0xff);
      Serial1.print("add 1,0,"+String(15));
      Serial1.write(0xff);
      Serial1.write(0xff);
      Serial1.write(0xff);
      Serial1.print("add 2,0,"+String(25));
      Serial1.write(0xff);
      Serial1.write(0xff);
      Serial1.write(0xff);
      int BPM = myNex.readNumber("page7.n0.val");
      int PIP = myNex.readNumber("page7.n1.val");
      int PEEP = myNex.readNumber("page7.n2.val");
      int VTidal = myNex.readNumber("page7.n3.val");
      int FiO2 = myNex.readNumber("page7.n4.val");
      Serial.print(BPM);
      Serial.print(PIP);
      Serial.print(PEEP);
      Serial.print(VTidal);
      Serial.println(FiO2);
      }
}
