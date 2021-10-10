//episode1: replacing delay() with millis ()

// --------CONSTANTS (won't change)---------------

const int targetPin =  13;
const int blinkDuration = 500; // number of millisecs that LED is on
const int interval = 300; // number of milisecs when the LED is off


//------------ VARIABLES (will change)---------------------

byte targetLedState = HIGH; // this variable is necesary to help arduino monitor the status of LED
                            //the initial condition (t=0) can be either HIGH or LOW
unsigned long currentMillis = 0;    // stores the value of millis() in each iteration of loop()
unsigned long previousLedMillis = 0;   // will store last time the LED was updated


//========== THE SETUP ==============================

void setup() {
// put your setup code here, to run once:
  
  Serial.begin(9600); //default comm speed between arduino and serial monitor. no need to change.
  Serial.println("blinkMillis.ino");  // so we know what sketch is running
  pinMode(targetPin, OUTPUT);
}

//========== THE LOOP ==============================

void loop() {
  // put your main code here, to run repeatedly:
  
  currentMillis = millis();   // capture the latest value of millis()
                              // this is equivalent to noting the time from a clock
  updateTargetLED();  //monitoring status of LED and do approproate actions

}

//------------ OTHER FUNCTIONS ---------------------

void updateTargetLED() {

  if (targetLedState == LOW) { //if the LED is OFF
     Serial.println("LOW");
    if ((unsigned long) currentMillis - previousLedMillis >= interval) { 
      targetLedState = HIGH;
      digitalWrite(targetPin, targetLedState);
      // and save the time when we made the change
      previousLedMillis += interval;
      // NOTE: The previous line could alternatively be
      //        previousLedMillis = currentMillis
      //        Adding on the interval is a better way to ensure that succesive periods are identical
    }
  }
  else {  // i.e. if onBoardLedState is HIGH (if the LED is ON)
    Serial.println("HIGH");
    if ((unsigned long) currentMillis - previousLedMillis >= blinkDuration) {
      targetLedState = LOW;
      digitalWrite(targetPin, targetLedState);
      // and save the time when we made the change
      previousLedMillis += blinkDuration;
      // NOTE: The previous line could alternatively be
      //        previousLedMillis = currentMillis
      //        Adding on the duration is a better way to ensure that succesive periods are identical
    }
  }
}


