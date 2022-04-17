#include <NewPing.h>

#define TRIGGER_PIN  15
#define ECHO_PIN     14
#define MAX_DISTANCE 200

NewPing DistanceSensor(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
int i=0;
int sum=0;
int p=0;
int k=1;
int samples[4]; 

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    unsigned int cm = DistanceSensor.ping_cm();
    int distance=average(cm);
    //Serial.print(" ||Distance: ");
    //Serial.println(distance);
    //Serial.println("cm");
    delay(1000);
}

int average(int dist){
  int avg;
    if(i>2){
    sum=sum-samples[p];
    samples[p]=dist;
    sum=sum+samples[p]; 
    p++;         
    avg=sum/3;}
     
      
   if(i<3){
      samples[p]=dist;
      avg=dist;
      sum=sum+samples[p];
      i++;
      p++;
      }

    if(p>2){
      p=0;
      }
            
    Serial.print(" Sum: ");
    Serial.print(sum);
    Serial.print(" ||Dist: ");
    Serial.print(dist);
    Serial.print(" Avg: ");
    Serial.println(avg);            
  return avg;
  }
