//episode3: decoding various IR protocol: SONY

// ----------LIBRARIES--------------
#include <IRremote.h>

// --------CONSTANTS (won't change)---------------
const int RECV_PIN = 8;
IRrecv irrecv(RECV_PIN);
decode_results results;
const int CONT_LED = 13; //output for continuous LED
const int DISC_LED = 12; //output for non-continuous LED
const int code1 = 0x490; //code for continuous button
const int code2 = 0x10; //code for non-continuous button

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
      case code1:
        digitalWrite(CONT_LED, HIGH);
        Serial.println(receivedCode, HEX);
        break;
      case code2:
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





