/* Program to run ventilator in Pressure Control mode with settable Inhale Pressure and Exhale Pressure
 * Date - 26/10/2020
 */

// Library Definition
#include <TimerOne.h>
#include <AutoPID.h>
#include <Filters.h>
#include <FlexiTimer2.h>

//Ventilator Set Parameters
//#define BPM 10 // BPM value
//double PEEP = 5;// Exhalation Pressure Setting(cmH2O)
//double PIP = 25; // Inhalation Pressure Setting(cmH2O)
int PEEP;
int PIP;
int BPM;

//IO Definitons
#define blowerPin 11 // Blower pwm pin controlled by Timer 1
#define solPin 2 // Solenoid control pin - solenoid is NC
#define PressureSensor_Pin A0 // Analog Pin to read pressure sensor value
#define InputFlowSensor_Pin A1// Analog Pin to read inhale flow sensor value
#define OutputFlowSensor_Pin A2// Analog Pin to read exhale flow sensor value

// System Variables
float zero_correctionpressure = 0; // pressure sensor zero correction variable 
float zero_correctioninputflow = 0; // flow sensor zero correction variable
float zero_correctionoutputflow = 0; // flow sensor zero correction variable 
float inhaletime, exhaletime; // Calculated based upon BPM and I:E ratio
float volt1 = 0; // T0 time for volume calculation
float volt2 = 0; // T1 time for voulme calculation
float FlowReading;
float temp2=0;
#define minBlowerpwm 8.0 // to set safe output range for PID 
#define maxBlowerpwm 65.0 // to set safe output range for PID
 
//PID Definition
double Setpoint;  
volatile double InhaleOutput,ExhaleOutput, PressureReading, InputFlowReading, OutputFlowReading, VolumeReading =0 ;
double InhaleIntegral, ExhaleIntegral; 
#define Kp 0
#define Ki 0
#define Kd 0//Tuning Parameters to be filled
//AutoPID InhalePcontrolPID(&PressureReading, &PIP,  &InhaleOutput, minBlowerpwm, maxBlowerpwm,6,10,20);
//AutoPID ExhalePcontrolPID(&PressureReading, &PEEP,  &ExhaleOutput, minBlowerpwm, maxBlowerpwm, 30,0,70);// create a PID object for pressure control

//Signal Filter Initiation
#define filterFrequency 670 // LPF cutoff frequency for pressure sensor output  
FilterOnePole lowpassFilter( LOWPASS, filterFrequency ); // Create a LPF to filter dP sensor o/p signals   

//Flag Initiation
int breathstate = 0; // flag to change breath stage - 1 = Inhale, 0 = Exhale
int zerocorrectflag = 1; // flag to enable/disable Zero Error Correction

//Sensor Calibration Parameter Definiton
#define convertvolttopressure 0.0818 // Pressure sensor calibration
#define constantpressure 20.079 // Pressure sensor calibration
#define convertvolttoflow 0.3261 // Flow sensor calibration
#define constantflow 47.478 // Flow sensor calibration

void setup(void)
{
  Serial.begin(9600);
  Serial2.begin(9600);// for communicating with HMI screen
  digitalWrite(solPin,HIGH);
  Timer1.initialize(25); // 50 us = 20 kHz for blower pwm frequency
  pinMode(blowerPin,OUTPUT); 
  pinMode(solPin,OUTPUT);
  pinMode(PressureSensor_Pin, INPUT);
  pinMode(InputFlowSensor_Pin, INPUT);
  pinMode(OutputFlowSensor_Pin, INPUT);
  //InhalePcontrolPID.setTimeStep(9); // PID loop calculation time cut off in msec
  //ExhalePcontrolPID.setTimeStep(9);
  //InhalePcontrolPID.setBangBang(10.0,10.0);
  //ExhalePcontrolPID.setBangBang(3.0,60.0);
  Serial.println("Setpt. Flow InputFlow OutputFlow Pressure Volume");// for printing plotter legend
  FlexiTimer2::set(10, 1.0/1000, computePID);
  FlexiTimer2::start(); // start timer at above defined parameters
  }

