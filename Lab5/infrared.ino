#include <IRremote.h>
const int receiverPin = 8;

IRrecv irrecv(receiverPin);
decode_results results;

void setup(){
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
}

void loop(){
  if (irrecv.decode(&results)){
  Serial.println(results.value, HEX);
  irrecv.resume();
  }
}
