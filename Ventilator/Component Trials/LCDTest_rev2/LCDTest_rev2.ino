#include<LiquidCrystal.h>
#define TV_PotPin A0 // Pin for tidal volume selector switch
#define BPM_PotPin A1 // Pin for BPM selector switch
#define LCDContrastPin 6 // Pin for Contrast adjustment in LCD
LiquidCrystal lcd = LiquidCrystal(7, 8, 9, 10, 11, 12);// Create an LCD object. Parameters: (RS, E, D4, D5, D6, D7); Connect Pins accordingly

void setup() {
  Serial.begin(9600); // for debugging purpose only
  lcd.begin(16,2); // Specify no. of columns and rows
  pinMode(LCDContrastPin, OUTPUT);
  analogWrite(LCDContrastPin, 10);// needs to be changed as per reqmt between 0 to 255
  pinMode(TV_PotPin,INPUT);
  pinMode(BPM_PotPin,INPUT);  
}

void loop() {
    int Volumepotval = analogRead(TV_PotPin); // to read Tidal volume pot value
    float TidalVolume  = vtidalVal(Volumepotval); // calls function vtidalVal to determine selected setting of Tidal Volume from pot value (int); returns float value in Litres
    lcd.setCursor(1, 0);
    lcd.print("Volume ");
    lcd.setCursor(8, 0);
    lcd.print(TidalVolume);
    Serial.println(TidalVolume);
    
    int BPMpotval = analogRead(BPM_PotPin); // to read BPM pot value
    int BPMval = getBPMVal(BPMpotval); // calls function to get selected BPM val, int value is returened for int input
    lcd.setCursor(1, 1);
    lcd.print("BPM ");
    lcd.setCursor(5, 1);
    lcd.print(BPMval);
    Serial.println(BPMval);  
}

float vtidalVal(int Volume){ 
  float result;
  if(Volume<255){
    result = 0.35;
    //Serial.println("Tidal Volume = 350 ml");
    }
  else if (255<Volume && Volume<513){
    result = 0.40;
    //Serial.println("Tidal Volume = 400 ml");
    }
  else if (512<Volume && Volume<770){
    result = 0.45;
    //Serial.println("Tidal Volume = 450 ml");
    }
  else{
    result = 0.50;
    //Serial.println("Tidal Volume = 500 ml");
    }
  //Serial.print(result);
  return result;
  }


int getBPMVal(int BPM){
  int temp;
  if(BPM<205){
    temp = 10;
    //Serial.println("BPM = 10");
    }
  else if (204<BPM && BPM<512){
    temp = 15;
    //Serial.println("BPM = 15");
    }
  else if (511<BPM && BPM<718){
    temp = 20;
    //Serial.println("BPM = 20");
    }
  else if (717<BPM && BPM<924){
    temp = 25;
    //Serial.println("BPM = 25");
    }
  else{
    temp = 30 ;
    //Serial.println("BPM = 30");
    }
  return temp;
  }
