/*
* Arduino Wireless Communication Tutorial
*     Example 1 - Transmitter Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

struct Message{
  int js0;
  int js1;
  int js2;
  int js3;
  int js_button;
};

Message k;

void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}
void loop() {
  if(Serial.available()> 4){
    k.js0 = Serial.read();
    k.js1 = Serial.read();
    k.js2 = Serial.read();
    k.js3 = Serial.read();
    k.js_button = Serial.read();

    if(k.js_button == 1){
      digitalWrite(LED_BUILTIN, LOW);  
    }else{
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
  //Serial.println(k.js0);
  //const char text[] = "Hello World";
  //radio.write(&k.js0, sizeof(k.js0));
  radio.write(&k, sizeof(k));
  //Serial.println(sizeof(k));
  delay(10);
}
