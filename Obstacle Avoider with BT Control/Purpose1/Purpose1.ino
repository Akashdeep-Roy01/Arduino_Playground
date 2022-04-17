/*Author AkRoy
 * Date 31/03/2017
 * Program for android controlled arduino robot using HC05 module and
 * Adafruit motor Shield(DK Electronics)
 */

#include<SoftwareSerial.h>
#define Rx 11//define Rx and Tx pins of HC05
#define Tx 12
SoftwareSerial mySerial = SoftwareSerial(Rx,Tx);//Create an object mySerial

#include <AFMotor.h>

AF_DCMotor Motor1(1);//Create motor objects
AF_DCMotor Motor2(2);
AF_DCMotor Motor3(3);
AF_DCMotor Motor4(2);


void setup() {
    pinMode(Rx,INPUT);
    pinMode(Tx,OUTPUT);
    //Serial.begin(9600);
    mySerial.begin(9600);
}

void loop() {
    if(mySerial.available()>0){
        char command=mySerial.read();
        if(command=='F'){
            Forward();
         }
        if(command=='S'){
            Brake();
        }
        if(command=='L'){
            Left();
         }
        if(command=='R'){
            Right();
        }
        if(command=='B'){
            Backward();
        }
    
    }

}

void Forward(){
  //FORWARD
    Motor1.setSpeed(100);
    Motor2.setSpeed(100);
    Motor3.setSpeed(100);
    Motor4.setSpeed(100);
    Motor1.run(FORWARD);
    Motor2.run(FORWARD);
    Motor3.run(FORWARD);
    Motor4.run(FORWARD);
}

void Brake(){
  //BRAKE
    Motor1.setSpeed(0);
    Motor2.setSpeed(0);
    Motor3.setSpeed(0);
    Motor4.setSpeed(0);
    Motor1.run(RELEASE);
    Motor2.run(RELEASE);
    Motor3.run(RELEASE);
    Motor4.run(RELEASE);
  }

void Left(){
    Motor1.setSpeed(0);
    Motor2.setSpeed(0);
    Motor3.setSpeed(100);
    Motor4.setSpeed(0);
    Motor1.run(BRAKE);
    Motor2.run(BRAKE);
    Motor3.run(FORWARD);
    Motor4.run(BRAKE);
    }

void Right(){
    Motor1.setSpeed(25);
    Motor2.setSpeed(25);
    Motor3.setSpeed(0);
    Motor4.setSpeed(100);
    Motor1.run(FORWARD);
    Motor2.run(FORWARD);
    Motor3.run(BRAKE);
    Motor4.run(FORWARD);
}

void Backward(){
    Motor1.setSpeed(2);
    Motor2.setSpeed(100);
    Motor3.setSpeed(100);
    Motor4.setSpeed(100);
    Motor1.run(BACKWARD);
    Motor2.run(BACKWARD);
    Motor3.run(BACKWARD);
    Motor4.run(BACKWARD); 
}

