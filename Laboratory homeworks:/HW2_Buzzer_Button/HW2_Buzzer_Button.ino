const int knockPin = A0;
const int buzzerPin = 8;
const int buttonPin = 2;
bool activeBuzzer = false;
long int lastKnock = 0;
int knockTreshold = 100;
bool knockCheck = false;
const long int knockWait = 5000;
const int buzzerInterval = 200;
long int lastBuzzerState = 0;
bool buzzerState = false;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // when the button is pressed
  Serial.begin(9600);
}

void loop() {
  if(activeBuzzer) {
    int buttonState = !digitalRead(buttonPin);
    if(buttonState == true) {
      Serial.println("buzzer has stopped");
      digitalWrite(buzzerPin, LOW);
      activeBuzzer = false;
      knockCheck = false;
      buzzerState = false;
    }
    else {
      if (millis() - lastBuzzerState >= buzzerInterval) {
        buzzerState = !buzzerState;
        digitalWrite(buzzerPin, buzzerState);
        lastBuzzerState = millis();
      }
    }
  }
  else if(knockCheck) {
    if(millis() - lastKnock >= knockWait) {
      Serial.println("buzzer has started at " + String(millis()));
      digitalWrite(buzzerPin, HIGH);
      lastBuzzerState = millis();
      activeBuzzer = true;
      buzzerState = true;
    }
  }
  else {
    int knockLevel = Serial.parseInt();
    if(knockLevel >= knockTreshold) {
      knockCheck = true;
      lastKnock = millis();
      Serial.println(9600);
    }
  }
}
