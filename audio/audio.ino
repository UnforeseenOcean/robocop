//wave shield
#include <FatReader.h>
#include <SdReader.h>
#include <avr/pgmspace.h>
#include "WaveUtil.h"
#include "WaveHC.h"
#include <Wire.h>

SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the filesystem on the card
FatReader f;      // This holds the information for the file we're play
WaveHC wave;

#define DEBOUNCE 100  // button debouncer
int inByte = 0; 

boolean playingFile = false;

void setup(){
  Wire.begin(4); 
  Wire.onReceive(receiveEvent);
  
  Serial.begin(9600);

  setupAudio();
} 


void loop() {
  //inByte = Serial.read();

}

void receiveEvent(int howMany)
{
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
  if (x == 1) {
   playaudio(); 
  }
  if (x == 0) {
    stopaudio();
  }
  
  if (x == 2) {
    playaudio2();
  }
}


/////////AUDIO STUFFFF///////////////////

void playaudio(){
  if (playingFile == false) {
    playingFile = true;
    playfile("TEST.wav");
  }
}

void playaudio2(){
  playfile("ALARM.wav");
}

void stopaudio() {
  if (wave.isplaying) {
    wave.stop();
  }
  playingFile = false;
}

//============================== playcomplete() =============================
// Plays a full file from beginning to end with no pause.
void playcomplete(char *name) {
  // call our helper to find and play this name
  playfile(name);
  while (wave.isplaying) {
    //do nothing while its playing
  }
  // now its done playing
}

//============================== playfile() =============================
void playfile(char *name) {
  // see if the wave object is currently doing something
  if (wave.isplaying) {// already playing something, so stop it!
    wave.stop(); // stop it
  } 
  
  // look in the root directory and open the file
  if (!f.open(root, name)) {
    putstring("Couldn't open file "); 
    Serial.print(name); 
    return;
  }
  // OK read the file and turn it into a wave object
  if (!wave.create(f)) {
    putstring_nl("Not a valid WAV"); 
    return;
  }

  wave.play();

  // ok time to play! start playback

  //  wave.resume();
}

//============================== freeRam() =============================
// this handy function will return the number of bytes currently free in RAM, great for debugging!   
int freeRam(void){
  extern int  __bss_end; 
  extern int  *__brkval; 
  int free_memory; 
  if((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__bss_end); 
  }
  else {
    free_memory = ((int)&free_memory) - ((int)__brkval); 
  }
  return free_memory; 
} 

//============================== sdErrorCheck() =============================
void sdErrorCheck(void){
  if (!card.errorCode()) return;
  //  putstring("\n\rSD I/O error: ");
  //  Serial.print(card.errorCode(), HEX);
  //  putstring(", ");
  //  Serial.println(card.errorData(), HEX);
  while(1);
}

void setupAudio() {
  //*************
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(13, OUTPUT);

  // enable pull-up resistors on switch pins (analog inputs)
  digitalWrite(14, HIGH);
  digitalWrite(15, HIGH);
  digitalWrite(16, HIGH);
  digitalWrite(17, HIGH);
  digitalWrite(18, HIGH);
  digitalWrite(19, HIGH);

  if (!card.init(true)) { //play with 4 MHz spi if 8MHz isn't working for you
    //if (!card.init()) {         //play with 8 MHz spi (default faster!)  
    putstring_nl("Card init. failed!");  // Something went wrong, lets print out why
    sdErrorCheck();
    while(1);                            // then 'halt' - do nothing!
  }

  // enable optimize read - some cards may timeout. Disable if you're having problems
  //card.partialBlockRead(true);

  // Now we will look for a FAT partition!
  uint8_t part;
  for (part = 0; part < 5; part++) {     // we have up to 5 slots to look in
    if (vol.init(card, part)) 
      break;                             // we found one, lets bail
  }
  if (part == 5) {                       // if we ended up not finding one  :(
    putstring_nl("No valid FAT partition!");
    sdErrorCheck();      // Something went wrong, lets print out why
    while(1);                            // then 'halt' - do nothing!
  }

  // Lets tell the user about what we found
  putstring("Using partition ");
  Serial.print(part, DEC);
  putstring(", type is FAT");
  Serial.println(vol.fatType(),DEC);     // FAT16 or FAT32?

  // Try to open the root directory
  if (!root.openRoot(vol)) {
    putstring_nl("Can't open root dir!"); // Something went wrong,
    while(1);                             // then 'halt' - do nothing!
  } 

}



