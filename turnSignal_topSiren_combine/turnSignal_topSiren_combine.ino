//episode5: various blink pattern with multiple LEDs: combining the two

// ----------LIBRARIES--------------
#include <IRremote.h>

// --------CONSTANTS (won't change)---------------
//related to remote
const int RECV_PIN = 8;
IRrecv irrecv(RECV_PIN);
decode_results results;
const int toggleDelay = 100; //to avoid toggling flicker

//LED PIN assignment
const int LEFT_TURN_LED = 13;
const int RIGHT_TURN_LED = 12;
const int SIREN_TOP_IN_LED = 11;
const int SIREN_TOP_OUT_LED = 10;

//constants for turn-signal LED
const int turnSignalInterval = 500;
const int turnSignalDuration = 500;

//constants for top-siren LED
const int topSirenInterval = 80;
const int topSirenDuration = 80;
const int cycleLength = 2 * topSirenInterval + 2 * topSirenDuration;

//IR remote codes
const int code1 = 0x10;  //code for turn left lamp
const int code2 = 0x810; //code for turn right lamp
const int code3 = 0x410; //code for hazard lamp (both turn lamp)
const int code4 = 0xC10; //code for top-siren

//------------ VARIABLES (will change)---------------------
unsigned long currentMillis = 0;

//arrays related to pattern
int patternStatus[] = {0, 0, 0, 0}; //{left, right, hazard, top-siren}
unsigned long previousToggle[] = {0, 0, 0, 0}; // {left, right, hazard, top-siren}
                                               //  to avoid toggling flicker

//arrays related to LED
byte ledState[] = {LOW, LOW, LOW, LOW}; // {left-turn, right-turn, in-top-siren, out-top-siren}
unsigned long previousUpdate[] = {0, 0, 0, 0};  // will store last time each LED was updated

//auxiliary variable for siren
unsigned long previousCycle = 0; 
int sirenSwapStatus = 0; //determines which LED is blinking. 0 for inside, 1 for outside

//========== THE SETUP ==============================

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  irrecv.enableIRIn(); // start the receiver

  pinMode(LEFT_TURN_LED, OUTPUT);
  pinMode(RIGHT_TURN_LED, OUTPUT);
  pinMode(SIREN_TOP_IN_LED, OUTPUT);
  pinMode(SIREN_TOP_OUT_LED, OUTPUT);
}

//========== THE LOOP ==============================

void loop() {
  currentMillis = millis();

//----------the logic--------------------
//define the general behavior when a remote button is pressed
//note: LED (logical) state does not necessarily the same with the physical state (ON/OFF)
//because the physical state can be determined by more than 1 logical state
  if (irrecv.decode(&results)) {
    unsigned int receivedCode = results.value;

    switch (receivedCode) {
      case code1:
        toggleButton(0);//0 is the index for turn left lamp
        //also need to change the state of the right lamp to OFF if the state is BLINKING
        if (patternStatus[1] == 1) {
          toggleButton(1); 
        }
        break;
      case code2:
        toggleButton(1); //1 is the index for turn right lamp
        //also need to change the state of the left lamp to OFF if the state is BLINKING
        if (patternStatus[0] == 1) {
          toggleButton(0); 
        }
        break;
      case code3:
        toggleButton(2); //2 is the index for hazard lamp (both lamp)
        break;
      case code4:
        toggleButton(3); //3 is the index for top siren
        break;
    }
     irrecv.resume(); // Receive the next value

  }

//----------the execution-------------------
  switchTheLeds () ; //the actual function that switch the LED ON or OFF based on the value of their state variable

}

// --------functions definition---------------


void toggleButton (int patternIndex) {  //the function that turns a pattern ON or OFF
                                        //the ledIndex argument determines which pattern is being toggled
  if ((unsigned long)currentMillis - previousToggle[patternIndex] >= toggleDelay) {
    if (patternStatus[patternIndex] == 1) {       // if pattern is on then
      patternStatus[patternIndex] = 0;            // set its state as off
    } else {                                      // else if pattern is off
      patternStatus[patternIndex] = 1;            // set its state as on
    }
    previousToggle[patternIndex] = currentMillis;
  }
}

void blinkPattern(int ledPinNum, int ledIndex, int interval, int duration) { //this is the function that defines the pattern
  if (ledState[ledIndex] == LOW) {  // if the LED is OFF
    if ((unsigned long)currentMillis - previousUpdate[ledIndex] >= interval) {
      ledState[ledIndex] = HIGH;
      digitalWrite(ledPinNum, HIGH);
      // and save the time when we made the change
      previousUpdate[ledIndex] += interval;
      }
  }
  else {  // i.e. if the LED is ON
    if (currentMillis - previousUpdate[ledIndex] >= duration) {
      ledState[ledIndex] = LOW;
      digitalWrite(ledPinNum, LOW);
      // and save the time when we made the change
      previousUpdate[ledIndex] += duration;
    }
  }
}

void switchTheLeds () {
//for the turning lamp
  if (patternStatus[0] == 1 || patternStatus[2] == 1) { //if turning left or hazard is on
    blinkPattern(LEFT_TURN_LED, 0, turnSignalInterval,turnSignalDuration );  //then left lamp is blinking
  }
  else {
    digitalWrite(LEFT_TURN_LED, LOW);
  }

  if (patternStatus[1] == 1 || patternStatus[2] == 1) { //if turning right or hazard is on
     blinkPattern(RIGHT_TURN_LED, 1, turnSignalInterval,turnSignalDuration );  //then right lamp is on blinking
  }
  else {
    digitalWrite(RIGHT_TURN_LED, LOW);
  }

//for the siren lamp
if (patternStatus[3] == 1) { //if siren is toggled to on
    swapTheSiren(); 
    if (sirenSwapStatus == 0) { 
      blinkPattern(SIREN_TOP_IN_LED, 2, topSirenInterval, topSirenDuration);
      digitalWrite(SIREN_TOP_OUT_LED, LOW);
    }
    else {
      blinkPattern(SIREN_TOP_OUT_LED, 3, topSirenInterval, topSirenDuration);
      digitalWrite(SIREN_TOP_IN_LED, LOW);
    }
  }
  else {
    digitalWrite(SIREN_TOP_IN_LED, LOW);
    digitalWrite(SIREN_TOP_OUT_LED, LOW);
  }
}

void swapTheSiren() { //this is the function that swap the blink between inside and outside LED
  if (sirenSwapStatus == 0) {
    if (currentMillis - previousCycle >= cycleLength)
    { sirenSwapStatus = 1;
      previousCycle += cycleLength;
    }
  }
  else {
    if (currentMillis - previousCycle >= cycleLength)
    { sirenSwapStatus = 0;
      previousCycle += cycleLength;
    }
  }
}

