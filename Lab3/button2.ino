const int buttonPin = 2;
const int ledPin = 13;
int ledValue = 0;
int buttonValue = 0;
int counter = 0;
int previousButtonValue = 0;

int previousDebounceTime = 0;
int debounceDelay = 50;

void setup() {
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
    Serial.begin(9600);
}

void loop() {
    int reading = digitalRead(buttonPin);

    if(reading != previousButtonValue) {
      previousDebounceTime = millis();
    }
    if(millis() - previousDebounceTime > debounceDelay) {
      if(reading != buttonValue){
        buttonValue = reading;
        if(buttonValue == HIGH) {
         ledValue = !ledValue; 
        }
      }
    }
    digitalWrite(ledPin, ledValue);
    previousDebounceTime = reading;
}
