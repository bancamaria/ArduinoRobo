const int buttonPin = 2;
const int ledPin = 13;
int ledState = 0;
const int photoCellPin = A0;
int photoCellValue = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  photoCellValue =analogRead(photoCellPin);
  if(photoCellValue < 20){
    digitalWrite(ledPin, HIGH);
  }
  if(photoCellValue > 160) {
    digitalWrite(ledPin, LOW);
  }
  Serial.println(photoCellValue);
}
