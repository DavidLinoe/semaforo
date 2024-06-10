const int greenLedPins[] = {13, 15, 5};    // Pins for the green LEDs
const int yellowLedPins[] = {12, 2, 18};   // Pins for the yellow LEDs
const int redLedPins[] = {14, 4, 19};     // Pins for the red LEDs

long redInterval = 8000;    // Red light duration (in milliseconds)
long yellowInterval = 2000; // Yellow light duration (in milliseconds)
long greenInterval = 8000;  // Green light duration (in milliseconds)

unsigned long previousMillis = 0; // Store the last time the LEDs were updated
int phase = 0; // Current phase of the traffic light

void setup() {
  Serial.begin(9600);
  
  for (int i = 0; i < 3; i++) {
    pinMode(redLedPins[i], OUTPUT);
    pinMode(yellowLedPins[i], OUTPUT);
    pinMode(greenLedPins[i], OUTPUT);
  }
}

void loop() {
  unsigned long currentMillis = millis();
  long interval = 0;

  // Determine the interval based on the current phase
  switch (phase) {
    case 0:
    case 3:
      interval = greenInterval;
      break;
    case 1:
    case 4:
      interval = yellowInterval;
      break;
    case 2:
    case 5:
      interval = redInterval;
      break;
  }

  if (currentMillis - previousMillis >= interval) {
    // Update the previousMillis to the current time
    previousMillis = currentMillis;

    // Update the traffic light phase
    phase = (phase + 1) % 6;

    // Update the LED states based on the new phase
    updateTrafficLights();
  }

  // Example of dynamically adjusting the intervals
  // You can replace this part with actual conditions or inputs
  redInterval = 6000; // Example adjustment
  yellowInterval = 3000; // Example adjustment
  greenInterval = 8000; // Example adjustment
}

void updateTrafficLights() {
  switch (phase) {
    case 0:
      // Phase 1: Two directions green, one red
      for (int i = 0; i < 2; i++) {
        digitalWrite(redLedPins[i], LOW);
        digitalWrite(yellowLedPins[i], LOW);
        digitalWrite(greenLedPins[i], HIGH);
      }
      digitalWrite(redLedPins[2], HIGH);
      digitalWrite(yellowLedPins[2], LOW);
      digitalWrite(greenLedPins[2], LOW);
      break;
    case 1:
      // Phase 2: Transition to yellow for direction 1 and 2
      for (int i = 0; i < 2; i++) {
        digitalWrite(redLedPins[i], LOW);
        digitalWrite(yellowLedPins[i], HIGH);
        digitalWrite(greenLedPins[i], LOW);
      }
      break;
    case 2:
      // Phase 3: All red
      for (int i = 0; i < 3; i++) {
        digitalWrite(redLedPins[i], HIGH);
        digitalWrite(yellowLedPins[i], LOW);
        digitalWrite(greenLedPins[i], LOW);
      }
      break;
    case 3:
      // Phase 4: Direction 3 green, direction 1 and 2 red
      digitalWrite(redLedPins[0], HIGH);
      digitalWrite(yellowLedPins[0], LOW);
      digitalWrite(greenLedPins[0], LOW);

      digitalWrite(redLedPins[1], HIGH);
      digitalWrite(yellowLedPins[1], LOW);
      digitalWrite(greenLedPins[1], LOW);

      digitalWrite(redLedPins[2], LOW);
      digitalWrite(yellowLedPins[2], LOW);
      digitalWrite(greenLedPins[2], HIGH);
      break;
    case 4:
      // Phase 5: Transition to yellow for direction 3
      digitalWrite(redLedPins[2], LOW);
      digitalWrite(yellowLedPins[2], HIGH);
      digitalWrite(greenLedPins[2], LOW);
      break;
    case 5:
      // Phase 6: All red
      for (int i = 0; i < 3; i++) {
        digitalWrite(redLedPins[i], HIGH);
        digitalWrite(yellowLedPins[i], LOW);
        digitalWrite(greenLedPins[i], LOW);
      }
      break;
  }
}
