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
#define solPin 14 // Solenoid control pin - solenoid is NC
#define PressureSensor_Pin A1 // Analog Pin to read pressure sensor value
#define InputFlowSensor_Pin A3// Analog Pin to read inhale flow sensor value
#define OutputFlowSensor_Pin A2// Analog Pin to read exhale flow sensor value

//Parameters Definiton
#define convertvolttopressure 0.0818 // Pressure sensor calibration
#define constantpressure 20.079 // Pressure sensor calibration
#define convertvolttoflow 0.3261 // Flow sensor calibration
#define constantflow 47.478 // Flow sensor calibration
#define minBlowerpwm 8.0 // to set safe output range for PID 
#define maxBlowerpwm 100.0 // to set safe output range for PID
#define BPM 20 // BPM value
#define PEEP 5 // Exhalation Pressure Setting(cmH2O)
#define PIP 30 // Inhalation Pressure Setting(cmH2O)
float zero_correctionpressure = 0; // pressure sensor zero correction variable 
float zero_correctioninputflow = 0; // flow sensor zero correction variable
float zero_correctionoutputflow = 0; // flow sensor zero correction variable 
float inhaletime, exhaletime;
float volt1 = 0;
float volt2 = 0;
float Volume = 0;
float FlowReading;
float temp2=0;

 
//PID Definition
double Setpoint;  
volatile double Output, PressureReading, InputFlowReading, OutputFlowReading, VolumeReading =0 ; 
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
  pinMode(InputFlowSensor_Pin, INPUT);
  pinMode(OutputFlowSensor_Pin, INPUT);
  PcontrolPID.SetOutputLimits(minBlowerpwm, maxBlowerpwm); // set safe blower pwm output range for PID result
  PcontrolPID.SetSampleTime(8); // PID loop calculation time cut off in msec
  PcontrolPID.SetMode(AUTOMATIC); // Enable PID
  FlexiTimer2::start(); // start timer at above defined parameters
  Serial.println("Setpt. Flow InputFlow OutputFlow Pressure Volume");// for printing plotter legend
  }

void loop(void)
{ 
  while(zerocorrectflag ==1){ // Perform zero error correction and checking till error is removed
   DoZeroCorrectionPressure();
   DoZeroCorrectionInputFlow();
   DoZeroCorrectionOutputFlow();
   zerocorrectflag = CheckZeroError(); // reset zero correction flag if no zero error found
   //Serial.print(zerocorrectflag); // For debugging only
   }

  CalculateBreathTime();
  SetPIDTuningParameters();
  
  if(breathstate == 1){
      VolumeReading = 0;
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

float ReadActualPressureValue(){
  int temp = analogRead(PressureSensor_Pin);
  float result = lowpassFilter.input(convertvolttopressure*temp - constantpressure - zero_correctionpressure);
  return result;
}

float ReadActualInputFlowValue(){
  int temp = analogRead(InputFlowSensor_Pin);
  float result = lowpassFilter.input(convertvolttoflow*temp - constantflow - zero_correctioninputflow);
  return result;
}
float ReadActualOutputFlowValue(){
  int temp = analogRead(OutputFlowSensor_Pin);
  float result = lowpassFilter.input(convertvolttoflow*temp - constantflow - zero_correctionoutputflow);
  return result;
}

void DoZeroCorrectionPressure(){
  //Serial.println("Performing Zero Error Correction");
  float totalzero = 0; // params defined for zero error correction
  int j = 0;
  digitalWrite(solPin,HIGH);
  for(j;j<100;j++){
    float samplezero = ReadActualPressureValue();
    totalzero = totalzero + samplezero;
    }
  zero_correctionpressure = totalzero/j;
}

void DoZeroCorrectionInputFlow(){
  //Serial.println("Performing Zero Error Correction");
  float totalzero = 0; // params defined for zero error correction
  int j = 0;
  digitalWrite(solPin,HIGH);
  for(j;j<100;j++){
    float samplezero = ReadActualInputFlowValue();
    totalzero = totalzero + samplezero;
    }
  zero_correctioninputflow = totalzero/j;
}


void DoZeroCorrectionOutputFlow(){
  //Serial.println("Performing Zero Error Correction");
  float totalzero = 0; // params defined for zero error correction
  int j = 0;
  digitalWrite(solPin,HIGH);
  for(j;j<100;j++){
    float samplezero = ReadActualOutputFlowValue();
    totalzero = totalzero + samplezero;
    }
  zero_correctionoutputflow = totalzero/j;
}

int CheckZeroError(){
  //Serial.println("Checking for Zero Error");
  float totalsample = 0;
  int i = 0;
  digitalWrite(solPin,HIGH);
  for(i;i<100;i++){
    float samplezeropressure = ReadActualPressureValue();
    float samplezeroflow = ReadActualInputFlowValue()+ReadActualOutputFlowValue();
    totalsample = totalsample + samplezeropressure + samplezeroflow;
    }
  float sampleaverage = abs(totalsample/(3*i));
  if(sampleaverage >0.3){
    return 1;
    Serial.println("Zero Error Present!! Redoing Correction...");
    } 
  else {
    return 0;
    } 
  }

void plotGraph(){
  PressureReading = ReadActualPressureValue(); // calls function to read actual pressure value from sensor readings, returns value in cm H2O
  InputFlowReading = ReadActualInputFlowValue(); // calls function to read actual pressure value from sensor readings, returns value in cm H2O
  OutputFlowReading = ReadActualOutputFlowValue();
  //GetFlow();
  GetVolume(OutputFlowReading);  
  //Serial.print(millis());
  //Serial.print(" ");
  //Serial.print(volt1);
  //Serial.print(", ");
  //Serial.print(volt2);
  //Serial.print(", ");
  Serial.print(Setpoint);
  Serial.print(", ");
  //Serial.print(FlowReading);
  //Serial.print(", ");
  //Serial.print(InputFlowReading);
  //Serial.print(", ");
  Serial.print(OutputFlowReading);
  Serial.print(", ");  
  Serial.print(PressureReading);
  Serial.print(", ");
  Serial.print(VolumeReading);
  Serial.println(", ");
  /*Serial1.print("add 7,0," + String(result));
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.print("add 7,1," + String(10));
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.print("add 15,0," + String(FlowReadingPrint));
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.print("add 15,1," + String(60));
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.print("add 16,0," + String(VolumeReading));
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.print("add 16,1," + String(0));
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);*/
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

void GetVolume(float temp){
  volt2 = volt1;
  volt1 = millis();
  VolumeReading = VolumeReading + (((temp2+temp)/2)*((volt1-volt2)/60));
  temp2 = temp;
  //if(VolumeReading<=0){
    //VolumeReading = 0;    
    //}
  }

//void GetFlow(){
  //FlowReading = InputFlowReading - abs(OutputFlowReading);
//  if(breathstate == 0 && ){
  // FlowReading = 0;
  // }
//}
