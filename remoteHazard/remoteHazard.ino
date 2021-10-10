//episode5: various blink pattern with multiple LEDs: car turn signal

// ----------LIBRARIES--------------
#include <IRremote.h>

// --------CONSTANTS (won't change)---------------
const int RECV_PIN = 8;
IRrecv irrecv(RECV_PIN);
decode_results results;
const int LEFT_LED = 13;
const int RIGHT_LED = 12;
const int toggleDelay = 100;

const int interval = 500; // number of millisecs between blinks
const int blinkDuration = 500; // number of millisecs that LED are ON

const int code1 = 0x10;  //code for turn left lamp
const int code2 = 0x810; //code for turn right lamp
const int code3 = 0x410; //code for hazard lamp (both)


//------------ VARIABLES (will change)---------------------

//arrays related to pattern
int patternStatus[] = {0, 0, 0}; //{left, right, hazard}
unsigned long previousToggle[] = {0, 0, 0}; // {left, right, hazard}

unsigned long currentMillis = 0;

//arrays related to LED
byte ledState[] = {LOW, LOW}; //one for each LED
unsigned long previousUpdate[] = {0, 0};  // will store last time the LED was updated

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
  // put your main code here, to run repeatedly:
  currentMillis = millis();

  if (irrecv.decode(&results)) {
    unsigned int receivedCode = results.value;

    switch (receivedCode) {
      case code1:
        toggleTurnLamp(0);//0 is the index for left lamp
        if (patternStatus[1] == 1) {
          toggleTurnLamp(1); //if the right-turn status is currently ON, turn it OFF
        }
        break;
      case code2:
        toggleTurnLamp(1); //1 is the index for right lamp
        if (patternStatus[0] == 1) {
          toggleTurnLamp(0); //if the left-turn status is currently ON, turn it OFF
        }
        break;
      case code3:
        toggleTurnLamp(2); //2 is the index for hazard lamp (both lamp)
        break;
    }

    Serial.println(receivedCode, HEX);
    irrecv.resume(); // Receive the next value

    Serial.print(patternStatus[0]); //0 is the index for left lamp
    Serial.print (" ");
    Serial.print(patternStatus[1]);
    Serial.print (" ");
    Serial.println(patternStatus[2]);

  }

  switchTheLeds (); //the actual function that switch the LED ON or OFF based on the value of their state variable 

}

// --------other functions---------------

void toggleTurnLamp (int ledIndex) {  //the function that turns a pattern ON or OFF
                                      //the ledIndex argument determines which pattern is being toggled
  if ((unsigned long)currentMillis - previousToggle[ledIndex] >= toggleDelay) {
    if (patternStatus[ledIndex] == 1) {       // if pattern is on then
      patternStatus[ledIndex] = 0;            // and set its state as off
    } else {                                  // else if pattern is off
      patternStatus[ledIndex] = 1;            // and set its state as on
    }
    previousToggle[ledIndex] = currentMillis;
  }
}

void blinkPattern(int ledPinNum, int ledIndex) { //this is the function that defines the pattern
  if (ledState[ledIndex] == LOW) {  // if the LED is OFF
    if ((unsigned long)currentMillis - previousUpdate[ledIndex] >= interval) {
      ledState[ledIndex] = HIGH;
      digitalWrite(ledPinNum, HIGH);
      // and save the time when we made the change
      previousUpdate[ledIndex] += interval;
      // NOTE: The previous line could alternatively be
      //        previousLedMillis = currentMillis
      //        Adding on the interval is a better way to ensure that succesive periods are identical
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
  if (patternStatus[0] == 1 || patternStatus[2] == 1) { //if turning left or hazard is on
    blinkPattern(LEFT_LED, 0);                          //then left lamp is blinking
  }
  else {
    digitalWrite(LEFT_LED, LOW);
  }

  if (patternStatus[1] == 1 || patternStatus[2] == 1) { //if turning right or hazard is on
    blinkPattern(RIGHT_LED, 1);                         //then right lamp is on blinking
  }
  else {
    digitalWrite(RIGHT_LED, LOW);
  }
}

