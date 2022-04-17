/*PotTest - Program to check if selection can be made by dividing the 
 *          range of the potentiometer readout (0-1023) to serve as
 *          selector options.
 * Author - AkRoy
 * Date - 04/09/2020
 */

#define TV_Slt A0
#define BPM_Slt A1
//#define PEEP_Slt A2

void setup() {
  Serial.begin(9600);
  pinMode(TV_Slt,INPUT);
  pinMode(BPM_Slt,INPUT);
  //pinMode(PEEP_Slt,INPUT);

}

void loop() {
  int VolumeSlt = analogRead(TV_Slt);
  VolumeDisplay(VolumeSlt);  
  int BPMSlt = analogRead(BPM_Slt);
  BPMDisplay(BPMSlt);
}

void VolumeDisplay(int Volume){
  if(Volume<255){
    Serial.println("Tidal Volume = 350 ml");
    }
  else if (255<Volume && Volume<512){
    Serial.println("Tidal Volume = 400 ml");
    }
  else if (512<Volume && Volume<769){
    Serial.println("Tidal Volume = 450 ml");
    }
  else{
    Serial.println("Tidal Volume = 500 ml");
    }
   }

void BPMDisplay(int BPM){
  if(BPM<205){
    Serial.println("BPM = 10");
    }
  else if (205<BPM && BPM<511){
    Serial.println("BPM = 15");
    }
  else if (511<BPM && BPM<717){
    Serial.println("BPM = 20");
    }
  else if (717<BPM && BPM<923){
    Serial.println("BPM = 25");
    }
  else{
    Serial.println("BPM = 30");
    }
  }
