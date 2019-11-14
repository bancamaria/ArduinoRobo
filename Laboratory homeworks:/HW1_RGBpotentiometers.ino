const int redPotPin = A2;
const int greenPotPin = A1;
const int bluePotPin = A0;
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

void setup() {
  Serial.begin(9600);
  pinMode(redPotPin, INPUT);
  pinMode(greenPotPin, INPUT);
  pinMode(bluePotPin, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  analogWrite(redPin, map(analogRead(redPotPin), 0, 1023, 0, 255));
  analogWrite(greenPin, map(analogRead(greenPotPin), 0, 1023, 0, 255));
  analogWrite(bluePin, map(analogRead(bluePotPin), 0, 1023, 0, 255));
}
