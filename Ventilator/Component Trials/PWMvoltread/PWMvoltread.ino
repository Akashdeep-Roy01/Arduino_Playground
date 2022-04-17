#define VSR_PWM 3
#define pwmtovoltread A0
#define Min_PWM 10
#define Max_PWM 250

void setup() {
  Serial.begin(9600);
  pinMode(VSR_PWM, OUTPUT);
  pinMode(pwmtovoltread, INPUT);
}

void loop() {
  for(int i=Min_PWM;i<Max_PWM;i++){ 
      analogWrite(VSR_PWM,i); 
      int pwmval =analogRead(pwmtovoltread);
      float voltage =  pwmval*0.0048; 
      Serial.println(i);
      Serial.println(pwmval);
      delay(5000);  
  }

}
