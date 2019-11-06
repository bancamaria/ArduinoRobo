void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT); //pin, type(type can be input or output)
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, LOW); //pin, value
  // delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
}
