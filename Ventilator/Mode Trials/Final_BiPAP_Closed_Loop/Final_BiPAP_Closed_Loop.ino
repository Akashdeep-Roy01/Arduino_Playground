/* Program to run ventilator in Bipap mode with settable Inhale Pressure and Exhale Pressure
 * Date - 14/09/2020
 */

// Library Definition
#include <FlexiTimer2.h>  
#include <TimerOne.h>
#include <PID_v1.h>
#include <Filters.h>

//IO Definitons
#define blowerPin 9 // Blower pwm pin controlled by Timer 1
#define solPin 12 // Solenoid control pin - solenoid is NC
#define PressureSensor_Pin A1 // Analog Pin to read sensor value

//Parameters Definiton
#define convertvolttopressure 0.0818 // Pressure sensor calibration
#define constantpressure 20.079 // Pressure sensor calibration
#define minBlowerpwm 8.0 // to set safe output range for PID 
#define maxBlowerpwm 100.0 // to set safe output range for PID
float zero_correction = 0; // zero correction variable 

//PID Definition
double Setpoint;  
volatile double Output, Input; 
double Kp = 1.544, Ki = 3.500, Kd = 0; //Tuning Parameters to be filled
PID PcontrolPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT); // create a PID object for pressure control

//Signal Filter Initiation
#define filterFrequency 670 // LPF cutoff frequency for pressure sensor output  
FilterOnePole lowpassFilter( LOWPASS, filterFrequency ); // Create a LPF to filter dP sensor o/p signals   

//Flag Initiation
int breathstate = 1; // flag to change breath stage - 1 = Inhale, 0 = Exhale
int zerocorrectflag =1; // flag to enable/disable Zero Error Correction

void setup(void)
{
  Serial.begin(9600);
  Timer1.initialize(50); // 50 us = 20 kHz for blower pwm frequency
  FlexiTimer2::set(10, 1.0/1000, computePID); // for interrupt timer - for data collection and pid output calculation
  pinMode(blowerPin,OUTPUT); 
  pinMode(solPin,OUTPUT);
  pinMode(PressureSensor_Pin, INPUT);
  PcontrolPID.SetOutputLimits(minBlowerpwm, maxBlowerpwm); // set safe blower pwm output range for PID result
  PcontrolPID.SetSampleTime(10); // PID loop calculation time cut off in msec
  PcontrolPID.SetMode(AUTOMATIC); // Enable PID
  FlexiTimer2::start(); // start timer at above defined parameters
  }

void loop(void)
{ 
  while(zerocorrectflag ==1){ // Perform zero error correction and checking till error is removed
   DoZeroCorrection();
   zerocorrectflag = CheckZeroError(); // reset zero correction flag if no zero error found
   //Serial.print(zerocorrectflag); // For debugging only
   }
  
  if(breathstate == 1){
      Setpoint = 30; // Define Inhale Setpt Pressure in cm H2O
      digitalWrite(solPin,LOW ); // Close Exhale Solenoid Valve
      //PcontrolPID.SetTunings(1, 3.5, 0); // Set Kp,Ki and Kd for Inhale period (if needed) 
      int start_inhale_time = millis();
      int now = millis();  
      while(now - start_inhale_time  < 1000){ // run inhale mode till inhale period is over
          now = millis();// update now time 
          plotGraph(); // Function call to plot graph
          }
      breathstate = 0; // switch to exhale mode
      }
  if(breathstate == 0){
      Setpoint = 5; // Define Exhale Setpt Pressure in cm H2O
      digitalWrite(solPin, HIGH); // Open Exhale Solenoid Valve
      //PcontrolPID1.SetTunings(4.62, 10.5, 0); // Set Kp,Ki and Kd for Exhale period (if needed)
      int start_exhale_time= millis(); 
      now = millis();
      while(now - start_exhale_time  < 2000){  // run exhale mode till exhale period is over
          now = millis();
          plotGraph(); 
          }
      breathstate = 1; // switch to inhale mode
  } 
}

void computePID(){
  int temp = ReadActualPressureValue(PressureSensor_Pin)- zero_correction; // calls function to read actual pressure value from sensor readings, returns value in cm H2O
  Input = lowpassFilter.input(temp);
  PcontrolPID.Compute();
  Timer1.pwm(blowerPin,(Output/ 100) * 1023);
}

int ReadActualPressureValue(int pin){
  int temp = analogRead(pin);
  int result = convertvolttopressure*temp - constantpressure;
  return result;
}

void DoZeroCorrection(){
  Serial.println("Performing Zero Error Correction");
  float totalzero = 0; // params defined for zero error correction
  int j = 0;
  digitalWrite(solPin,HIGH);
  for(j;j<100;j++){
    float samplezero = ReadActualPressureValue(PressureSensor_Pin);
    totalzero = totalzero + samplezero;
    }
  zero_correction = totalzero/j;
}

int CheckZeroError(){
  Serial.println("Checking for Zero Error");
  float totalsample = 0;
  int i = 0;
  digitalWrite(solPin,HIGH);
  for(i;i<100;i++){
    float samplezero = ReadActualPressureValue(PressureSensor_Pin);
    totalsample = totalsample + samplezero;
    }
  float sampleaverage = totalsample/i;
  if(sampleaverage >0.3){
    return 1;
    Serial.println("Zero Error Present!! Redoing Correction...");
    } 
  else {
    return 0;
    Serial.println("Zero Error Correction Done!!");
    Serial.print("Zero Error ");
    Serial.println(zero_correction);
    } 
  }

void plotGraph(){
  Serial.print(Setpoint);
  Serial.print(" ");
  Serial.print(Input);
  Serial.print(" ");
  Serial.println(Output);
}

int ReadSetPressureValue(int pin){
  int temp = analogRead(pin);
  int result = map(temp, 0, 1024, 0, 30); // Settable range is between 0 to 40 cm water
  return result;
}
