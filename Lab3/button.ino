const int buttonPin = 2;
const int ledPin = 13;
int ledValue = 0;
int buttonValue = 0;
int counter = 0;
int previousButtonValue = 0;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  buttonValue = digitalRead(buttonPin);
  if(previousButtonValue != buttonValue) {
    if(buttonValue == LOW) {
      counter++;
      ledValue = !ledValue;
    }
    previousButtonValue = buttonValue;
  }
  
  digitalWrite(ledPin, ledValue);
  Serial.println(buttonValue);
  
}
