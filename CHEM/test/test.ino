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

const int thePieceOfWoodThatRepresentsOurLocationOnTopOfOurOSV = 11;

const float toleranceRad = 0.05;

const int IN1=9;//left motor
const int IN2=8;
const int ENA=10;

const int IN3=12;//right motor
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

const int motorSpeed =  255;

const float rocksEndLocation = 2.0;//may need to change this, dependent on end location of rocky bois



Enes100 enes("MAD ZCIENTIZTZ", CHEMICAL, thePieceOfWoodThatRepresentsOurLocationOnTopOfOurOSV, 0, 1);

void setup() {
  pinMode(IN1, OUTPUT);//motors
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  
  pinMode(IN4, OUTPUT);//Motors
  pinMode(IN3, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(trigPin, OUTPUT);//distance sensors
  pinMode(echoPinR, INPUT);
  pinMode(echoPinL, INPUT);  
}

void loop() {//this is main.
  //adjusting y direction
  updated();
  moveToDesY();
  turnToRight();
  enes.println("At correct y, moving to correct x."); 
  //adjusting x direction
  //move until passed rocks
  updated();
  moveForward();
  delay(distanceToTime(1.0));
  enes.println("past the rocky bois");
  stopOSV();
  delay(500);
  while(enes.location.x<(enes.destination.x-0.2)){
    updated();
    moveForward();
    avoidObstacle();
    delay(500 + (enes.destination.x - enes.location.x) * 1500);
    stopOSV();
  }
  stopOSV();
  while(1);
  //base mission execution
  
  //base mission transmission
}

float distanceToTime(float distance){//distance in meters, time in milliseconds
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
  while (!enes.retrieveDestination());
  enes.print("LOCATION : ");
  enes.print(enes.location.x);
  enes.print(",");
  enes.print(enes.location.y);
  enes.print(",");  
  enes.println(enes.location.theta);
}

void rotateCW(int mSpeed){
  motorLForward(mSpeed);
  motorRBackward(mSpeed);
}

void rotateCCW(int mSpeed){
  motorRForward(mSpeed);
  motorLBackward(mSpeed);
}

void motorLForward(int Speed){
     digitalWrite(IN3,HIGH); 
     digitalWrite(IN4,LOW);  
     analogWrite(ENB,Speed);
}
  
void motorLBackward(int Speed){    
     digitalWrite(IN3,LOW); 
     digitalWrite(IN4,HIGH);  
     analogWrite(ENB,Speed);
}
void motorLBrake(){
     digitalWrite(IN3,LOW); 
     digitalWrite(IN4,LOW); 
}     
void motorRForward(int Speed){
     digitalWrite(IN1,LOW); 
     digitalWrite(IN2,HIGH);  
     analogWrite(ENA,Speed);
}
  
void motorRBackward(int Speed){    
     digitalWrite(IN1,HIGH); 
     digitalWrite(IN2,LOW);  
     analogWrite(ENA,Speed);
}
void motorRBrake(){
     digitalWrite(IN1,LOW); 
     digitalWrite(IN2,LOW); 
}

void moveToDesY(){
  updated();
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
  }
  if(isBlocked()){
    turnToRight();
    updated();
    float startLocation = enes.location.x;
    while(abs(enes.location.x - startLocation)<0.2 && !isBlocked()){
      updated();
      moveForward();
    }
  }
  stopOSV();
}

void turnToUp(){//do initial check for theta
  updated();
  if(abs(enes.location.theta)<(PI/2)){//check current theta
    int speed1 =motorSpeed/2;
    while(!(enes.location.theta < ((PI/2)+toleranceRad))){//turns until passes tolerance
      updated();
      if(speed1<motorSpeed){
        speed1++;
        delay(10);
      }
      rotateCCW(speed1);
      enes.println(enes.location.theta);
    }
    while(!(enes.location.theta>((PI/2)-toleranceRad))){//turns back until in tolerance if out of tolerance
      updated();
      if(speed1<motorSpeed){
        speed1++;
        delay(10);
      }
      rotateCW(speed1/10);
      enes.println(enes.location.theta);
    }
    if(!(enes.location.theta < ((PI/2)+toleranceRad) && (enes.location.theta>((PI/2)-toleranceRad)))){
      turnToUp();//recursive call
    }
    stopOSV();
  }
  else{//does same in opposite direction
    int speed1 =motorSpeed/2;
    while(!(enes.location.theta>((PI/2)-toleranceRad))){
      updated();
      if(speed1<motorSpeed){
        speed1++;
      }
      rotateCW(speed1);
      enes.println(enes.location.theta);
      delay(10);
    }
    while(!(enes.location.theta < ((PI/2)+toleranceRad))){
      updated();
      if(speed1<motorSpeed){
        speed1++;
      }
      rotateCCW(speed1/10);
      enes.println(enes.location.theta);
      delay(10);
    }
    if(!(enes.location.theta < ((PI/2)+toleranceRad) && (enes.location.theta>((PI/2)-toleranceRad)))){
      turnToUp();//recursive call
    }
    stopOSV();
  }
  
}

