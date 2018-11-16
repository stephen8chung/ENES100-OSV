#include <Enes100.h>

/*
 * finley XAND stephen
 * 
*Code to be implemented into ardiuno chip to execute mission
*AKA
*THE FINAL CO(de)UNTDOWN
*
*NOTE: wait until over rocks for checking obstacle
*NOTE: loaction and adjust after rocks
*NOTE: side sensors and stuff
*NOTE: speedyboi osv, turn until clear then full send set dist, then orient in direction of dest
*AVG time and success rate
*NOTE: dont avoid mission site
*loop v setup
*/
const int IN1=9;
const int IN2=8;
const int ENA=10;

const int IN3=12;
const int IN4=13;
const int ENB=11;

const int ARM1=2;
const int ARM2=4;
const int ARMEN=3;

const int pHIn=A0;

const int motorSpeed = 400;

const float rocksEndLocation = 0.0;



Enes100 enes("Mad Scientists", CHEMICAL, 15315, 0, 1);

void setup() {
  stopOSV();
  pinMode(IN1, OUTPUT);//motors
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  stopOSV();
  pinMode(IN4, OUTPUT);//Motors
  pinMode(IN3, OUTPUT);
  pinMode(ENB, OUTPUT);
  stopOSV();
  pinMode(ARM1, OUTPUT);//Motors
  pinMode(ARM2, OUTPUT);
  pinMode(ARMEN, OUTPUT);
  stopOSV();
  pinMode(pHIn, INPUT);
  stopOSV();
}

void loop() {//this is main?
  //adjusting y direction
  stopOSV();
  Serial.print("Start");
  moveForward();
  delay(7305);
  stopOSV();
  enes.navigated();

  //base mission execution*/
  dropArm(200);
  delay(30000);
  //base mission transmission
  enes.println(0.01-analogRead(pHIn) - 0.06206);
  while(1);
}

void dropArm(int Speed){
  digitalWrite(ARM1,LOW); 
  digitalWrite(ARM2,HIGH);  
  analogWrite(ARMEN,Speed);
  delay(500);
  digitalWrite(ARM1,LOW); 
  digitalWrite(ARM2,LOW);
}

void stopOSV(){
  motor1Brake();
  motor2Brake();
}

void moveForward(){
  motor1Backward(motorSpeed);
  motor2Backward(motorSpeed);
}

void updated(){
  while (!enes.updateLocation());
}

void rotateCW(){
  motor1Forward(motorSpeed);
  motor2Backward(motorSpeed);
}

void rotateCCW(){
  motor2Forward(motorSpeed);
  motor1Backward(motorSpeed);
}

void motor1Forward(int Speed) 
{
     digitalWrite(IN1,HIGH); 
     digitalWrite(IN2,LOW);  
     analogWrite(ENA,Speed);
}
  
void motor1Backward(int Speed) 
{    
     digitalWrite(IN1,LOW); 
     digitalWrite(IN2,HIGH);  
     analogWrite(ENA,Speed);
}
void motor1Brake()      
{
     digitalWrite(IN1,LOW); 
     digitalWrite(IN2,LOW); 
}     
void motor2Forward(int Speed) 
{
     digitalWrite(IN3,LOW); 
     digitalWrite(IN4,HIGH);  
     analogWrite(ENB,Speed);
}
  
void motor2Backward(int Speed) 
{    
     digitalWrite(IN3,HIGH); 
     digitalWrite(IN4,LOW);  
     analogWrite(ENB,Speed);
}
void motor2Brake()
{
     digitalWrite(IN3,LOW); 
     digitalWrite(IN4,LOW); 
}
