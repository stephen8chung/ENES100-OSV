/*
 * Finley Potter & Stephen Chung
 * Navigation SIMULATION code
 * Mad Zientiztz 
 */

#include "Enes100Simulation.h"
#include "DFRTankSimulation.h"

#define abs(x) ((x)>0?(x):-(x))

Enes100Simulation enes; //create enes obj
DFRTankSimulation tank; //create tank obj

int distanceSensorNum = 2;

bool goDown;

void setup() {
  tank.init(); // initialize tank nav code
  while(!enes.retrieveDestination());//updates vars
  while(!enes.updateLocation());//updates vars
  enes.println("start");
  
}

void loop() {//this is main
  goDown = true;
  //Y DIRECTION
  moveToDesY();
  enes.println("at mission y coordinate");
  tank.turnOffMotors();

  turnToZero();
  enes.println("turned towards mission site");

  //X DIRECTION

  while (enes.location.x < enes.destination.x - 0.25){//Tolerance of 5-10cm away from destination
      while(!enes.updateLocation());
      moveForward();//Moves forward and checks for obstacles
      avoidObstacle();
  }
  tank.turnOffMotors();

//call mission prgram here in main

  while(1);//this stops the program
} 


//additional functions you can call in main loop
void turnToZero(){
  while(abs(enes.location.theta) > 0.02) { //this loop turns the OSV towards the mission site within a tolerance of +- 0.02 rads
    if(enes.location.theta > 1.55 && enes.location.theta < 1.59){
      tank.setLeftMotorPWM(63);
      tank.setRightMotorPWM(-63);
    }
    if(enes.location.theta < -1.55 && enes.location.theta > -1.59){
      tank.setLeftMotorPWM(-63);
      tank.setRightMotorPWM(63);
      enes.readDistanceSensor(2);
    }
    while(!enes.updateLocation());
  }
  tank.turnOffMotors();
}

void turnToPi(){
  while(enes.location.theta > PI-0.02 && enes.location.theta < -(PI)+0.02) { //this loop turns the OSV towards the mission site within a tolerance of +- 0.02 rads
    if(enes.location.theta > 1.55 && enes.location.theta < 1.59){
      tank.setLeftMotorPWM(-63);
      tank.setRightMotorPWM(63);
    }
    if(enes.location.theta < -1.55 && enes.location.theta > -1.59){
      tank.setLeftMotorPWM(63);
      tank.setRightMotorPWM(-63);
    }
    while(!enes.updateLocation());
  }
  tank.turnOffMotors();
}

void turnToDown(){
  while(!(enes.location.theta < -1.55 && enes.location.theta > -1.59)) { //turns OSV down
    while(!enes.updateLocation());
    tank.setLeftMotorPWM(63);
    tank.setRightMotorPWM(-63);
  }
  tank.turnOffMotors();
}

void turnToUp(){
  while(!(enes.location.theta > 1.55 && enes.location.theta < 1.59)) { //turns OSV down
    while(!enes.updateLocation());
    tank.setLeftMotorPWM(-63);
    tank.setRightMotorPWM(63);
  }
  tank.turnOffMotors();
}

