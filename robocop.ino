#include <Servo.h> 

Servo leftArm[2];
Servo rightArm[2];
int leftPos[2];
int rightPos[2];

int leftFootPin = 0;
int rightFootPin = 1;

int leftFoot = 0;
int rightFoot = 0;

void setup(){
  Serial.begin(9600);

  leftArm[0].attach(9);
  leftArm[1].attach(10);
  for (int i=0; i < 2; i++) {
    leftArm[i].write(0);
    //rightArm[i].write(0);
  }
} 


void loop() {

  leftFoot = analogRead(leftFootPin);
  rightFoot = analogRead(rightFootPin);

  //printAnalog();

  moveArms();
  delay(1000);
  resetArms();
  delay(1000);
} 

void moveArms() {
  friskMotion(leftArm, 0, 100);

  for (int i = 0; i < 10; i++) {
    randomSeed(analogRead(5));
    int height = random(105, 130);
    friskMotion(leftArm, 20, height);
    friskMotion(leftArm, 0, height);    
  }

  friskMotion(leftArm, 0, 100);
}

//arm[0] should be the bottom servo, controlling distance of hand from butt
//arm[1] should be top servo, controlling the height of hand
void friskMotion(Servo arm[], int handDistance, int handHeight) {
  arm[1].write(handHeight);
  delay(200);
  arm[0].write(handDistance);
  delay(200);

}

void resetArms(){
  leftArm[0].write(0);
  leftArm[1].write(0);
}

void printAnalog(){
  Serial.print("Left foot: ");
  Serial.print(leftFoot);
  Serial.print(", Right foot: ");
  Serial.println(rightFoot);
  delay(1); 
}




