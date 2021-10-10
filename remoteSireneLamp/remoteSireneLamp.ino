// ----------LIBRARIES--------------
#include <IRremote.h>
#include <IRremoteInt.h>

// --------CONSTANTS (won't change)---------------
const int RECV_PIN = 8;
IRrecv irrecv(RECV_PIN);
decode_results results;
const int led = 13;
const int toggleDelay = 100;
const int intervalMinor = 70; // number of millisecs between blinks
const int intervalMajor = 300;
const int blinkDuration = 80; // number of millisecs that Led's are on - all three leds use this

#define code1  16


//------------ VARIABLES (will change)---------------------

int ledStatus = 0;
unsigned long currentMillis = 0;
unsigned long previousToggle = 0;
boolean majorInterval = true;
byte targetLedState = LOW;
unsigned long previousOnBoardLedMillis = 0;   // will store last time the LED was updated
 int flashCount =0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  irrecv.enableIRIn(); // Start the receiver
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
}

void loop() {
  currentMillis = millis();
  Serial.println(ledStatus);
  // put your main code here, to run repeatedly:
  if (ledStatus == 1) {
  sireneLed(led);
}
else {
   digitalWrite(led, LOW); 
}
  if (irrecv.decode(&results)) {
    unsigned int receivedCode = results.value;

    switch (receivedCode) {
      case code1:
        toggleLed(led);
        break;

    }
    
    Serial.println(receivedCode, HEX);

    irrecv.resume(); // Receive the next value



  }
}

void toggleLed (int ledNum) {
  if (currentMillis - previousToggle >= toggleDelay) {
    Serial.println(previousToggle);
    if (ledStatus == 1) {       // if first led is on then
   
      ledStatus = 0;           // and set its state as off
 
    } else {                      // else if first led is off
     
      ledStatus = 1;          // and set its state as on

    }
    previousToggle = currentMillis;
  }
}



void sireneLed(int ledNum) {
 int onBoardLedInterval ;

   if (targetLedState == LOW) {
   
    onBoardLedInterval = (flashCount==0) ? intervalMajor : intervalMinor;
    // if the Led is off, we must wait for the interval to expire before turning it on
    if (currentMillis - previousOnBoardLedMillis >= onBoardLedInterval) {
      // time is up, so change the state to HIGH
      targetLedState = HIGH;
      digitalWrite(ledNum, HIGH); 
      // and save the time when we made the change
      previousOnBoardLedMillis += onBoardLedInterval;
      flashCount += 1; 
      flashCount = (flashCount>2) ? 0 : flashCount;
      // NOTE: The previous line could alternatively be
      //              previousOnBoardLedMillis = currentMillis
      //        which is the style used in the BlinkWithoutDelay example sketch
      //        Adding on the interval is a better way to ensure that succesive periods are identical

    }
   }
  else {  // i.e. if onBoardLedState is HIGH

    // if the Led is on, we must wait for the duration to expire before turning it off
    if (currentMillis - previousOnBoardLedMillis >= blinkDuration) {
      // time is up, so change the state to LOW
      targetLedState = LOW;
       digitalWrite(ledNum, LOW); 
      // and save the time when we made the change
      previousOnBoardLedMillis += blinkDuration;
    }
  }
}

