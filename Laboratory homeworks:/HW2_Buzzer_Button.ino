const int knockPin = A0;
const int buzzerPin = 8;
const int buttonPin = 2;

bool buzzerActive = false;
long int lastKnock = 0;

int knockTreshold = 100;
bool knockHappened = false;
const long int knockWait = 5000;

const int buzzerInterval = 100;
long int lastBuzzerEvent = 0;
bool buzzerState = false;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  if(buzzerActive) {
    int buttonState = !digitalRead(buttonPin);
    if(buttonState == true) {
      Serial.println("Alarm stopped via button!");
      digitalWrite(buzzerPin, LOW);
      buzzerActive = false;
      knockHappened = false;
      buzzerState = false;
    }
    else {
      if(millis() - lastBuzzerEvent >= buzzerInterval) {
        buzzerState = !buzzerState;
        digitalWrite(buzzerPin, buzzerState);
        lastBuzzerEvent = millis();
      }
    }
  }
  else {
    if(knockHappened) {
       if(millis() - lastKnock >= knockWait) {
         Serial.println("Alarm started at: " + String(millis()));
         digitalWrite(buzzerPin, HIGH);
         lastBuzzerEvent = millis();
         buzzerActive = true;
         buzzerState = true;
       }
    }
    else {
      int knockLevel = Serial.parseInt();
      if(knockLevel >= knockTreshold) {
        knockHappened = true;
        lastKnock = millis();
        Serial.println("Knock received at " + String(lastKnock));
      }
    }
  }
}
