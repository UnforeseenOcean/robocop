#include <Servo.h>

Servo leftArm[2];
Servo rightArm[2];

Servo arms[4];

int leftFootPin = 0;
int rightFootPin = 1;

int leftFoot = 0;
int rightFoot = 0;

const int WAITING = 0;
const int FRISKING = 1;

int state = WAITING;

void setup(){
  Serial.begin(9600);

  leftArm[0].attach(5);
  leftArm[1].attach(6);

  rightArm[0].attach(9);
  rightArm[1].attach(10);

  arms[0] = leftArm[0];
  arms[1] = leftArm[1];
  arms[2] = rightArm[0];
  arms[3] = rightArm[1];

  for (int i=0; i < 4; i++) {
    arms[i].write(0);
  }
} 


void loop() {
  leftFoot = analogRead(leftFootPin);
  rightFoot = analogRead(rightFootPin);
  //printAnalog();

  if (leftFoot > 80 && state == WAITING) {
    moveArms();
    resetArms();
  }

} 

void moveArms() {
  friskMotion(leftArm, 0, 100);
  friskMotion(rightArm, 0, 100);

  for (int i = 0; i < 1; i++) {
    randomSeed(analogRead(5));
    int height = random(105, 130);
    friskMotion(leftArm, 20, height);
    friskMotion(leftArm, 0, height);
    randomSeed(analogRead(5));
    height = random(105, 130);
    friskMotion(rightArm, 20, height);
    friskMotion(rightArm, 0, height);    
    
  }

  friskMotion(leftArm, 0, 100);
  friskMotion(rightArm, 0, 100);

}

//arm[0] should be the bottom servo, controlling distance of hand from butt
//arm[1] should be top servo, controlling the height of hand
void friskMotion(Servo a[], int handDistance, int handHeight) {
  a[1].write(handHeight);
  delay(200);
  a[0].write(handDistance);
  delay(200);
}

void resetArms(){
  for (int i = 0; i < 4; i++) {
    arms[i].write(0);
  }
  state = WAITING;
}

void printAnalog(){
  Serial.print("Left foot: ");
  Serial.print(leftFoot);
  Serial.print(", Right foot: ");
  Serial.println(rightFoot);
  delay(1); 
}
