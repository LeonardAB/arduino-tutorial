//episode5: various blink pattern with multiple LEDs: siren with two lamps

// ----------LIBRARIES--------------
#include <IRremote.h>

// --------CONSTANTS (won't change)---------------
const int RECV_PIN = 8;
IRrecv irrecv(RECV_PIN);
decode_results results;
const int LEFT_LED = 13;
const int RIGHT_LED = 12;
const int toggleDelay = 100; //to avoid toggling flicker
const int interval = 80; // number of millisecs between blinks
const int blinkDuration = 80; // number of millisecs that LED are ON
const int cycleLength = 2 * interval + 2 * blinkDuration;

const int code1 = 0x10;

//------------ VARIABLES (will change)---------------------

int sirenStatus = 0;
unsigned long currentMillis = 0;
unsigned long previousToggle = 0; //to avoid toggling flicker
byte ledState[] = {LOW, LOW}; //one for each LED
unsigned long previousUpdate[] = {0, 0};  // will store last time the LED was updated
unsigned long previousCycle = 0;
int sirenSwapStatus = 0; //determines which LED is blinking. 0 for left, 1 for right

//========== THE SETUP ==============================

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  irrecv.enableIRIn(); // start the receiver

  pinMode(LEFT_LED, OUTPUT);
  pinMode(RIGHT_LED, OUTPUT);
}

//========== THE LOOP ==============================

void loop() {
  currentMillis = millis();

  if (irrecv.decode(&results)) {
    unsigned int receivedCode = results.value;

    switch (receivedCode) {
      case code1:
        toggleSiren();
        break;
    }

    Serial.println(receivedCode, HEX);
    irrecv.resume(); // Receive the next value

    Serial.println(sirenSwapStatus);

  }

  switchTheLeds () ; //the actual function that switch the LED ON or OFF based on the value of their state variable

}

// --------other functions---------------

void toggleSiren () { //the function that turns a pattern ON or OFF
  if (currentMillis - previousToggle >= toggleDelay) {
    if (sirenStatus == 1) {      // if siren is on then
      sirenStatus = 0;           // and set its state as off
    } else {                      // else if siren is off
      sirenStatus = 1;          // and set its state as on
    }
    previousToggle = currentMillis;
  }
}

void sirenPattern(int ledPinNum, int ledIndex) { //this is the function that defines the pattern
  if (ledState[ledIndex] == LOW) {// if the LED is OFF
    if ((unsigned long)currentMillis - previousUpdate[ledIndex] >= interval) {
      ledState[ledIndex] = HIGH;
      digitalWrite(ledPinNum, HIGH);
      // and save the time when we made the change
      previousUpdate[ledIndex] += interval;
        }
  }
  else {  // i.e. if the LED is ON
    if (currentMillis - previousUpdate[ledIndex] >= blinkDuration) {
      ledState[ledIndex] = LOW;
      digitalWrite(ledPinNum, LOW);
      // and save the time when we made the change
      previousUpdate[ledIndex] += blinkDuration;
    }
  }
}

void switchTheLeds () {
  if (sirenStatus == 1) { //if it is toggled to on
    swapTheBlink(); //this is the function that swap the blink between left and right LED
    if (sirenSwapStatus == 0) { 
      sirenPattern(LEFT_LED, 0);
      digitalWrite(RIGHT_LED, LOW);
    }
    else {
      sirenPattern(RIGHT_LED, 1);
      digitalWrite(LEFT_LED, LOW);
    }
  }
  else {
    digitalWrite(LEFT_LED, LOW);
    digitalWrite(RIGHT_LED, LOW);
  }
}

void swapTheBlink() { //this is the function that swap the blink between left and right LED
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