void loop(void)
{ 
  /*while(zerocorrectflag == 1){ // Perform zero error correction and checking till error is removed
   DoZeroCorrectionPressure();
   delay(2000);
   zerocorrectflag = CheckZeroError(); // reset zero correction flag if no zero error found
   //Serial.print(zerocorrectflag); // For debugging only
   }*/

  CalculateBreathTime(); // Function call to update Inhale and Exhale time for changing bpm and IE ratio
  //SetPIDTuningParameters(); // Function to update PID params if needed
  
  if(breathstate == 1){
      VolumeReading = 0;
      digitalWrite(solPin,LOW); // Close Exhale Solenoid Valve
      int start_inhale_time = millis();
      int now = millis(); 
      Setpoint = PIP; 
      while(now - start_inhale_time  < inhaletime){ // run inhale mode till inhale period is over
          now = millis();// update now time
          plotGraph(); // Function call to plot graph
          }
      breathstate = 0; // switch to exhale mode
      }
  if(breathstate == 0){
      digitalWrite(solPin, HIGH); // Open Exhale Solenoid Valve
      int start_exhale_time= millis(); 
      int now = millis();
      Setpoint = PEEP-2.0;
      //ExhalePcontrolPID.setIntegral(ExhaleIntegral);
      while(now - start_exhale_time  < exhaletime){  // run exhale mode till exhale period is over
          now = millis();
          plotGraph(); 
          }
      //ExhaleIntegral = ExhalePcontrolPID.getIntegral();
      breathstate = 1; // switch to inhale mode
      } 
}

void computePID(){
  CheckHMIdata();
  PressureReading = ReadActualPressureValue(); // calls function to read actual pressure value from sensor readings, returns value in cm H2O
  if(breathstate == 1){
    //ExhalePcontrolPID.stop();
    //InhalePcontrolPID.run();
    Timer1.pwm(blowerPin,(70.0/ 100) * 1023);
    //Timer1.pwm(blowerPin,(InhaleOutput/ 100) * 1023);
  }
  else if(breathstate == 0){
    //InhalePcontrolPID.stop();
    //ExhaleIntegral = ExhalePcontrolPID.getIntegral();
    //ExhalePcontrolPID.run();
    Timer1.pwm(blowerPin,(44.0/ 100) * 1023);
    //Timer1.pwm(blowerPin,(ExhaleOutput/ 100) * 1023);
    }
 }

void CheckHMIdata(){  // to check string length logic 
  if (Serial2.available()){
  char character="";
  String data_from_display="";
  delay(20);
  while(Serial2.available()){
    character=char (Serial2.read());
    data_from_display += character;
  }
  //Mode = data_from_display.substring(0,6);
  PIP = data_from_display.substring(6,9).toInt();
  PEEP = data_from_display.substring(9,12).toInt();
  //FIO2 = data_from_display.substring(12,15).toInt();
  BPM = data_from_display.substring(15,18).toInt();    
}}
  
float ReadActualPressureValue(){
  int temp = analogRead(PressureSensor_Pin);
  float result = lowpassFilter.input(convertvolttopressure*temp - constantpressure - zero_correctionpressure);
  return result;
}

int ReadActualInputFlowValue(){
  float temp = analogRead(InputFlowSensor_Pin);
  float result;
  if(temp < 195){
   result = 0.633*temp - 91.106;
  }
  else{
   result = 0.25*temp - 20.267;
  }
  return result;
}

int ReadActualOutputFlowValue(){
  float temp = analogRead(OutputFlowSensor_Pin);
  float result;
  if(temp<210){
    result = 0.4782*temp - 68.263;
  }
  else{
    result = 0.1875*temp - 12.767;
    }
  return result;
}

void DoZeroCorrectionPressure(){
  delay(2000);
  //Serial.println("Performing Zero Error Correction");
  float totalzero = 0; // params defined for zero error correction
  int j = 0;
  //digitalWrite(solPin,LOW);
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
  //digitalWrite(solPin,LOW);
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
  //digitalWrite(solPin,LOW);
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
  //digitalWrite(solPin,LOW);
  for(i;i<100;i++){
    float samplezeropressure = ReadActualPressureValue();
    //float samplezeroflow = ReadActualInputFlowValue()+ReadActualOutputFlowValue();
    totalsample = totalsample + samplezeropressure;
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
  GetFlow();
  GetVolume(FlowReading);  
  Serial.println(BPM);
  Serial.println(PIP);
  Serial.println(PEEP);
  //Serial.print(millis());
  //Serial.print(", ");
  //Serial.print(volt1);
  //Serial.print(", ");
  //Serial.print(volt2);
  //Serial.print(", ");
  //Serial.print(Setpoint);
  //Serial.print(", ");
  //Serial.print(FlowReading);
  //Serial.print(", ");
  //Serial.print(InputFlowReading);
  //Serial.print(", ");
  //Serial.print(OutputFlowReading);
  //Serial.print(", ");  
  //Serial.print(PressureReading);
  //Serial.print(", ");
  //Serial.print(VolumeReading);
  //Serial.println(", ");
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

void GetVolume(float temp){
  volt2 = volt1;
  volt1 = millis();
  VolumeReading = VolumeReading + (((temp2+temp)/2)*((volt1-volt2)/60));
  temp2 = temp;
  if (VolumeReading<0){
    VolumeReading = 0;
    }
}

void GetFlow(){
  FlowReading = InputFlowReading - abs(OutputFlowReading);
}