void turnToDown(){//do check for initial theta for speed
  updated();
  if(abs(enes.location.theta)<(PI/2)){//check current theta
    int speed1 = motorSpeed/2;
    while(!(enes.location.theta < (-1*(PI/2)+toleranceRad))){//turns until passes tolerance
      updated();
      if(speed1<motorSpeed){
        speed1++;
      }
      rotateCW(speed1);
      enes.println(enes.location.theta);
      delay(10);
    }
    while(!(enes.location.theta>(-1*(PI/2)-toleranceRad))){//turns back until in tolerance if out of tolerance
      updated();
      if(speed1<motorSpeed){
        speed1++;
      }
      rotateCCW(speed1/10);
      enes.println(enes.location.theta);
      delay(10);
    }
    if(!(enes.location.theta < (-1*(PI/2)+toleranceRad) && (enes.location.theta>(-1*(PI/2)-toleranceRad)))){
      turnToDown();//recursive call
    }
    stopOSV();
  }
  else{//does same in opposite direction
    int speed1=motorSpeed/2;
    while(!(enes.location.theta>(-1*(PI/2)-toleranceRad))){
      updated();
      if(speed1<motorSpeed){
        speed1++;
      }
      rotateCCW(speed1);
      enes.println(enes.location.theta);
      delay(10);
    }
    while(!(enes.location.theta < (-1*(PI/2)+toleranceRad))){
      updated();
      if(speed1<motorSpeed){
        speed1++;
      }
      rotateCW(speed1/10);
      enes.println(enes.location.theta);
      delay(10);
    }
    if(!(enes.location.theta < (-1*(PI/2)+toleranceRad) && (enes.location.theta>(-1*(PI/2)-toleranceRad)))){
      turnToDown();//recursive call
    }
    stopOSV();
  }
}

void turnToRight(){
  if(enes.location.theta < 0){//check current location
    int speed1=motorSpeed/2;
    while(!(abs(enes.location.theta)<(toleranceRad))){
      updated();
      if(speed1<motorSpeed){
        speed1++;
      }
      enes.println(enes.location.theta);
      rotateCCW(speed1);
      delay(10);
    }
    while(!(abs(enes.location.theta)>(-1*toleranceRad))){
      updated();
      if(speed1<motorSpeed){
        speed1++;
      }
      rotateCW(speed1/10);
      enes.println(enes.location.theta);
      delay(10);
    }
    if(!(abs(enes.location.theta)>(-1*toleranceRad) && abs(enes.location.theta)<(toleranceRad))){
      turnToRight();//recursively call to check again if not in constraints
    }
    stopOSV();
  }
  else{
    int speed1=motorSpeed/2;
    while(!(abs(enes.location.theta)>(-1*toleranceRad))){
      updated();
      if(speed1<motorSpeed){
        speed1++;
      }
      rotateCW(speed1);
      enes.println(enes.location.theta);
      delay(10);
    }
    while(!(abs(enes.location.theta)<(toleranceRad))){
      updated();
      if(speed1<motorSpeed){
        speed1++;
      }
      rotateCCW(speed1/10);
      enes.println(enes.location.theta);
      delay(10);
    }
    if(!(abs(enes.location.theta)>(-1*toleranceRad) && abs(enes.location.theta)<(toleranceRad))){
      turnToRight();//recursively call to check again if not in constraints
    }
    stopOSV();
  }
}

boolean isBlocked(){//call sensor data
  updated();
   if(enes.location.x > 1.2 && distanceRight()<0.2 && distanceLeft()<0.2){
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
  enes.println(0.01883 * reading - 0.06206);
}
