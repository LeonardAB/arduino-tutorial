//episode3: decoding various IR protocol

// ----------LIBRARIES--------------
#include <IRremote.h>

// --------CONSTANTS (won't change)---------------
const int RECV_PIN = 8;
IRrecv irrecv(RECV_PIN);
decode_results results;

//========== THE SETUP ==============================

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

//========== THE LOOP ==============================

void loop() {
  // put your main code here, to run repeatedly:

  if (irrecv.decode(&results)) { //if a signal is received
    unsigned int receivedCode = results.value;
    Serial.println(receivedCode, HEX);
    irrecv.resume(); // receive the next value
  }

}





