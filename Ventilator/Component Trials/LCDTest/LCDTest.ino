/*PotTest - Program to simply read the value outputted by potentiometers.
 *          Have to check if #define works with analog pins otherwise to define pins as int name and pin number 
 * Author - AkRoy
 * Date - 04/09/2020
 */
#include<LiquidCrystal.h>
#define TV_Slt A0
#define BPM_Slt A1
//#define PEEP_Slt A2
//#define LCDContrastPin 6 // Pin for Contrast adjustment in LCD
LiquidCrystal lcd = LiquidCrystal(7, 8, 9, 10, 11, 12);

void setup() {
  Serial.begin(9600);
  pinMode(TV_Slt,INPUT);
  pinMode(BPM_Slt,INPUT);
  //pinMode(LCDContrastPin, OUTPUT);
  //analogWrite(LCDContrastPin, 100);// needs to be changed as per reqmt between 0 to 255
  lcd.begin(16,2);
  //pinMode(PEEP_Slt,INPUT);

}

void loop() {
  int VolumeSlt = analogRead(TV_Slt);
  //Serial.print("Volume Pot Value = ");
  lcd.setCursor(8, 0);
  lcd.print(VolumeSlt);
  //Serial.println(VolumeSlt);
  //int BPMSlt = analogRead(BPM_Slt);
  //Serial.print("BPM Pot Value = ");
  //Serial.println(BPMSlt);
}
