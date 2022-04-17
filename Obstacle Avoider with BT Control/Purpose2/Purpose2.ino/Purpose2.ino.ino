#include <NewPing.h>
#include <AFMotor.h>
#include<Servo.h>

//AF_DCMotor Motor1(1);
//AF_DCMotor Motor2(2);
AF_DCMotor Motor3(3);
AF_DCMotor Motor4(2);

Servo blueservo;

#define TRIGGER_PIN  15
#define ECHO_PIN     14
#define MAX_DISTANCE 200

NewPing DistanceSensor(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

int rightDistance;
int leftDistance;
int pos=90;
int i=0;
int sum=0;
int p=0;
int samples[4]; 

void setup()
{
    //Serial.begin(9600);
    blueservo.attach(9);
    blueservo.write(pos);
    Forward();
    delay(5000);
}

void loop()
{
    unsigned int cm = DistanceSensor.ping_cm();
    int distance=average(cm);
    //Serial.print("Distance: ");
    //Serial.print(cm);
    //Serial.println("cm");
    //delay(500);
    if(distance<=20){
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
      
      if (leftDistance > rightDistance){
        Left();
        delay(500); // Change the time to make 90 deg. turn
      }
      else if (leftDistance < rightDistance){
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
   Forward();   
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

int average(int dist){
  int avg;
    if(i>2){
    sum=sum-samples[p];
    samples[p]=dist;
    sum=sum+samples[p]; 
    p++;         
    avg=sum/3;}
     
      
   if(i<3){
      samples[p]=dist;
      avg=dist;
      sum=sum+samples[p];
      i++;
      p++;
      }

    if(p>2){
      p=0;
      }
            
    Serial.print(" Sum: ");
    Serial.print(sum);
    Serial.print(" ||Dist: ");
    Serial.print(dist);
    Serial.print(" Avg: ");
    Serial.println(avg);            
  return avg;
  }    

