/* Program to run ventilator in Volume Control mode with settable Inhale Pressure and Exhale Pressure
 * Date - 14/10/2020
 */

// Library Definition
#include <FlexiTimer2.h>  
#include <TimerOne.h>
#include <PID_v1.h>
#include <Filters.h>

//IO Definitons
#define blowerPin 11 // Blower pwm pin controlled by Timer 1
#define solPin 2 // Solenoid control pin - solenoid is NC
#define PressureSensor_Pin A1 // Analog Pin to read pressure sensor value
#define FlowSensor_Pin A3// Analog Pin to read flow sensor value

//Parameters Definiton
#define convertvolttopressure 0.0818 // Pressure sensor calibration
#define constantpressure 20.079 // Pressure sensor calibration
#define convertvolttoflow 0.1387 // Flow sensor calibration
#define constantflow 35.173 // Flow sensor calibration
#define minBlowerpwm 8.0 // to set safe output range for PID 
#define maxBlowerpwm 100.0 // to set safe output range for PID
float zero_correctionpressure = 0; // pressure sensor zero correction variable 
float zero_correctionflow = 0; // flow sensor zero correction variable 
#define BPM 20 // BPM value
float inhaletime, exhaletime;
#define PEEP 10 // Exhalation Pressure Setting(cmH2O)
#define PIP 30 // Inhalation Pressure Setting(cmH2O)

//PID Definition
double Setpoint;  
volatile double Output, PressureReading, FlowReading; 
double Kp, Ki, Kd; //Tuning Parameters to be filled
PID PcontrolPID(&PressureReading, &Output, &Setpoint, Kp, Ki, Kd, DIRECT); // create a PID object for pressure control

//Signal Filter Initiation
#define filterFrequency 670 // LPF cutoff frequency for pressure sensor output  
FilterOnePole lowpassFilter( LOWPASS, filterFrequency ); // Create a LPF to filter dP sensor o/p signals   

//Flag Initiation
int breathstate = 1; // flag to change breath stage - 1 = Inhale, 0 = Exhale
int zerocorrectflag = 1; // flag to enable/disable Zero Error Correction

void setup(void)
{
  Serial.begin(9600);
  Serial1.begin(9600);
  Timer1.initialize(50); // 50 us = 20 kHz for blower pwm frequency
  FlexiTimer2::set(10, 1.0/1000, computePID); // for interrupt timer - for data collection and pid output calculation
  pinMode(blowerPin,OUTPUT); 
  pinMode(solPin,OUTPUT);
  pinMode(PressureSensor_Pin, INPUT);
  pinMode(FlowSensor_Pin, INPUT);
  PcontrolPID.SetOutputLimits(minBlowerpwm, maxBlowerpwm); // set safe blower pwm output range for PID result
  PcontrolPID.SetSampleTime(8); // PID loop calculation time cut off in msec
  PcontrolPID.SetMode(AUTOMATIC); // Enable PID
  FlexiTimer2::start(); // start timer at above defined parameters
  }

void loop(void)
{ 
  while(zerocorrectflag ==1){ // Perform zero error correction and checking till error is removed
   DoZeroCorrectionPressure();
   //DoZeroCorrectionFlow();
   zerocorrectflag = CheckZeroError(); // reset zero correction flag if no zero error found
   //Serial.print(zerocorrectflag); // For debugging only
   }

  CalculateBreathTime();
  SetPIDTuningParameters();
  
  if(breathstate == 1){
      digitalWrite(solPin,LOW); // Close Exhale Solenoid Valve
      //Timer1.pwm(blowerPin,(43.0/ 100) * 1023);
      Setpoint = PIP; // Define Inhale Setpt Pressure in cm H2O
      //PcontrolPID.SetTunings(0.5, 3.5, 0); // Set Kp,Ki and Kd for Inhale period (if needed) 
      int start_inhale_time = millis();
      int now = millis();  
      while(now - start_inhale_time  < inhaletime){ // run inhale mode till inhale period is over
          now = millis();// update now time
          plotGraph(); // Function call to plot graph
          }
      breathstate = 0; // switch to exhale mode
      }
  if(breathstate == 0){
      digitalWrite(solPin, HIGH); // Open Exhale Solenoid Valve
      //Timer1.pwm(blowerPin,(55.0/ 100) * 1023);
      Setpoint = PEEP; // Define Exhale Setpt Pressure in cm H2O
      //PcontrolPID.SetTunings(2.85, 3.12, 0); // Set Kp,Ki and Kd for Exhale period (if needed)
      int start_exhale_time= millis(); 
      int now = millis();
      while(now - start_exhale_time  < exhaletime){  // run exhale mode till exhale period is over
          now = millis();
          plotGraph(); 
          }
      breathstate = 1; // switch to inhale mode
  } 
}

