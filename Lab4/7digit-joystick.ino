const int pinSW = 10;
const int pinX = A0;
const int pinY = A1;
int Xvalue, Yvalue, swValue;
bool joyMoved = false;
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
int digit = 0;
byte state = 0;
byte stateDP = 0;
int i;
const int minTreshold = 400, maxTreshold = 600;

const int noOfDigits = 10;
const byte segments[segSize] = {pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP};
const byte digitMatrix[noOfDigits][segSize - 1] = {
// a  b  c  d  e  f  g
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

void displayFunction(int digit) {
  for(i = 0; i < segSize - 1; i++) {
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }
  digitalWrite(segments[segSize - 1], stateDP);
}

void setup() {
   for(i = 0; i < segSize; i++) {
     pinMode(segments[i], OUTPUT);
  }
  pinMode(pinSW, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  //analogRead(pinX);
  Xvalue = analogRead(pinX);
  if(Xvalue < minTreshold && joyMoved == false) {
    joyMoved = true;
    digit--;
    if(digit < 0) {  // first way
      digit = 9;
    }
    /*if(digit == 0) { // second way
      digit = 9;
    }
    else {
      digit--;
    }*/
  }
  if(Xvalue > maxTreshold && joyMoved == false) {
    joyMoved = true;
    digit++;
    if(digit > 9) {  // first way
      digit = 0;
    }
    /*if(digit == 9) { // second way
      digit = 0;
    }
    else {
      digit++;
    }*/
  }

   if(Xvalue < maxTreshold && Xvalue > minTreshold) {
    joyMoved = false;
   }
  
  displayFunction(digit);
  delay(1);
}
