/*
 * finley XAND stephen
 * 
*Code to be implemented into ardiuno chip to execute mission
*AKA
*THE FINAL CO(de)UNTDOWN
*
*NOTE: include location/angle/motion adjusters
*AVG time and success rate
*NOTE: dont avoid mission site!!!!!!!!!! try to figure this one out
*/

#include "Enes100.h"
#include <math.h>

const int thePieceOfWoodThatRepresentsOurLocationOnTopOfOurOSV = 3u;

const float toleranceRad = 0.05;

const int IN1=9;//right motor
const int IN2=8;
const int ENA=10;

const int IN3=12;//left motor
const int IN4=13;
const int ENB=11;

const int PUMP1=15;//Pump motor
const int PUMP2=16;
const int ENPUMP=17;

const int ARM1=2;
const int ARM2=3;
const int ENARM=4;

const int trigPin = 7;
const int echoPinR = 6;
const int echoPinL = 5;
long duration;
float m;

const int motorSpeed = 127;

const float rocksEndLocation = 2.0;//may need to change this, dependent on end location of rocky bois



Enes100 enes("MAD ZCIENTIZTZ", CHEMICAL, thePieceOfWoodThatRepresentsOurLocationOnTopOfOurOSV, 0, 1);

void setup() {
  pinMode(IN1, OUTPUT);//motors
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  
  pinMode(IN4, OUTPUT);//Motors
  pinMode(IN3, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(PUMP1, OUTPUT);//Pump motors
  pinMode(PUMP2, OUTPUT);
  pinMode(ENPUMP, OUTPUT);

  pinMode(trigPin, OUTPUT);//distance sensors
  pinMode(echoPinR, INPUT);
  pinMode(echoPinL, INPUT);

  Serial.begin(9600);
  Serial.println("Begin pH sensing");
  
  dropArm();
  startPump();
  delay(20000);
  stopPump();
  
}

void loop() {//this is main.
  readpH();
  //startPump();
  //base mission execution
  
  //base mission transmission
}

boolean distanceToTime(float distance){//distance in meters, time in milliseconds
  return distance / 3.1415926535897 / 0.12 / 30 * 60000;
}

void stopOSV(){
  motorRBrake();
  motorLBrake();
}

void moveForward(){
  motorRForward(motorSpeed);
  motorLForward(motorSpeed);
}

void updated(){
  while (!enes.updateLocation());
  enes.print("LOCATION : ");
  enes.print(enes.location.x);
  enes.print(",");
  enes.print(enes.location.y);
  enes.print(",");  
  enes.println(enes.location.theta);
}

void rotateCW(){
  motorLForward(motorSpeed);
  motorRBackward(motorSpeed);
}

void rotateCCW(){
  motorRForward(motorSpeed);
  motorLBackward(motorSpeed);
}

void motorLForward(int Speed) 
{
     digitalWrite(IN3,HIGH); 
     digitalWrite(IN4,LOW);  
     analogWrite(ENA,Speed);
}
  
void motorLBackward(int Speed) 
{    
     digitalWrite(IN3,LOW); 
     digitalWrite(IN4,HIGH);  
     analogWrite(ENA,Speed);
}
void motorLBrake()      
{
     digitalWrite(IN3,LOW); 
     digitalWrite(IN4,LOW); 
}     
void motorRForward(int Speed) 
{
     digitalWrite(IN1,LOW); 
     digitalWrite(IN2,HIGH);  
     analogWrite(ENB,Speed);
}
  
void motorRBackward(int Speed) 
{    
     digitalWrite(IN1,HIGH); 
     digitalWrite(IN2,LOW);  
     analogWrite(ENB,Speed);
}
void motorRBrake()
{
     digitalWrite(IN1,LOW); 
     digitalWrite(IN2,LOW); 
}

void moveToDesY(){
  updated();
  enes.print(enes.location.x);
    enes.print(",");
    enes.println(enes.location.y);
  if(enes.location.y < enes.destination.y){
    turnToUp();
    enes.println("turning to up i think");
  }
  else{
    turnToDown();
    enes.println("moving to down i think");
  }
  while(!isBlocked() && abs(enes.location.y - enes.destination.y) >0.05){
    moveForward();
    updated();
    enes.print(enes.location.x);
    enes.print(",");
    enes.println(enes.location.y);
  }
  if(isBlocked()){
    turnToRight();
    updated();
    float startLocation = enes.location.x;
    while(abs(enes.location.x - startLocation)<0.2 && !isBlocked()){
      updated();
      moveForward();
      enes.print(enes.location.x);
    enes.print(",");
    enes.println(enes.location.y);
    }
  }
  stopOSV();
}

void turnToUp(){//do initial check for theta
  updated();
  if(abs(enes.location.theta)<(PI/2)){//check current theta
    while(!(enes.location.theta < ((PI/2)+toleranceRad))){//turns until passes tolerance
      updated();
      rotateCCW();
      enes.println(enes.location.theta);
    }
    /*while(!(enes.location.theta>((PI/2)-toleranceRad))){//turns back until in tolerance if out of tolerance
      updated();
      rotateCW();
      enes.println(enes.location.theta);
    }
    if(!(enes.location.theta < ((PI/2)+toleranceRad) && (enes.location.theta>((PI/2)-toleranceRad)))){
      turnToUp();//recursive call
    }*/
    stopOSV();
  }
  else{//does same in opposite direction
    while(!(enes.location.theta>((PI/2)-toleranceRad))){
      updated();
      rotateCW();
      enes.println(enes.location.theta);
    }
    /*while(!(enes.location.theta < ((PI/2)+toleranceRad))){
      updated();
      rotateCCW();
      enes.println(enes.location.theta);
    }
    if(!(enes.location.theta < ((PI/2)+toleranceRad) && (enes.location.theta>((PI/2)-toleranceRad)))){
      turnToUp();//recursive call
    }*/
    stopOSV();
  }
  
}

void turnToDown(){//do check for initial theta for speed
  updated();
  if(abs(enes.location.theta)<(PI/2)){//check current theta
    while(!(enes.location.theta < (-1*(PI/2)+toleranceRad))){//turns until passes tolerance
      updated();
      rotateCW();
      enes.println(enes.location.theta);
    }
    /*while(!(enes.location.theta>(-1*(PI/2)-toleranceRad))){//turns back until in tolerance if out of tolerance
      updated();
      rotateCCW();
      enes.println(enes.location.theta);
    }
    if(!(enes.location.theta < (-1*(PI/2)+toleranceRad) && (enes.location.theta>(-1*(PI/2)-toleranceRad)))){
      turnToDown();//recursive call
    }*/
    stopOSV();
  }
  else{//does same in opposite direction
    while(!(enes.location.theta>(-1*(PI/2)-toleranceRad))){
      updated();
      rotateCCW();
      enes.println(enes.location.theta);
    }
    /*while(!(enes.location.theta < (-1*(PI/2)+toleranceRad))){
      updated();
      rotateCW();
      enes.println(enes.location.theta);
    }
    if(!(enes.location.theta < (-1*(PI/2)+toleranceRad) && (enes.location.theta>(-1*(PI/2)-toleranceRad)))){
      turnToDown();//recursive call
    }*/
    stopOSV();
  }
}

void turnToRight(){
  if(enes.location.theta < 0){//check current location
    while(!(abs(enes.location.theta)<(toleranceRad))){
      updated();
      enes.println(enes.location.theta);
      rotateCCW();
    }
    /*while(!(abs(enes.location.theta)>(-1*toleranceRad))){
      updated();
      rotateCW();
      enes.println(enes.location.theta);
    }
    if(!(abs(enes.location.theta)>(-1*toleranceRad) && abs(enes.location.theta)<(toleranceRad))){
      turnToRight();//recursively call to check again if not in constraints
    }*/
    stopOSV();
  }
  else{
    while(!(abs(enes.location.theta)>(-1*toleranceRad))){
      updated();
      rotateCW();
      enes.println(enes.location.theta);
    }
    /*while(!(abs(enes.location.theta)<(toleranceRad))){
      updated();
      rotateCCW();
      enes.println(enes.location.theta);
    }
    if(!(abs(enes.location.theta)>(-1*toleranceRad) && abs(enes.location.theta)<(toleranceRad))){
      turnToRight();//recursively call to check again if not in constraints
    }
    */
    stopOSV();
  }
}

boolean isBlocked(){//call sensor data
   if(distanceRight()<0.2 && distanceLeft()<0.2){
    return true;
   }
   else{
    return false;
   }
}

boolean isPathClear(){//call sensor data
  if(distanceRight()<0.5 && distanceLeft()<0.5){
    return true;
   }
   else{
    return false;
   }
}

boolean wallInFront(){
  updated();
  return (enes.location.y<0.3 && enes.location.theta<0.79) || (enes.location.y >1.7 && enes.location.theta>0.79);
}

void avoidObstacle(){
  if(isBlocked()){
    stopOSV();
    if(enes.location.y<0.5 || (enes.location.y < 1.5 &&random(1)>0.5)){
      turnToUp();
      if(isPathClear() &&!wallInFront()){
        turnToDown();
      }
    }
    else{
      turnToDown();
      if(isPathClear() && !wallInFront()){
        turnToUp();
      }
    }

    updated();
    float startLocation = enes.location.y;
    while(abs(enes.location.y-startLocation) < 0.5 && !isBlocked()){
      updated();
      moveForward();
      delay(500);
      stopOSV();
    }

    turnToRight();
    updated();
    startLocation = enes.location.x;
    while(abs(enes.location.x-startLocation)<0.8 && !isBlocked()){
      updated();
      moveForward();
      delay(500);
      stopOSV();
    }

    if(isBlocked()){
      avoidObstacle();
    }

    stopOSV();
    moveToDesY();
    turnToRight();
  }
}

float distanceRight(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPinR, HIGH);
  m = (2+(duration/2)/29.1)/100;
  return m;
}

float distanceLeft(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPinL, HIGH);
  m = (2+(duration/2)/29.1)/100;
  return m;
}

void startPump(){
  digitalWrite(PUMP1,LOW); 
  digitalWrite(PUMP2,HIGH);  
  //analogWrite(ENPUMP,255);
}

void stopPump(){
  digitalWrite(IN1,LOW); 
  digitalWrite(IN2,LOW);
}

void dropArm(){
  digitalWrite(ARM1,HIGH); 
  digitalWrite(ARM2,LOW); 
  delay(500);
  digitalWrite(ARM1,LOW);
  digitalWrite(ARM2,LOW);
}

void readpH(){
  float reading = analogRead(A0);
  Serial.print(reading);
  Serial.print("  ");
  Serial.println(0.01883 * reading - 0.06206);
}
