//episode2: LED siren blink

// --------CONSTANTS (won't change)---------------

const int targetPin =  13;
const int intervalMinor = 70;  // number of millisecs that LED is off between flashes
const int intervalMajor = 300; // number of millisecs that LED is off between cycles
const int blinkDuration = 80;  // number of millisecs that LED is on
const int flashesInCycle = 3;  // number of flashes inside a cycle


//------------ VARIABLES (will change)---------------------

byte targetLedState = HIGH; // this variable is necesary to help arduino monitor the status of LED
//the initial condition (t=0) can be either HIGH or LOW
unsigned long currentMillis = 0;    // stores the value of millis() in each iteration of loop()
unsigned long previousLedMillis = 0;   // will store last time the LED was updated
int flashCount = 0;

//========== THE SETUP ==============================

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600); //default comm speed between arduino and serial monitor. no need to change.

  Serial.println("sirenBlink.ino");  // so we know what sketch is running
  pinMode(targetPin, OUTPUT);
}

//========== THE LOOP ==============================

void loop() {
  // put your main code here, to run repeatedly:

  currentMillis = millis();   // capture the latest value of millis()
  //   this is equivalent to noting the time from a clock

  updateTargetLED(); //monitoring status of LED and do approproate actions

}

//------------ OTHER FUNCTIONS ---------------------

void updateTargetLED() {
  int offInterval ;

  if (targetLedState == LOW) { //if the LED is OFF
    Serial.println("LOW");
    offInterval = (flashCount < flashesInCycle) ? intervalMinor : intervalMajor;  //choose interval value
    if ((unsigned long)currentMillis - previousLedMillis >= offInterval) {
      targetLedState = HIGH;
      digitalWrite(targetPin, targetLedState);
      previousLedMillis += offInterval;
      flashCount = (flashCount < flashesInCycle) ? (flashCount += 1) : 0; //update the counter
    }
  }
  else {  // i.e. if onBoardLedState is HIGH (if the LED is ON)
    Serial.println("HIGH");
    if ((unsigned long)currentMillis - previousLedMillis >= blinkDuration) {
      targetLedState = LOW;
      digitalWrite(targetPin, targetLedState);
      previousLedMillis += blinkDuration;
    }
  }
}


