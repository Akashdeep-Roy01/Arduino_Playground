/* BlowerTest-Program to test the connectivity of board, blower BLDC and Arduino.
 *            Program has for loop to run blower from minimum to maximum speed by varying pwm value between min and max value
 *            Modification - Frequency of PWM is settable; PWM library is to be added using Include Library from Sketch Menu
 * Author - AkRoy
 * Date - 15/04/2020
 */
#include<PWM.h>
 
#define VSR_PWM 3
#define EN_Stop_Start 2
#define FR_Direct 4
#define FG_Hall 7
#define Min_PWM 10
#define Max_PWM 250
int32_t frequency = 11000; //frequency (in Hz)

void setup() {
  Serial.begin(9600);
  InitTimersSafe();// all timers except Timer 0 are initialized
  SetPinFrequencySafe(VSR_PWM, frequency);//Set frequency as per defined frequency
  pinMode(VSR_PWM,OUTPUT);//for controlling via PWM signal
  pinMode(EN_Stop_Start,OUTPUT);//for Start- High; Stop - Low
  pinMode(FR_Direct,OUTPUT); //for CCW - High; CW - Low
 // pinMode(FG_Hall,INPUT); // for speed pulse output 
}

void loop() {
  BlowerRun();//function to run blower from min to max rpm with PWM signal
  delay(1000);
  BlowerStop();//function to stop the blower
  delay(5000);
}

void BlowerRun(){
  digitalWrite(FR_Direct,HIGH);
  delay(1000);
  for(int i=Min_PWM;i<Max_PWM;i++){ 
      pwmWrite(VSR_PWM,i); 
      delay(1000);
      digitalWrite(EN_Stop_Start,HIGH);
      Serial.println(i);
      delay(5000);  
  }
  }

void BlowerStop(){
  pwmWrite(VSR_PWM,10); 
  digitalWrite(EN_Stop_Start,LOW);
  }
