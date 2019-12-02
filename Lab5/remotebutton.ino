#include <IRremote.h>
const int receiverPin = 8;
IRrecv irrecv(receiverPin);
decode_results results;
unsigned long keyValue = 0;
void setup(){
Serial.begin(9600);
irrecv.enableIRIn(); // configure the baud rate and start the IR receiver
irrecv.blink13(true); // blinks the LED everytime it receives a signal
}
void loop(){
if (irrecv.decode(&results)){
if (results.value == 0XFFFFFFFF)
results.value = keyValue;
switch(results.value){
case 0xFFA25D:
Serial.println("CH-");
break;
case 0xFF629D:
Serial.println("CH");
break;
case 0xFFE21D:
Serial.println("CH+");
break;
case 0xFF22DD:
Serial.println("|<<");
break;
case 0xFF02FD:
Serial.println(">>|");
break ;
case 0xFFC23D:
Serial.println(">|");
break ;
case 0xFFE01F:
Serial.println("-");
break ;
case 0xFFA857:
Serial.println("+");
break ;
case 0xFF906F:
Serial.println("EQ");
break ;
case 0xFF6897:
Serial.println("0");
break ;
case 0xFF9867:
Serial.println("100+");
break ;
case 0xFFB04F:
Serial.println("200+");
break ;
case 0xFF30CF:
Serial.println("1");
break ;
case 0xFF18E7:
Serial.println("2");
break ;
case 0xFF7A85:
Serial.println("3");
break ;
case 0xFF10EF:
Serial.println("4");
break ;
case 0xFF38C7:
Serial.println("5");
break ;
case 0xFF5AA5:
Serial.println("6");
break ;
case 0xFF42BD:
Serial.println("7");
break ;
case 0xFF4AB5:
Serial.println("8");
break ;
case 0xFF52AD:
Serial.println("9");
break ;
}
keyValue = results.value;
irrecv.resume();
}
}
