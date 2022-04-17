/*PotTest - Program to simply read the value outputted by potentiometers.
 *          Have to check if #define works with analog pins otherwise to define pins as int name and pin number 
 * Author - AkRoy
 * Date - 04/09/2020
 */

#define TV_Slt A0
#define BPM_Slt A1
//#define PEEP_Slt A2

void setup() {
  //Serial.begin(9600);
  pinMode(TV_Slt,INPUT);
  pinMode(BPM_Slt,INPUT);
  //pinMode(PEEP_Slt,INPUT);

}

void loop() {
  int VolumeSlt = analogRead(TV_Slt);
  Serial.print("Volume Pot Value = ");
  Serial.println(VolumeSlt);
  int BPMSlt = analogRead(BPM_Slt);
  Serial.print("BPM Pot Value = ");
  Serial.println(BPMSlt);
}


