//episode3: decoding various IR protocol: PHILLIPS

// ----------LIBRARIES--------------
#include <IRremote.h>

// --------CONSTANTS (won't change)---------------
const int RECV_PIN = 8;
IRrecv irrecv(RECV_PIN);
decode_results results;
const int CONT_LED = 13; //output for continuous LED
const int DISC_LED = 12; //output for non-continuous LED
const int code1a = 0x10; //first code for continuous button
const int code1b = 0x810; //second code for continuous button
const int code2a = 0x1; //first code for non-continuous button
const int code2b = 0x801; //second code for non-continuous button

//========== THE SETUP ==============================

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
 irrecv.enableIRIn(); // Start the receiver
  pinMode(CONT_LED, OUTPUT); //output for continuous LED
   pinMode(DISC_LED, OUTPUT); //output for non-continuous LED
}

//========== THE LOOP ==============================

void loop() {
  // put your main code here, to run repeatedly:
  
  if (irrecv.decode(&results)) {
    unsigned int receivedCode = results.value;

    switch (receivedCode) {
      case code1a:
        digitalWrite(CONT_LED, HIGH);
        Serial.println(receivedCode, HEX);
        break;
      case code1b:
        digitalWrite(CONT_LED, HIGH);
        Serial.println(receivedCode, HEX);
        break;
      case code2a:
        digitalWrite(DISC_LED, HIGH);
        Serial.println(receivedCode, HEX);
        break;
      case code2b:
        digitalWrite(DISC_LED, HIGH);
        Serial.println(receivedCode, HEX);
        break;  
      default:
        Serial.print("other button: ");
        Serial.println(receivedCode, HEX);
        break;
    }
   
    irrecv.resume(); // Receive the next value

  }
  digitalWrite(CONT_LED, LOW); //LED is off when the button is not pressed
  digitalWrite(DISC_LED, LOW);
}





