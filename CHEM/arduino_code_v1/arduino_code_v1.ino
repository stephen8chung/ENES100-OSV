/*
 * finley XAND stephen
 * 
*Code to be implemented into ardiuno chip to execute mission
*AKA
*THE FINAL CO(de)UNTDOWN
*
*note: dont plug rf into 0 and 1 until after the code sucessfuly uploaded to the arduino board
*note: WE NEED TO ADD CODE FOR THE MF DISTANCE SENSOR BOIS
*/
const int IN1=9;
const int IN2=8;
const int ENA=10;

const int IN3=12;
const int IN4=13;
const int ENB=11;

const int motorSpeed = 200;

const float rocksEndLocation = 2.0;//may need to change this, dependent on end location of rocky bois



Enes100 enes("Mad Scienctists", CHEMICAL, 7, 0, 1);

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

void loop() {//this is main.
  //adjusting y direction
up  moveToDesY();
  turnToRight();
  enes.println("At correct y, moving to correct x."); 
  //adjusting x direction
  //move until passed rocks
  updated();
  while(enes.location.x<rocksEndLocation){
    updated();
    moveForward();
  }
  enes.println("past the rocky bois");
  stopOSV();
  while(enes.location.x<(enes.destination.x-0.2)){
    updated();
    moveForward();
    avoidObstacle();
  }

  

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
  while(!isBlocked() && abs(enes.location.y - enes.destination.y) >0.5){
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
  while(!(enes.location.theta>1.55 && enes.location.theta <1.59)){
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
    while(!(abs(enes.location.theta)>-0.02 && abs(enes.location.theta)<0.2)){
      updated();
      rotateCCW();
    }
  }
  else{
    while(!(abs(enes.location.theta)>-0.02 && abs(enes.location.theta)<0.2)){
      updated();
      rotateCW();
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
    }

    turnToRight();
    updated();
    startLocation = enes.location.x;
    while(abs(enes.location.x-startLocation)<0.8 && !isBlocked()){
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
