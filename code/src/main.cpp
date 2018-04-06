#include "Arduino.h"

#define GNDNUM 4
#define PWRNUM 5

struct PinPair {
  int gndPin;
  int pwrPin;

  PinPair(int g, int pw): gndPin(g), pwrPin(pw) {};
  PinPair() {};
};

PinPair mapper[20] = {
  PinPair(15,4),
  PinPair(15,17),
  PinPair(15,5),
  PinPair(15,18),
  PinPair(2,4),
  PinPair(15,16),
  PinPair(2,18),
  PinPair(2,17),
  PinPair(2,5),
  PinPair(2,16),

  PinPair(19,4),
  PinPair(19,16),
  PinPair(19,5),
  PinPair(19,18),
  PinPair(21,4),
  PinPair(19,17),
  PinPair(21,18),
  PinPair(21,16),
  PinPair(21,5),
  PinPair(21,17),
};

void TurnOn(int led);

int gndSet[] = {15, 2, 19, 21};
int pwrSet[] = {4, 16, 17, 5, 18};

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < GNDNUM; i++) {
    pinMode(gndSet[i], OUTPUT);
  }
  for (int i = 0; i < PWRNUM; i++) {
    pinMode(pwrSet[i], OUTPUT);
  }
}

void TurnOn(int led){
  for (size_t i = 0; i < GNDNUM; i++) {
    digitalWrite(gndSet[i], 1);
  }
  for (size_t i = 0; i < PWRNUM; i++) {
    digitalWrite(pwrSet[i], 0);
  }
  PinPair pair = mapper[led];
  digitalWrite(pair.pwrPin, 1);
  digitalWrite(pair.gndPin, 0);
}
int i = 0;
void loop() {
 Serial.print(mapper[i%20].gndPin);
 Serial.print(": ");
 Serial.println(mapper[i%20].pwrPin);
 TurnOn(i%20);
 delay(1000);
 i++;
}
