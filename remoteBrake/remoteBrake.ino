// ----------LIBRARIES--------------
#include <IRremote.h>
#include <IRremoteInt.h>

// --------CONSTANTS (won't change)---------------
const int RECV_PIN = 8;
IRrecv irrecv(RECV_PIN);
decode_results results;
const int ledBrake = 9;
const int toggleDelay = 100;
const int brakeBuffer = 50;

#define code1  16
#define codeUp  1168

//------------ VARIABLES (will change)---------------------

int brakeStatus= 0;
int lampStatus = 0;
int analogVal = 0;
unsigned long currentMillis = 0;
unsigned long previousToggle = 0;
unsigned long previousBrake = 0;

void setup() {
  // put your setup code here, to run once:
 
  Serial.begin(9600);

  irrecv.enableIRIn(); // Start the receiver
  // initialize the digital pin as an output.


}

void loop() {
 currentMillis = millis();
if (brakeStatus == 1) {
  analogVal = 255;  //if brake is pressed, lamp is on with full brightness
  }
  else if (lampStatus ==1 ) { analogVal = 100;  }
  else {analogVal = 0;}

analogWrite(ledBrake, analogVal);

  if (irrecv.decode(&results)) {
    unsigned int receivedCode = results.value;

Serial.print(brakeStatus);
Serial.print(" ");
Serial.print(lampStatus);
Serial.print(" ");
Serial.println(analogVal);

    switch (receivedCode) {
      case code1:
        toggleLed();
       break;
      case codeUp:
      brakeStatus = 1;
       previousBrake = currentMillis;
          break;
          
      }

    Serial.println(receivedCode, HEX);

    irrecv.resume(); // Receive the next value

  }

offBrake ();


}

void toggleLed () {
  if (currentMillis - previousToggle >= toggleDelay) {

    if (lampStatus == 1) {       // if first led is on then

      lampStatus = 0;           // and set its state as off

    } else {                      // else if first led is off

      lampStatus= 1;          // and set its state as on

    }
    previousToggle = currentMillis;
  }
}

void offBrake () {
 
  if (currentMillis - previousBrake >= brakeBuffer) {

      brakeStatus = 0;           // and set its state as off
  }
 
}


