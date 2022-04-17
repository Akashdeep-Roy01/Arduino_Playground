/* Program to tune PID parameters for Pressure Control and Closed Loop CPAP mode
 * 
 * Author - AkRoy
 * Date - 14/05/2020
 */

#include<PWM.h>
#include<PID_v1.h>

#define VSR_PWM 3 // motor control board pin for controlling via PWM signal
#define EN_Stop_Start 2 // motor control board pin for Start- High; Stop - Low
#define FR_Direct 4 // motor control board pin for CCW - High; CW - Low
#define SetP_PotPin A0 // Pin for required pressure
#define PressureSensor_Pin A1 // Pin connected to pressure sensor
#define minBlowerpwm 10 // to set safe output range for PID 
#define maxBlowerpwm 200

int32_t frequency = 11000; //pwm frequency (in Hz)
double Setpoint, Input, Output; //define variables for PID
double Kp = , Ki = , Kd = ; // Tuning Parameters to be filled
PID PcontrolPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT); // create a PID object for pressure control

void setup() {
  Serial.begin(9600);
  InitTimersSafe();// all timers except Timer 0 are initialized
  SetPinFrequencySafe(VSR_PWM, frequency);//Set frequency as per defined frequency
  pinMode(VSR_PWM,OUTPUT);
  pinMode(EN_Stop_Start,OUTPUT);
  pinMode(FR_Direct,OUTPUT); 
  pinMode(SetP_PotPin,INPUT);
  pinMode(PressureSensor_Pin, INPUT);
  PcontrolPID.SetOutputLimits(minBlowerpwm, maxBlowerpwm); // set safe blower pwm output range for PID result
  PcontrolPID.setMode(Automatic); // In automatic PID is being used
}

void loop() {
  Setpoint =  ReadSetPressureValue(SetP_PotPin); // calls function to read set pressure value, returns int between 0 to 40 cm H2O
  Serial.println(Setpoint);
  Input = ReadActualPressureValue(PressureSensor_Pin); // calls function to read actual pressure value from sensor readings, returns value in cm H2O
  Serial.println(Input);
  int pwm = PcontrolPID.Compute();
  runBlower(pwm);
}


int ReadSetPressureValue(int pin){
  int temp = analogRead(pin);
  int result = map(temp, 0, 1024, 0, 40); // Settable range is between 0 to 40 cm water
  return result;
}

int ReadActualPressureValue(int pin){
  int temp = analogRead(pin);
  int result = convertvolttopressure*temp + constant;
  return result;
}

void runBlower(int pwmval){
  digitalWrite(FR_Direct,HIGH);
  pwmWrite(VSR_PWM,pwmval); 
  digitalWrite(EN_Stop_Start,HIGH);
 }
