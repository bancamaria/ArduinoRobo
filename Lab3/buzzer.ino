const int photoCellPin = A0;
int buzzerPin = 11;
int photoCellValue = 0;
int buzzerValue = 0;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  photoCellValue =analogRead(photoCellPin);
  tone(buzzerPin, buzzerValue);
  delay(100);
  noTone(buzzerPin);
  delay(100);
  buzzerValue += 100;
  Serial.println(photoCellValue);
}
