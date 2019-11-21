const int pinA = 2;
const int pinB = 3;
const int pinC = 4;
const int pinD = 5;
const int pinE = 6;
const int pinF = 7;
const int pinG = 8;
const int pinDP = 9;
#define segSize 8
int index = 0;
byte state = 0;
int i;
const byte segments[segSize] = {pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP};

void setup() {
  for( i = 0; i < segSize; i++) {
     pinMode(segments[i], OUTPUT);
  }
}

void loop() {
  digitalWrite(segments[index], state);
  index++;
  delay(200);
  if(index == segSize) {
    state = !state;
    index = 0;
  }
}
