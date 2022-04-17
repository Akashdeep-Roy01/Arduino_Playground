/* Program to identify process characteristics for Pressure Control and Closed Loop CPAP mode
 * 
 * Author - AkRoy
 * Date - 14/05/2020
 */

#include<PWM.h>
#include<PID_v1.h>

#define VSR_PWM 3 // motor control board pin for controlling via PWM signal
#define EN_Stop_Start 2 // motor control board pin for Start- High; Stop - Low
#define FR_Direct 4 // motor control board pin for CCW - High; CW - Low
#define SetRPM_PotPin A0 // Pin for required blower rpm
#define PressureSensor_Pin A1 // Pin connected to pressure sensor
int32_t frequency = 11000; //pwm frequency (in Hz)
#define convertvolttopressure 1 // as per correlation will be changed

void setup() {
  Serial.begin(9600);
  //Serial.println("CLEARDATA");
  //Serial.println("LABEL,Computer Time, Time(Millisec), Pressure");
  InitTimersSafe();// all timers except Timer 0 are initialized
  SetPinFrequencySafe(VSR_PWM, frequency);//Set frequency as per defined frequency
  pinMode(VSR_PWM,OUTPUT);
  pinMode(EN_Stop_Start,OUTPUT);
  pinMode(FR_Direct,OUTPUT); 
  pinMode(SetRPM_PotPin,INPUT);
  pinMode(PressureSensor_Pin, INPUT);
}

void loop() {
  int pwm =  ReadPotValue(SetRPM_PotPin); // calls function to read set rpm value, returns int between 0 to 255 pwm value
  runBlower(pwm);
  Serial.print(pwm);
  int milli_time = millis();// for getting time in excel
  Serial.print(milli_time);
  int pressurereading = ReadActualPressureValue(PressureSensor_Pin); // calls function to read actual pressure value from sensor readings, returns value in cm H2O
  Serial.println(pressurereading);
}

int ReadPotValue(int pin){
  int temp = analogRead(pin);
  int result = map(temp, 0, 1024, 0, 255); // Settable range is between 0 to 255 pwm val
  return result;
}

int ReadActualPressureValue(int pin){
  int temp = analogRead(pin);
  int result = convertvolttopressure*temp ;
  return result;
}

void runBlower(int pwmval){
  digitalWrite(FR_Direct,HIGH);
  pwmWrite(VSR_PWM,pwmval); 
  digitalWrite(EN_Stop_Start,HIGH);
 }
