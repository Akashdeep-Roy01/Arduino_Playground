#include<math.h>
#include<MatrixMath.h>
#include<AFMotor.h>
#define NumSamples 200
#define NumClasses 5
#define NumFeatures 4 

AF_DCMotor Motor1(1);
AF_DCMotor Motor2(4);
AF_DCMotor Motor3(3);

float data[NumSamples];
double features[NumFeatures];
int motion;
double Results[NumClasses];

const double coefficients[NumClasses][NumFeatures]={
  {2.76934538800396, -0.0818119056586182, -0.0889055408623875, 0.573371415719429},
{-1.05807503810391, 0.0399415344295564,  0.103982450405432, 0.915444955291467},
{3.43012952437581,  -0.126260291734234,  -0.0357620614334344, 0.534509896799532},
{0.422611925242412, -0.0559018370527478, 0.117781063434496, 0.287468529798410},
{2.58576042369466,  -0.0845640388809051, -0.0653560288818472, 0.526998348520332},
  
  };

const double constants[NumClasses]={-38.5639668089430,
-40.0471682146560,
-68.7830150754454,
-20.5756092572708,
-36.8985095075342};

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
float sum=0;
for(int i=0;i<NumSamples;i++){
  int sensorValue = analogRead(A0);
  data[i]=sensorValue ;//* (5.0 / 1023.0);
  //Serial.println(data[i]);
  sum+=data[i];
  }
  //Serial.println(sum);
  features[0]= Mean(sum);
  features[1]=Var(data, NumSamples, features[0]);
  features[2]=Wavelength(data,NumSamples);
  features[3]=Ssc(data,NumSamples);
  //Matrix.Print((float*)features, 1, NumFeatures, "outputactivation");  
  motion = predictMotion(features, NumFeatures);
  //Serial.print(features[0]);
  //Serial.print(features[1]);
  //Serial.print(features[2]);
  //Serial.print(features[3]);  
  //Serial.println(motion);
  //Serial.print("mean");
  //Serial.println(features[1]);
  //Serial.print("var");
  //Serial.println(features[2]);
  if(motion==0){
    Serial.println("Precision");
    Precision();
    } 
   else if(motion==1){
    Serial.println("Open");
    Open();
    } 
  else if(motion==2){
    Serial.println("Close");
    Close();
    } 
   else if(motion==3){
    Serial.println("Point");
    Open();
    } 
    else if (motion==4)
    {
      Serial.print("Lateral");
      }
}

int predictMotion(double feat[],int n){
   double out[NumClasses];
   int result;
   float maxm=0;
   
   Matrix.Multiply((float*)coefficients,(float*)features,NumClasses,NumFeatures,1,(float*)out);
   Matrix.Add((float*)out, (float*)constants,NumClasses,1, (float*)Results);
   for(int j=0;j<NumClasses;j++){
    if (j==0) {
      maxm=Results[0];
      result=0;
    }
    if(Results[j]>maxm){
      result=j;
      maxm=Results[j];
      }
    }
   /* Serial.println(Results[0]);
    Serial.println(Results[1]);
    Serial.println(Results[2]);
    Serial.println(Results[3]);
    Serial.println(Results[4]);*/
    return result;
    }
  

double Mean(float sm){
  float result = sm/NumSamples;
  return result;
  }

double Var(float dta[],int n,float sm){
  float temp;
  float tempt;
  float result;
  float accum=0;
  for(int j=0;j<n;j++){
    temp=dta[j]-sm;
    tempt=pow(temp,2);
    accum=accum+tempt;
    //Serial.print("sm");
   // Serial.println(tempt);
    }
    //Serial.println(temp);
    result=accum/(n-1);
    //Serial.print("accum");
    //Serial.println(result);
    return result;
     }

double Wavelength(float dta[],int n){
  float accum=0;
  float delta=0;
  for(int j=0;j<n-1;j++){
    delta=dta[j+1]-dta[j];
    accum+=abs(delta);
    }
  return accum;
  }

double Ssc(float dta[],int n){
  float count=0;
  for(int j=0;j<n;j++){
    if(dta[j+1]>dta[j] && dta[j+1]>dta[j+2]){
      count+=1;
      }
      else if(dta[j+1]<dta[j]&& dta[j+1]<dta[j+2]){
        count+=1;
        }
    }
  return count;
  }
 
 void Precision(){
    Motor1.setSpeed(100);
    Motor3.setSpeed(100);
    Motor2.setSpeed(0);
    Motor1.run(FORWARD);
    Motor3.run(FORWARD);
    Motor2.run(RELEASE);
    }

 
 void Open(){
    Motor1.setSpeed(100);
    Motor2.setSpeed(100);
    Motor3.setSpeed(100);
    Motor1.run(BACKWARD);
    Motor2.run(BACKWARD);
    Motor3.run(BACKWARD);
    }

 
 void Close(){
    Motor1.setSpeed(100);
    Motor2.setSpeed(100);
    Motor3.setSpeed(100);
    Motor1.run(FORWARD);
    Motor2.run(FORWARD);
    Motor3.run(FORWARD);
    }
