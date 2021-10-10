//episode3: decoding various IR protocol: NEC

// ----------LIBRARIES--------------
#include <IRremote.h>

// --------CONSTANTS (won't change)---------------
const int RECV_PIN = 8;
IRrecv irrecv(RECV_PIN);
decode_results results;
const int CONT_LED = 13; //output for continuous LED
const int DISC_LED = 12; //output for non-continuous LED
const int code1 = 0x40BF; //code for continuous button
const int code2 = 0x8F7; //code for non-continuous button

//------------ VARIABLES (will change)---------------------

int repeatedCode = 0x0; //to save the value of the button other than 0xFFFF
int effectiveCode = 0x0; //code that is executed to the switch-case

//========== THE SETUP ==============================

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  irrecv.enableIRIn(); // Start the receiver
  pinMode(CONT_LED, OUTPUT); //output for continuous LED
  pinMode(DISC_LED, OUTPUT);//output for non-continuous LED
}

//========== THE LOOP ==============================

void loop() {
  // put your main code here, to run repeatedly:

  if (irrecv.decode(&results)) {
    unsigned int receivedCode = results.value;

    if (receivedCode != 0xFFFF) {
      effectiveCode = receivedCode; //use the sent code as is
      repeatedCode = receivedCode; //save this value so when FFFF appears, it will be replaced by this value
    }
    else {
      effectiveCode = repeatedCode; // if it equals FFFF, replace it with the last saved value
    }

    switch (effectiveCode) {
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





