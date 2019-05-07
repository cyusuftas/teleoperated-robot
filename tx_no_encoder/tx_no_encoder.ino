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
RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00011";

struct Message{
  uint8_t js0;
  uint8_t js1;
  uint8_t js2;
  uint8_t js3;
  uint8_t js_button;
};

Message k;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(3,OUTPUT);
}
void loop() {
  if(Serial.available()> 4){
    k.js0 = Serial.read();
    k.js1 = Serial.read();
    k.js2 = Serial.read();
    k.js3 = Serial.read();
    k.js_button = Serial.read();
  }
  
  radio.write(&k, sizeof(k));
  
  delay(10);
  
}
