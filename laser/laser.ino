#include <dummy.h>

const int PORT = 13;
int level = HIGH;


void setup() {
  // put your setup code here, to run once:
  pinMode(PORT, OUTPUT);
  digitalWrite(PORT, HIGH);
}

void loop() {
}
