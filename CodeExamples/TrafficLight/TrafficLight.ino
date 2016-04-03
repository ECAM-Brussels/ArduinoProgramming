// TrafficLight.ino
// Author: Sébastien Combéfis
// Version: April 3, 12016

const int configs[][4] = {
  {LOW, LOW, HIGH, 2000}, // red
  {LOW, HIGH, LOW, 500},  // orange
  {HIGH, LOW, LOW, 2000}  // green
};
const int pins[] = {13, 12, 8};
int state = 0;

void setup() {
  for (int i = 0; i < 3; i++) {
    pinMode(pins[i], OUTPUT);
  }
  digitalWrite(pins[0], HIGH);
  delay(configs[0][3]);
}

void loop() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(pins[i], configs[state][i]);
  }
  delay(configs[state][3]);
  state = (state + 1) % 3;
}