void computePID(){
  PressureReading = ReadActualPressureValue(); // calls function to read actual pressure value from sensor readings, returns value in cm H2O
  PcontrolPID.Compute();
  Timer1.pwm(blowerPin,(Output/ 100) * 1023);
  }

int ReadActualPressureValue(){
  int temp = analogRead(PressureSensor_Pin);
  int result = lowpassFilter.input(convertvolttopressure*temp - constantpressure - zero_correctionpressure);
  return result;
}

int ReadActualFlowValue(){
  int temp = analogRead(FlowSensor_Pin);
  int result = lowpassFilter.input(-convertvolttoflow*temp + constantflow - zero_correctionflow);
  return result;
}

void DoZeroCorrectionPressure(){
  Serial.println("Performing Zero Error Correction");
  float totalzero = 0; // params defined for zero error correction
  int j = 0;
  digitalWrite(solPin,HIGH);
  for(j;j<100;j++){
    float samplezero = ReadActualPressureValue();
    totalzero = totalzero + samplezero;
    }
  zero_correctionpressure = totalzero/j;
}

void DoZeroCorrectionFlow(){
  Serial.println("Performing Zero Error Correction");
  float totalzero = 0; // params defined for zero error correction
  int j = 0;
  digitalWrite(solPin,HIGH);
  for(j;j<100;j++){
    float samplezero = ReadActualFlowValue();
    totalzero = totalzero + samplezero;
    }
  zero_correctionflow = totalzero/j;
}

int CheckZeroError(){
  Serial.println("Checking for Zero Error");
  float totalsample = 0;
  int i = 0;
  digitalWrite(solPin,HIGH);
  for(i;i<100;i++){
    float samplezeropressure = ReadActualPressureValue();
    //float samplezeroflow = ReadActualFlowValue();
    totalsample = totalsample + samplezeropressure;// + samplezeroflow;
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
    Serial.println(zero_correctionpressure);
    Serial.println(zero_correctionflow);
    } 
  }

void plotGraph(){
  PressureReading = ReadActualPressureValue(); // calls function to read actual pressure value from sensor readings, returns value in cm H2O
  //FlowReading = ReadActualFlowValue(); // calls function to read actual pressure value from sensor readings, returns value in cm H2O
  Serial.print(Setpoint);
  //Serial.print(millis());
  Serial.print(" ");
  Serial.println(PressureReading);
  int yy = analogRead(PressureSensor_Pin);
  int result = lowpassFilter.input(convertvolttopressure*yy - constantpressure - zero_correctionpressure);
  Serial.println(yy);
  Serial1.print("add 7,0," + String(result));
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  //Serial.print(" ");
  //Serial.println(FlowReading);
}

void CalculateBreathTime(){
  inhaletime = 20000/BPM; // 60000/(3*Bpm) as I:E Ratio is 1:2, output is in mSec
  exhaletime = 40000/BPM; // (2*60000)/(3*Bpm) as I:E Ratio is 1:2, output is in mSec
  }

void SetPIDTuningParameters()
{
  if(PEEP<6){
    PcontrolPID.SetTunings(1.72, 2.36, 0);
  }
  else{
    PcontrolPID.SetTunings(1.18, 1.35, 2.08);
  }  
  }

int ReadSetPressureValue(int pin){
  int temp = analogRead(pin);
  int result = map(temp, 0, 1024, 0, 30); // Settable range is between 0 to 40 cm water
  return result;
}

int ReadSetFlowValue(int pin){
  int temp = analogRead(pin);
  int result = map(temp, 0, 1024, 0, 30); // Settable range is between 0 to 40 cm water
  return result;
}
