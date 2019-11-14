const int potPin = A0;
const int ledPin = 9;
int potValue = 0;
int voltage = 0;
int ledValue = 0;   

void setup() {
Serial.begin(9600);  
  pinMode(potPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  potValue = analogRead(potPin);
  ledValue =  map(potValue, 0,1023,0,255);
  analogWrite(ledPin, ledValue);
}
