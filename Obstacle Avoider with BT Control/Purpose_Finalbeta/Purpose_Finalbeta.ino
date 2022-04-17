/***Author AkRoy
 * Date June 5, 2017
 * Program for bluetooth controlled and autonomous obstacle avoider robot
 * */

 #include<AFMotor.h>
 #include<Servo.h>
 #include<NewPing.h>
 #include<SoftwareSerial.h>
 
 #define Rx 11//define Rx and Tx pins of HC05
 #define Tx 12
 #define TRIGGER_PIN  15
 #define ECHO_PIN     14
 #define MAX_DISTANCE 200
 //#define rightSW
 //#define leftSW
 
 //AF_DCMotor Motor1(1);
 //AF_DCMotor Motor2(2);
 AF_DCMotor Motor3(3);
 AF_DCMotor Motor4(2);
 Servo blueservo;
 NewPing DistanceSensor(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
 SoftwareSerial mySerial = SoftwareSerial(Rx,Tx);//Create an object mySerial

 int rightDistance;
 int leftDistance;
 int pos=90;
 //int rightSWincomingData;
 //int leftSWincomingData;
 //int incomingData; 
   char mode = 'm';

void setup()
{   pinMode(Rx,INPUT);
    pinMode(Tx,OUTPUT);
    //pinMode(leftSW,  INPUT_PULLUP);
    //pinMode(rightSW, INPUT_PULLUP);
    mySerial.begin(9600);
    //Serial.begin(9600);
    blueservo.attach(9);
    blueservo.write(pos);
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
      
      else{
      int cm = DistanceSensor.ping_cm();
    //Serial.print("Distance: ");
    //Serial.print(cm);
    //Serial.println("cm");
    //delay(500);
        if(cm<=20){
            Brake();
            for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
                blueservo.write(pos);               // tell servo to go to position in variable 'pos'
                delay(5);                         // waits 15ms for the servo to reach the position
             }
      //Read new distance from the right side
          leftDistance = DistanceSensor.ping_cm();
          delay(100);
      //Now look left
          for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
              blueservo.write(pos);               // tell servo to go to position in variable 'pos'
              delay(5);                         // waits 15ms for the servo to reach the position
           }
      //Read new distance from the left side
          rightDistance = DistanceSensor.ping_cm();
          delay(100);
          pos = 90; // look forward again
          blueservo.write(pos);  
      
          if(leftDistance > rightDistance){
              Left();
              delay(500); // Change the time to make 90 deg. turn
          }
          else if(leftDistance < rightDistance){
             Right();
             delay(500);
          }
          else{ //that means that two distances are equal
            Backward();
            delay(1000);// Go back for 1 sec 
            Left();   // and turn left
            delay(500);
         }           
      }  
      else{
        /**leftSWState = digitalRead(leftSW);
    rightSWState = digitalRead(rightSW);
    
    //Check for "contact"
    if (leftSWState == LOW){
      backward();
      delay(500);
      right();
      delay(400);
    }
    if (rightSWState == LOW){
      backward();
      delay(500);
      left();
      delay(400);
    }*/
    Forward();
    } 
      }
    }

    
 void Forward(){
  //FORWARD
    //Motor1.setSpeed(200);
    //Motor2.setSpeed(255);
    Motor3.setSpeed(100);
    Motor4.setSpeed(100);
    //Motor1.run(FORWARD);
    //Motor2.run(FORWARD);
    Motor3.run(FORWARD);
    Motor4.run(FORWARD);
}

 
void Brake(){
  //BRAKE
    //Motor1.setSpeed(0);
    //Motor2.setSpeed(0);
    Motor3.setSpeed(0);
    Motor4.setSpeed(0);
    //Motor1.run(RELEASE);
    //Motor2.run(RELEASE);
    Motor3.run(RELEASE);
    Motor4.run(RELEASE);
  }

  void Right(){
    //Motor1.setSpeed(255);
    //Motor2.setSpeed(0);
    Motor3.setSpeed(0);
    Motor4.setSpeed(100);
    //Motor1.run(FORWARD);
    //Motor2.run(BRAKE);
    Motor4.run(FORWARD);
    Motor3.run(BRAKE);
    }

void Backward(){
  //BACKWARD
    //Motor1.setSpeed(200);
    //Motor2.setSpeed(255);
    Motor3.setSpeed(100);
    Motor4.setSpeed(100);
    //Motor1.run(BACKWARD);
    //Motor2.run(FORWARD);
    Motor3.run(BACKWARD);
    Motor4.run(BACKWARD);
}

void Left(){
    //Motor1.setSpeed(255);
    //Motor2.setSpeed(0);
    Motor3.setSpeed(100);
    Motor4.setSpeed(0);
    //Motor4.run(FORWARD);
    //Motor2.run(BRAKE);
    Motor3.run(FORWARD);
    Motor4.run(BRAKE);
    }

