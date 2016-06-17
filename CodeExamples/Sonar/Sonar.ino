// Sonar.ino
// Author: Sébastien Combéfis
// Version: June 17, 2016

#define TRIG_PIN 13
#define ECHO_PIN 12

void setup() {
  Serial.begin(9600);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

// Send a 10µs pulse to the trigger input
void sendPulse() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
}

void loop() {
  char output[512];
  unsigned long duration, distance;
  
  // Trigger a measure
  sendPulse();

  // Measure the duration of the pulse on the echo output
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration / 2) / 29.1;
  
  // Print the estimated distance
  if (0 <= distance && distance <= 200) {
    sprintf(output, "%lu cm\n", distance);
    Serial.write(output);
  } else {
    Serial.println("Out of range.");
  }
  delay(500);
}

