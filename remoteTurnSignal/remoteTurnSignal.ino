// ----------LIBRARIES--------------
#include <IRremote.h>
#include <IRremoteInt.h>

// --------CONSTANTS (won't change)---------------
const int RECV_PIN = 8;
IRrecv irrecv(RECV_PIN);
decode_results results;
const int ledLeft = 13;
const int ledRight = 12;
const int toggleDelay = 100;
const int interval = 500; // number of millisecs between blinks
const int blinkDuration = 500; // number of millisecs that Led's are on - all three leds use this

#define code1  16
#define code2  2064


//------------ VARIABLES (will change)---------------------

int ledAssignmentStatus[] = {0, 0};
unsigned long currentMillis = 0;
unsigned long previousToggle[] = {0, 0};
byte ledState[] = {LOW, LOW};
unsigned long previousOnBoardLedMillis[] = {0, 0};  // will store last time the LED was updated


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  irrecv.enableIRIn(); // Start the receiver
  // initialize the digital pin as an output.
  pinMode(ledLeft, OUTPUT);
  pinMode(ledRight, OUTPUT);
}

void loop() {
  currentMillis = millis();

  //define the behavior in the most general level: 
  //when led is assigned, it blinks, otherwise it is off
  if (ledAssignmentStatus[0] == 1) {
    turnSignal(ledLeft, 0); //blinking
  }
  else {
     digitalWrite(ledLeft, LOW); //off
  }

  if (ledAssignmentStatus[1] == 1) {
    turnSignal(ledRight, 1); //blinking
  }
  else {
     digitalWrite(ledRight, LOW); //off
  }

  //define the behavior when receiving IR signal: toggle the LED assignment
  if (irrecv.decode(&results)) {
    unsigned int receivedCode = results.value;

    switch (receivedCode) {
      case code1:
        toggleLed(0); //0 is the index for left lamp
        break;
      case code2:
        toggleLed(1); //1 is the index for left lamp
        break;

    }

    irrecv.resume(); // Receive the next value
  }
}

//---------FUNCTIONS---------
//define the functions used in the above code

void toggleLed (int ledIndex) {
//assigned means blinking (or do whatever pattern it is assigned for)
//unassigned means off. 
//(the term on and off is not used here because it is used in the pattern definition)
  if (currentMillis - previousToggle[ledIndex] >= toggleDelay) {
    if (ledAssignmentStatus[ledIndex] == 1) {      // if the led is assigned then
      ledAssignmentStatus[ledIndex] = 0;           // set its state as unassigned
    } else {                                       // else if the led is assigned
      ledAssignmentStatus[ledIndex] = 1;           // set its state as unassigned
    }
    previousToggle[ledIndex] = currentMillis;
  }
}

//define the pattern
void turnSignal(int ledPinNum, int ledIndex) {
//on and off here means literally on and off (HIGH and LOW)
  if (ledState[ledIndex] == LOW) {

    // if the LED is off, we must wait for the interval to expire before turning it on
    if (currentMillis - previousOnBoardLedMillis[ledIndex] >= interval) {
      // time is up, so change the state to HIGH
      ledState[ledIndex] = HIGH;
      digitalWrite(ledPinNum, HIGH);
      // and save the time when we made the change
      previousOnBoardLedMillis[ledIndex] += interval;

      // NOTE: The previous line could alternatively be
      //        previousOnBoardLedMillis = currentMillis
      //        which is the style used in the BlinkWithoutDelay example sketch
      //        However, adding on the interval is a better way to ensure that succesive periods are identical

    }
  }
  else {  // i.e. if onBoardLedState is HIGH

    // if the LED is on, we must wait for the duration to expire before turning it off
    if (currentMillis - previousOnBoardLedMillis[ledIndex] >= blinkDuration) {
      // time is up, so change the state to LOW
      ledState[ledIndex] = LOW;
      digitalWrite(ledPinNum, LOW);
      // and save the time when we made the change
      previousOnBoardLedMillis[ledIndex] += blinkDuration;
    }
  }
}

