#include <Servo.h>
#include <Wire.h>

//servo setup
Servo arms[4];

int leftFootPin = 0;
int rightFootPin = 1;

int leftFoot = 0;
int rightFoot = 0;

//state variables
const int WAITING = 0;
const int FRISKING = 1;
int state = WAITING;
boolean playingFile = false;

//convience variables for arms
int servoPins[4];
const int LEFT_DISTANCE = 0;
const int LEFT_HEIGHT = 1;
const int RIGHT_DISTANCE = 2;
const int RIGHT_HEIGHT = 3;

//timing variables
unsigned long currentTime;
unsigned long startTime;
unsigned long timing[6][3];
int audioLength = 12000;
int handInterval = 100;


void setup(){
  Serial.begin(9600);
  Wire.begin();

  servoPins[LEFT_DISTANCE] = 6;
  servoPins[LEFT_HEIGHT] = 5;
  servoPins[RIGHT_DISTANCE] = 11;
  servoPins[RIGHT_HEIGHT] = 10;

  //set arms to zero and then detach them
  attachArms();
  resetArms();
  //detachArms();
} 


void loop() {
  currentTime = millis();

  leftFoot = analogRead(leftFootPin);
  rightFoot = analogRead(rightFootPin);
  printAnalog();

  if (feet() && state == WAITING) {
    startTime = currentTime;
    //attachArms();
    resetArms();
    setTiming();
    playaudio();
    //delay(1000);
    state = FRISKING;
    //Serial.println("STARTING");
  }

  if ((!feet() || startTime + audioLength < currentTime) && state == FRISKING) {
    state = WAITING;
    stopaudio();
    resetArms();
    delay(5000);
    //detachArms();
    //Serial.println("STOPPING");
  }

  if (state == FRISKING) {
    //Serial.println("FRISKING");
    frisk(); 
  }

  //for debugging
  //friskWithDelay();
  //frisk();
  //testServos();



}

/////////IS SOMEONE HERE?///////////////////
boolean feet() {
  return (leftFoot > 80 && rightFoot > 80);
}

/////////SERVO STUFFFF///////////////////
void resetArms() {
  for (int i = 0; i < 4; i++) {
    if (i == RIGHT_DISTANCE || i == RIGHT_HEIGHT) {
      arms[i].write(120); 
    } 
    else {
      arms[i].write(0);
    }
  }
}

void attachArms() {
  for (int i = 0; i < 4; i++) {
    arms[i].attach(servoPins[i]);
  }
}

void detachArms() {
  for (int i = 0; i < 4; i++) {
    arms[i].detach();
  } 
}

void frisk() {
  for (int i = 0; i < 6; i++) {

    if (timing[i][0] <= currentTime) {
      if (i == 0) {
        timing[i][2] = random(105, 130); 
      } 
      else if (i == 3) {
        timing[i][2] = random(10, 35); 
      }
      timing[i][0] += handInterval * 6;
      //timing[i][0] += (handInterval + random(50)) * 6;
      arms[timing[i][1]].write(timing[i][2]);
    }
  } 
}

void friskWithDelay() {
  for (int i = 0; i < 6; i++) {
    if (i == 0 || i == 3) {
      timing[i][2] = random(105, 130);
    }
    delay(handInterval);
    arms[timing[i][1]].write(timing[i][2]);
  }
} 


void setTiming(){
  for (int i = 0; i < 6; i++) {
    timing[i][0] = (i + 1) * handInterval + startTime;
    //timing[i][0] = (i + 1) * handInterval + random(50) + startTime;
  }

  //left arm
  timing[0][1] = LEFT_HEIGHT;
  timing[0][2] = random(105, 130);

  timing[1][1] = LEFT_DISTANCE;
  timing[1][2] = 80;

  timing[2][1] = LEFT_DISTANCE;
  timing[2][2] = 20;

  //right arm
  timing[3][1] = RIGHT_HEIGHT;
  timing[3][2] = random(10, 35);

  timing[4][1] = RIGHT_DISTANCE;
  timing[4][2] = 60;

  timing[5][1] = RIGHT_DISTANCE;
  timing[5][2] = 120;
}



/////////AUDIO STUFFFF///////////////////

void playaudio(){
  if (playingFile == false) {
    Wire.beginTransmission(4); 
    Wire.write(1);
    Wire.endTransmission(); 
  }
}

void playaudio2() {

}

void stopaudio() {
  Wire.beginTransmission(4); 
  Wire.write(0);
  Wire.endTransmission(); 
  playingFile = false;
}


/////////DEBUGGING STUFFFF///////////////////
void printAnalog(){
  Serial.print("Left foot: ");
  Serial.print(leftFoot);
  Serial.print(", Right foot: ");
  Serial.println(rightFoot);
  delay(1); 
}

void testServos() {
  for (int i = 0; i < 4; i++) {
    arms[i].write(random(20, 50));
    delay(100);
  } 
}







