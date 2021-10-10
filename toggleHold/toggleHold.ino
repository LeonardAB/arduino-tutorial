//episode4: controlling an LED with IR remote

// ----------LIBRARIES--------------
#include <IRremote.h>

// --------CONSTANTS (won't change)---------------
const int RECV_PIN = 8;
IRrecv irrecv(RECV_PIN);
decode_results results;
const int HOLD_LED = 13;
const int TOGGLE_LED = 12;
const int SIREN_LED = 11;

//constants for the siren
const int intervalMinor = 70;  // number of millisecs that siren LED is off between flashes
const int intervalMajor = 300; // number of millisecs that siren LED is off between cycles
const int blinkDuration = 80;  // number of millisecs that siren LED is on
const int flashesInCycle = 3;  // number of flashes inside a cycle

const int toggleDelay = 100; //one press will result in 3 burst of signal,
                             //so after the first signal is received,
                             //arduino need to ignore the other signals for a few milliseconds
const int holdBuffer = 50; //signal is received as repeated burst, resulting in flickering light.
                            //arduino need to wait a few milliseconds before it turns off the LED
const int code1 = 0x490; //code for continuous button
const int code2 = 0x10;  //code for non-continuous button
const int code3 = 0x810; //code for non-continuous button


//------------ VARIABLES (will change)---------------------

int holdStatus = 0;
int toggleStatus = 0;
int sirenStatus = 0;
byte sirenLedStatus = LOW;
int flashCount = 0;
unsigned long currentMillis = 0;
unsigned long previousToggle = 0; // this is used both for normal toggling and siren toggling
unsigned long previousHold = 0;
unsigned long previousLedMillis = 0; //for controlling the siren

//========== THE SETUP ==============================

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver

  pinMode(TOGGLE_LED, OUTPUT);
  pinMode(HOLD_LED, OUTPUT);
  pinMode(SIREN_LED, OUTPUT);

}

//========== THE LOOP ==============================

void loop() {
  // put your main code here, to run repeatedly:
  currentMillis = millis();

  if (irrecv.decode(&results)) {
    unsigned int receivedCode = results.value;

    switch (receivedCode) {
      case code1:
        holdStatus = 1; //this button always turning ON, so no need to put as a function
        previousHold = currentMillis;
        break;
      case code2:
        toggleTheLed();  // this button can turn ON or OFF depending on the current status of the lamp
        break;
      case code3:
        toggleTheSiren();  // this button can turn ON or OFF depending on the current status of the siren
        break;
      default:
        break;
    }

    Serial.println(receivedCode, HEX);
    irrecv.resume(); // Receive the next value

  }

  holdTheLed (); // function that makes the LED stays ON when the button is pressed (and removing the flicker)
  switchTheLeds (); //the actual function that switch the LED ON or OFF based on the value of their state variable

}

// --------other functions---------------

void toggleTheLed () {
  if ((unsigned long)currentMillis - previousToggle >= toggleDelay) {
    if (toggleStatus == 1) {       // if LED is on then
      toggleStatus = 0;            // set its state as off
    } else {                       // else if LED is off
      toggleStatus = 1;            // set its state as on
    }
    previousToggle = currentMillis;
  }
}

void holdTheLed () {
  if ((unsigned long)currentMillis - previousHold >= holdBuffer) {
    holdStatus = 0;           // set its state as OFF only after the buffer time has passed
  }
}

void toggleTheSiren () {
  if ((unsigned long)currentMillis - previousToggle >= toggleDelay) {
    if (sirenStatus == 1) {       // if siren is on then
      sirenStatus = 0;            // set its state as off
    } else {                       // else if siren is off
      sirenStatus = 1;            // set its state as on
    }
    previousToggle = currentMillis;
  }
}

void switchTheLeds () {
  if (holdStatus == 1) {
    digitalWrite(HOLD_LED, HIGH);
  }
  else {
    digitalWrite(HOLD_LED, LOW);
  }

  if (toggleStatus == 1)
  {
    digitalWrite(TOGGLE_LED, HIGH);
  }
  else {
    digitalWrite(TOGGLE_LED, LOW);
  }
  if (sirenStatus == 1)
  {
    sirenPattern();
  }
  else {
    digitalWrite(SIREN_LED, LOW);
  }

}

void sirenPattern() {
  int offInterval ;
  if (sirenLedStatus == LOW) { //if the LED is OFF
    offInterval = (flashCount < flashesInCycle) ? intervalMinor : intervalMajor;  //choose interval value
    if ((unsigned long)currentMillis - previousLedMillis >= offInterval) {
      sirenLedStatus = HIGH;
      digitalWrite(SIREN_LED, sirenLedStatus);
      previousLedMillis += offInterval;
      flashCount = (flashCount < flashesInCycle) ? (flashCount += 1) : 0; //update the counter
    }
  }
  else {  // i.e. if onBoardLedState is HIGH (if the LED is ON)
    if ((unsigned long)currentMillis - previousLedMillis >= blinkDuration) {
      sirenLedStatus = LOW;
      digitalWrite(SIREN_LED, sirenLedStatus);
      previousLedMillis += blinkDuration;
    }
  }
}