void avoidObstacle(){
  if (isBlocked()){//Checks distance sensors
    enes.println("obstacle detected");
    enes.println(enes.readDistanceSensor(0));
    enes.println(enes.readDistanceSensor(1));
    enes.println(enes.readDistanceSensor(2));
    enes.println(enes.readDistanceSensor(0) < 0.15 || enes.readDistanceSensor(1) < 0.15 || enes.readDistanceSensor(2) < 0.15);
    tank.turnOffMotors();//Stops from moving foward
    if (enes.location.y < 0.5 || (enes.location.y < 1.50 && random(1) > 0.5)){//If on the lower half of the field
      turnUp();
      if (isPathClear() && !wallInFront()){//If obstacle right above OSV
        turnDown();
      }
    }else{
      turnDown();
      if(isPathClear() && !wallInFront()){//If obstacle right below OSV
        turnUp();
      }
    }
    enes.println("turned vertical");
    
    while(!enes.updateLocation());
    float startLocation = enes.location.y;
    enes.println(startLocation);
    while (abs(enes.location.y - startLocation) < 0.50 && !isBlocked()){//Moving 30cm up/down from original position to get out the way of obstacle
      while(!enes.updateLocation());
      //enes.println(enes.location.y);
      moveForward();
    }
    enes.println("moved out of the way in y");
    tank.turnOffMotors();

    turnRight();
    enes.println("turned right");
    while(!enes.updateLocation());
    startLocation = enes.location.x;
    while (abs(enes.location.x - startLocation) < 0.8 && !isBlocked()){//Moving past the obstacle
      while(!enes.updateLocation());
      moveForward();
    }
    enes.println("cleared x direction");
    if (isBlocked()){//Checks if more obstacles, repeats procedure
      avoidObstacle();
    }
    tank.turnOffMotors();
    enes.println("moving back to correct y");
    moveToDesY();
    turnRight();
  }
}

boolean isBlocked(){
  return enes.readDistanceSensor(0) < 0.15 || enes.readDistanceSensor(1) < 0.15 || enes.readDistanceSensor(2) < 0.15;
}

boolean isPathClear(){
  return enes.readDistanceSensor(0) < 0.50 || enes.readDistanceSensor(1) < 0.50 || enes.readDistanceSensor(2) < 0.50;
}

boolean wallInFront(){
   return (enes.location.y < 0.30 && enes.location.theta < -0.79) || (enes.location.y > 1.70 && enes.location.theta > 0.79);
}

long distance(long microseconds){
  return microseconds / 29 / 2;
}

void moveForward(){
  tank.setLeftMotorPWM(63);
  tank.setRightMotorPWM(63);
}

void moveBackward(){
  tank.setLeftMotorPWM(-63);
  tank.setRightMotorPWM(-63);
}

void rotateCW(){
  tank.setLeftMotorPWM(63);
  tank.setRightMotorPWM(-63);
}

void rotateCCW(){
  tank.setLeftMotorPWM(-63);
  tank.setRightMotorPWM(63);
}

void turnUp(){
  while(!(enes.location.theta > 1.55 && enes.location.theta < 1.59)) {
    while(!enes.updateLocation());
    enes.println(enes.location.theta);
    rotateCCW();
    delay(50);
  }
  tank.turnOffMotors();
}

void turnDown(){
  while(!(enes.location.theta > -1.59 && enes.location.theta < -1.55)) {
    while(!enes.updateLocation());
    enes.println(enes.location.theta);
    rotateCW();
    delay(50);
  }
  tank.turnOffMotors();
}

void turnRight(){
  if (enes.location.theta < 0){
    while(!(abs(enes.location.theta) > -0.02 && abs(enes.location.theta) < 0.02)) { //If the OSV is facing upwards initially
      while(!enes.updateLocation());
      rotateCCW();
    }
  }else{
    while(!(abs(enes.location.theta) > -0.02 && abs(enes.location.theta) < 0.02)) { //If the OSV is facing downwards initially
      while(!enes.updateLocation());
      rotateCW();
    }
  }
  tank.turnOffMotors();
}

void moveToDesY(){
  while(!enes.updateLocation());
  if (enes.location.y < enes.destination.y){//If below destination y coordinate
      enes.println("turning up");
      turnUp();
      enes.println("moving up");
    }else{
      enes.println("turning down");
      turnDown();
      enes.println("moving down");
    }
    while (!isBlocked() && abs(enes.location.y - enes.destination.y) > 0.05){//Moves back to destination y
        while(!enes.updateLocation());
        moveForward();
    }
    if (isBlocked()){
      turnRight();
      while(!enes.updateLocation());
      float startLocation = enes.location.x;
      while (abs(enes.location.x - startLocation) < 0.2 && !isBlocked()){//Moving past the obstacle
        while(!enes.updateLocation());
        moveForward();
      }
    }
    tank.turnOffMotors();
}
