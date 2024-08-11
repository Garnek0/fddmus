// FDDMUS - Arduino software

#include "packet.h"

typedef struct {
  uint8_t pinDir; //DIR# pin
  uint8_t pinStep; //STEP# pin
  uint8_t maxTrack; //Max track number the FDD supports (generally 80)
  uint8_t currentTrack; //internal
  bool dirFwd; //internal

  //moveHead = true -> range C0-~G4
  //moveHead = false -> range C0-~A#3

  //moveHead = true -> silent low nodes. Introduces a sound made by the head switching direction
  //                   which may reduce note quality.
  //moveHead = false -> loud and powerful low notes.
  bool moveHead;

  float note; //internal (set by packet)
  bool toggle; //internal (set by packet)
  double microsBetweenSteps; //internal
  long long nextStep; //internal
} drive_t;

#define DRIVE_COUNT 2 // Change this according to how many drives you have hooked up

drive_t drives[DRIVE_COUNT];

uint8_t buf[sizeof(packet_t)];
size_t receivedBytes;
packet_t packet;

void initDrive(drive_t* drive){
  pinMode(drive->pinDir, OUTPUT);
  pinMode(drive->pinStep, OUTPUT);

  digitalWrite(drive->pinDir, HIGH);

  for(int i = 0; i < drive->maxTrack; i++){
    digitalWrite(drive->pinStep, LOW);
    digitalWrite(drive->pinStep, HIGH);
    delay(5);
  }

  digitalWrite(drive->pinDir, LOW);

  drive->currentTrack = 0;
  drive->dirFwd = true;
  drive->toggle = NOTE_OFF;
  drive->note = 0;
  drive->nextStep = 0;
  drive->microsBetweenSteps = 0;
}

void setup() {
  Serial.begin(115200);

  // Configs for each drive

  // DRIVE 0

  drives[0].pinDir = 2;
  drives[0].pinStep = 3;
  drives[0].maxTrack = 80;
  drives[0].moveHead = false;

  // DRIVE 1

  drives[1].pinDir = 4;
  drives[1].pinStep = 5;
  drives[1].maxTrack = 80; 
  drives[1].moveHead = false;

  for(int i = 0; i < DRIVE_COUNT; i++){
    initDrive(&drives[i]);
  }
}

void loop() {
  // Check for incoming packets and build a buffer.
  if(Serial.available() > 0) {
    buf[receivedBytes] = Serial.read();

    receivedBytes++;
  }

  if(receivedBytes == sizeof(packet_t)){
    memcpy(&packet, buf, sizeof(packet_t));
    receivedBytes = 0;

    Serial.print("note: ");
    Serial.println(packet.note);
    Serial.print("toggle: ");
    Serial.println(packet.toggle);
    Serial.println("");

    if(packet.toggle) noteOn(packet.note);
    else noteOff(packet.note);

    Serial.write(PACKET_RESPONSE_RDY);
  }

  for(int i = 0; i < DRIVE_COUNT; i++){
    if(drives[i].toggle && (micros() >= drives[i].nextStep)){
      drives[i].nextStep = micros()+drives[i].microsBetweenSteps;
      step(&drives[i]);
    }
  }
}

void step(drive_t* drive){
  if(drive->moveHead){
    if(drive->dirFwd){
      digitalWrite(drive->pinDir, LOW);
    } else {
      digitalWrite(drive->pinDir, HIGH);
    }

    digitalWrite(drive->pinStep, LOW);
    digitalWrite(drive->pinStep, HIGH);

    if(drive->dirFwd){
      drive->currentTrack++;
      if(drive->currentTrack == drive->maxTrack-1){
        drive->dirFwd = false;
      }
    } else {
      drive->currentTrack--;
      if(drive->currentTrack == 0){
        drive->dirFwd = true;
      }
    }
  } else {
    if(drive->dirFwd){
      digitalWrite(drive->pinDir, LOW);
      drive->dirFwd = false;
    } else {
      digitalWrite(drive->pinDir, HIGH);
      drive->dirFwd = true;
    }

    digitalWrite(drive->pinStep, LOW);
    digitalWrite(drive->pinStep, HIGH);
  }
}

void noteOn(float freq){
  for(int i = 0; i < DRIVE_COUNT; i++){
    if(!drives[i].toggle){
      drives[i].toggle = NOTE_ON;
      drives[i].note = freq;
      if(drives[i].moveHead) drives[i].microsBetweenSteps = (double)(1/freq)*1000000;
      else drives[i].microsBetweenSteps = (double)(1/freq)*1000000/2;
      drives[i].nextStep = micros()+drives[i].microsBetweenSteps;

      break;
    }
  }
}

void noteOff(float freq){
  for(int i = 0; i < DRIVE_COUNT; i++){
    if(drives[i].toggle && drives[i].note == freq){
      drives[i].toggle = NOTE_OFF;
      break;
    }
  }
}
