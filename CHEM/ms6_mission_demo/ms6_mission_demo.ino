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
const int IN1=5;
const int IN2=4;
const int ENA=6;

const int IN3=8;
const int IN4=7;
const int ENB=9;

const int ARM1=3;
const int ARM2=5;
const int ARMEN=4;

const int pHIn=A0;

const int motorSpeed = 400;

const float rocksEndLocation = 0.0;



Enes100 enes("Mad Scientists", CHEMICAL, 15315, 0, 1);

void setup() {
  pinMode(IN1, OUTPUT);//motors
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  
  pinMode(IN4, OUTPUT);//Motors
  pinMode(IN3, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(ARM1, OUTPUT);//Motors
  pinMode(ARM2, OUTPUT);
  pinMode(ARMEN, OUTPUT);
  
  pinMode(pHIn, INPUT);
}

void loop() {//this is main?
  //adjusting y direction
  stopOSV();
  Serial.print("Start");
  moveForward();
  delay(30000);
  stopOSV();

  //base mission execution
  dropArm(255);
  delay(60000);
  //base mission transmission
  enes.print(0.01883 * analogRead(pHIn) - 0.06206);
  while(1);
}

void dropArm(int Speed){
  digitalWrite(ARM1,LOW); 
  digitalWrite(ARM2,HIGH);  
  analogWrite(ARMEN,Speed);
  delay(5000);
  digitalWrite(ARM1,LOW); 
  digitalWrite(ARM2,LOW);
}

void stopOSV(){
  motor1Brake();
  motor2Brake();
}

void moveForward(){
  motor1Forward(motorSpeed);
  motor2Forward(motorSpeed);
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
