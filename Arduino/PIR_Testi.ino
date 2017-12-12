/*
 * PIR sensor tester
 */
 
int ledPin = 13;                // choose the pin for the LED
int inputPin = 2;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
int wakePin = 4;
int calibrationTime = 20;
 
void setup() {
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
  pinMode(wakePin, OUTPUT);
 
  Serial.begin(9600);

  Serial.print("Calibrating sensor");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      if(i == calibrationTime - 1){
        Serial.println(" ");
      }
      delay(1000);
      }
  Serial.println("Calibration done");
}
 
void loop(){
  val = digitalRead(inputPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      pirState = HIGH;
      digitalWrite(wakePin, LOW);
      Serial.println("wakePin LOW");
      delay(50);
      digitalWrite(wakePin, HIGH);
      Serial.println("wakePin HIGH");
      
      //Give Huzzah 40 seconds to do its thing
      Serial.println("Staying in HIGH mode for 40 seconds");
      for(int i = 0; i < 40; i++){
        val = HIGH;
        delay(1000);
      }
      Serial.println("HIGH mode off");
    }
  } else {
    digitalWrite(ledPin, LOW); // turn LED OFF
    if (pirState == HIGH){
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }
}
