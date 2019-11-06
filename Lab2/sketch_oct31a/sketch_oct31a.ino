const int ledPin = 13;
int ledState = LOW; // 0
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
const long interval = 1000;

void setup() {
  pinMode(13, OUTPUT);
}

void loop() {
  currentMillis = millis();
  if(currentMillis - previousMillis >= interval){
    previousMillis = currentMillis;
    // ledState = !ledState; // 1,0 -> HIGH AND LOW
    if(ledState == LOW){
      ledState = HIGH;
    }else{
      ledState = LOW;
    }
  }
  digitalWrite(ledPin   , ledState);
}
