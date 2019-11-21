// joystick
const int xPin = A0;
const int yPin = A1;
const int pushPin = A2;
int xValue, yValue;
int xValue_minTreshold = 400;
int yValue_minTreshold = 400;
int xValue_maxTreshold = 600;
int yValue_maxTreshold = 600;
bool pushValue;
bool joystick_moved = false;
int lastDebounce = 0;
bool lastPushValue;
int pushDebounceInterval = 10;

// 4 digit 7 segment display
const int pinA = 12;
const int pinB = 8;
const int pinC = 5;
const int pinD = 3;
const int pinE = 2;
const int pinF = 11;
const int pinG = 6;
const int pinDP = 4;
const int pinD1 = 7;
const int pinD2 = 9;
const int pinD3 = 10;
const int pinD4 = 13;
const int segSize = 8;
const int noOfDisplays = 4;
const int noOfDigits = 10;
bool dpState;

int segments[segSize] = {pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP};
int digits[noOfDisplays] = {pinD1, pinD2, pinD3, pinD4};
int currentDigitsValues[noOfDisplays] = {0, 0, 0, 0};

int currentDigit = 0;
int flushInterval = 1;
int lastFlush = 0;
int selectedDigit = noOfDisplays;
int blinkingDigit = noOfDisplays - 1;
int blinkState = true;
int blinkInterval = 499 + flushInterval;
int lastBlink = 0;

byte digitMatrix[noOfDigits][segSize - 1] = {
//   a  b  c  d  e  f  g
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

void displayDigit(int num, byte digit, byte decimalPoint) {
  showDigit(num);
  for (int i = 0; i < segSize - 1; i++) {
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }
  digitalWrite(segments[segSize - 1], decimalPoint);
}

void showDigit(int num) {
  for (int i = 0; i < noOfDisplays; i++) {
    digitalWrite(digits[i], HIGH);
  }
  digitalWrite(digits[num], LOW);
}

void clearDigit(int num) {
  showDigit(num);
  for (int i = 0; i < segSize; ++i) {
    digitalWrite(segments[i], LOW);
  }
}

void setup() {
  pinMode(pushPin, INPUT_PULLUP);
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
  for (int i = 0; i < noOfDisplays; i++) {
    pinMode(digits[i], OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
  // if no digit is selected, blink
  if (selectedDigit == noOfDisplays) { 
    xValue = analogRead(xPin);
    if (xValue < xValue_minTreshold && joystick_moved == false) {       // move digit to the left and make it blink
      if (blinkingDigit > 0)
        blinkingDigit--;
      else
        blinkingDigit = noOfDisplays - 1;
      blinkState = true;
      lastBlink = millis();
      joystick_moved = true;
    }
    if (xValue > xValue_maxTreshold&& joystick_moved == false) {       // move digit to the right and make it blink
      if (blinkingDigit < noOfDisplays - 1)
        blinkingDigit++;
      else
        blinkingDigit = 0;
      blinkState = true;
      lastBlink = millis();
      joystick_moved = true;
    }
    if (xValue >= xValue_minTreshold && xValue <= xValue_maxTreshold) // joystick is in neutral position
      joystick_moved = false;
    if (millis() - lastBlink >= blinkInterval) {                      // update blink info based on time elapsed since last update
      lastBlink = millis();
      blinkState = !blinkState;
      //Serial.println("Blink at " + String(millis()));
    }
    if (millis() - lastFlush >= flushInterval) {                      // use the old digit before trying to update it
      if (currentDigit == blinkingDigit && blinkState == false)
        clearDigit(currentDigit);
      else
        displayDigit(currentDigit, currentDigitsValues[currentDigit], false);
      lastFlush = millis();
      if (currentDigit == noOfDisplays - 1)
        currentDigit = 0;
      else
        currentDigit++;
    }
  }
   // a digit was selected
  else {                                                           
    yValue = analogRead(yPin);
    if (yValue < yValue_minTreshold && joystick_moved == false) {     // decrease value
      if (currentDigitsValues[selectedDigit] > 0)
        currentDigitsValues[selectedDigit]--;
      else
        currentDigitsValues[selectedDigit] = noOfDigits - 1;
      joystick_moved = true;
    }
    if (yValue > yValue_maxTreshold&& joystick_moved == false) {     // incresease value
      if (currentDigitsValues[selectedDigit] < noOfDigits - 1)
        currentDigitsValues[selectedDigit]++;
      else
        currentDigitsValues[selectedDigit] = 0;
      joystick_moved = true;
    }
    if (yValue >= yValue_minTreshold && yValue <= yValue_maxTreshold) // joystick is in neutral position
      joystick_moved = false;
    if (millis() - lastFlush >= flushInterval) {                      // use the old digit before trying to update i
      if (currentDigit == selectedDigit)
        displayDigit(currentDigit, currentDigitsValues[currentDigit], true);
      else
        displayDigit(currentDigit, currentDigitsValues[currentDigit], false);
      lastFlush = millis();
      if (currentDigit == noOfDisplays - 1)
        currentDigit = 0;
      else
        currentDigit++;
    }
  }
  if(millis() - lastDebounce >= pushDebounceInterval) {
    lastDebounce = millis();
    pushValue = !digitalRead(pushPin);
    if (pushValue != lastPushValue) {                             // if the joystick was pushed
      if (pushValue == true) {
        Serial.println(pushValue);
        if (selectedDigit == noOfDisplays) {                      // if there is no digit selected it remains with the old one (the blinking one)
          selectedDigit = blinkingDigit;
          blinkingDigit = noOfDisplays;
        }
        else {                                                    // else update new digit value
          blinkingDigit = selectedDigit;
          selectedDigit = noOfDisplays;
          blinkState = true;
          lastBlink = millis();
        }
      }
    lastPushValue = pushValue;
    }
  }
}
