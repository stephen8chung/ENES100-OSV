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

const int motorSpeed = 400;

const float rocksEndLocation = 0.0;



Enes100 enes("Mad Scienctists", CHEMICAL, 7, 10, 11);

void setup() {
  pinMode(IN1, OUTPUT);//motors
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  
  pinMode(IN4, OUTPUT);//Motors
  pinMode(IN3, OUTPUT);
  pinMode(ENB, OUTPUT);

  while (!enes.retrieveDestination()) {
    enes.println("Unable to retrieve location");
  }

  enes.print("My destination is at ");
  enes.print(enes.destination.x);
  enes.print(",");
  enes.println(enes.destination.y);
  
}

void loop() {//this is main?
  //adjusting y direction
  updated();
  moveToDesY();
  turnToZero();
  enes.println("At correct y, moving to correct x."); 
  //adjusting x direction
  //move until passed rocks
  updated();
  while(enes.location.x<rocksEndLocaiton){
    updated();
    moveForward();
    avoidObstacle();
  }
  stopOSV();

  //base mission execution
  
  //base mission transmission
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

void moveToDesY(){
  updated();
  if(enes.location.y < enes.destination.y){
    turnToUp();    
  }
  else{
    turnToDown();
  }
  while(!isBlocked() && abs(enes.location.y - enes.destination) >0.5){
    moveForward();
    updated();
  }
  if(isBlocked()){
    turnRight()
    updated();
    float startLocation = enes.location.x;
    while(abs(enes.location.x - startl=Location)<0.2 && !isBlocked()){
      updated();
      moveForward();
    }
  }
  stopOSV();
}

void turnToUp(){//do initial check for theta
  while(!(enes.location.theta>1.55 && enes.location.theta <1.59){
    updated();
    rotateCCW();
  }
  stopOSV();
}

void turnToDown(){//do check for initial theta for speed
  while(!(enes.location.theta>-1.59 && enes.location.theta < -1.55)){
    updated();
    rotateCW();
  }
  stopOSV();
}

void turnToRight(){
  if(enes.location.theta < 0){
    while(!(abs(enes.location.theta)>-0.02 && abs(enes.location.theta)<0.2){
      updated();
      rotatCCW();
    }
  }
  else{
    while(!(abs(enes.location.theta)>-0.02 && abs(enes.location.theta)<0.2){
      updated();
      rotatCW();
    }
  }
  stopOSV();
}

boolean isBlocked(){//call sensor data
  
}

boolean isPathClear(){//call sensor data
  
}

boolean wallInFront(){
  updated();
  return (enes.location.y<0.3 && enes.location.theta<0.79) || (enes.location.y >1.7 && enes.location.theta>0.79)
}

void avoidObstacle(){
  if(isBlocked()){
    stopOSV();
    if(enes.location.y<0.5 || (enes.location.y < 1.5 &&random(1)>0.5){
      turnToUp();
      if(pathIsClear() &&!wallInFront()){
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
    }

    turnToRight();
    updated();
    startLocation = enes.location.x;
    while(abs(enes.location.x-startLocation)<0.8 && !isBlocaked()){
      updated();
      moveForward();
    }

    if(isBlocked()){
      avoidObstacle();
    }

    stopOSV();
    moveToDesY();
    turnToRight();
  }
}
