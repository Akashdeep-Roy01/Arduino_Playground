/* Program to calibrate flow sensor
 * Date - 14/10/2020
 */

// Library Definition
#include <FlexiTimer2.h>  
#include <TimerOne.h>
#include <Filters.h>

//IO Definitons
#define blowerPin 11 // Blower pwm pin controlled by Timer 1
#define solPin 2 // Solenoid control pin - solenoid is NC
#define InFlowSensor_Pin A3 // Analog Pin to read sensor value
#define OutFlowSensor_Pin A2 // Analog Pin to read sensor value
#define VTidal = 400

//Parameters Definiton
#define convertvolttoflow 1 // Pressure sensor calibration
#define constantflow 0 // Pressure sensor calibration
float Input1;
float Input2;

void setup(void)
{
  Serial.begin(9600);
  Timer1.initialize(50); // 50 us = 20 kHz for blower pwm frequency
  pinMode(blowerPin,OUTPUT); 
  pinMode(InFlowSensor_Pin, INPUT);
  pinMode(OutFlowSensor_Pin, INPUT);
  pinMode(solPin,OUTPUT); 
  digitalWrite(solPin, LOW);
  }

void loop(void)
{ //for(float blowerpwm = 20.0; blowerpwm<80.0; blowerpwm += 55.0){
  //for(int i = 0; i<100; i++){
      Timer1.pwm(blowerPin,(30.0/ 100) * 1023);
      //Input1 = analogRead(InFlowSensor_Pin);
      Input1 = ReadActualInputFlowValue();
      Serial.print(Input1);
      Serial.print(",");
      Input2 = ReadActualOutputFlowValue();
      //Input2 = analogRead(OutFlowSensor_Pin);
      Serial.print(Input2);
      float delta = Input1-abs(Input2);
      Serial.print(",");
      Serial.print(delta);
      Serial.println(",");
      //}}
}

int ReadActualInputFlowValue(){
  float temp = analogRead(InFlowSensor_Pin);
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
  float temp = analogRead(OutFlowSensor_Pin);
  float result;
  if(temp<210){
    result = 0.4782*temp - 68.263;
  }
  else{
    result = 0.1875*temp - 12.767;
    }
  return result;
}


/*void CalculateBreathTime(){
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

float CalcPWMfromVTidal(){
  float temp = (VTidal*3*convertlpmtopwm)/(50*inhaletime)-constantlpmtopwm; // (Vtidal in mL * 60)/(1000 * Inhale time in mSec)
  float result = round(temp);
  return result;
  }*/
