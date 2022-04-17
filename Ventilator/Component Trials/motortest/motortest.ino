#include <AFMotor.h>

AF_DCMotor Motor1(1);
AF_DCMotor Motor2(2);
AF_DCMotor Motor3(3);
AF_DCMotor Motor4(4);

void setup()
{
}

void loop()
{
  //FORWARD
    Motor1.setSpeed(50);
    Motor2.setSpeed(50);
    Motor3.setSpeed(100);
    Motor4.setSpeed(50);
    Motor1.run(FORWARD);
    Motor2.run(FORWARD);
    delay(3000);
    Motor3.run(FORWARD);
    delay(1000);
    Motor4.run(FORWARD);
    delay(3000);
 //BRAKE
    Motor1.setSpeed(0);
    Motor2.setSpeed(0);
    Motor3.setSpeed(0);
    Motor4.setSpeed(0);
    Motor1.run(RELEASE);
    Motor2.run(RELEASE);
    Motor3.run(RELEASE);
    Motor4.run(RELEASE);
    delay(3000);
 //BACKWARD
    Motor1.setSpeed(50);
    Motor2.setSpeed(50);
    Motor3.setSpeed(100);
    Motor4.setSpeed(50);
    Motor1.run(BACKWARD);
    Motor2.run(BACKWARD);
    delay(3000);
    Motor3.run(BACKWARD);
    delay(1000);
    Motor4.run(BACKWARD);
    delay(3000);
//RIGHT
  /*  Motor1.setSpeed(255);
    Motor2.setSpeed(255);
    Motor3.setSpeed(0);
    Motor4.setSpeed(0);
    Motor1.run(FORWARD);
    Motor2.run(FORWARD);
    Motor3.run(BRAKE);
    Motor4.run(BRAKE);
    delay(2000);
//LEFT
    Motor1.setSpeed(0);
    Motor2.setSpeed(0);
    Motor3.setSpeed(255);
    Motor4.setSpeed(255);
    Motor1.run(BRAKE);
    Motor2.run(BRAKE);
    Motor3.run(FORWARD);
    Motor4.run(FORWARD);
    delay(2000);
//BRAKE
    Motor1.setSpeed(0);
    Motor2.setSpeed(0);
    Motor3.setSpeed(0);
    Motor4.setSpeed(0);
    Motor1.run(BRAKE);
    Motor2.run(BRAKE);
    Motor3.run(BRAKE);
    Motor4.run(BRAKE);
    delay(1000);*/
}
