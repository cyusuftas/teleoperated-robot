/*
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

int mag = 0;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  pinMode(22,OUTPUT);     //motor1 in1 
  pinMode(24,OUTPUT);     //motor1 in2
  pinMode(26,OUTPUT);     //motor1 enA
  pinMode(31,OUTPUT);     //motor2 in3 
  pinMode(33,OUTPUT);     //motor2 in4
  pinMode(35,OUTPUT);     //motor2 enB
  pinMode(38,OUTPUT);     //motor3 in3
  pinMode(40,OUTPUT);     //motor3 in4
  pinMode(42,OUTPUT);     //motor3 enB
}
void loop() {
  if(radio.available()) {
    radio.read(&k, sizeof(k));
  }

  Serial.println(k.js0);
    
  if(k.js0 < 127){
    mag = (127 - k.js0) * 2;
    digitalWrite(22,LOW);
    digitalWrite(24,HIGH);
    digitalWrite(31,LOW);
    digitalWrite(33,HIGH);
    digitalWrite(38,LOW);
    digitalWrite(40,HIGH);
  }else{
    mag = (k.js0 - 127) * 2;
    digitalWrite(22,HIGH);
    digitalWrite(24,LOW);
    digitalWrite(31,HIGH);
    digitalWrite(33,LOW);
    digitalWrite(38,HIGH);
    digitalWrite(40,LOW);
  }
  
  
  if(mag>255){
    mag = 255;
  }
  else if(mag<0){
     mag = 0;
  }

  Serial.println(mag);

  analogWrite(26,mag);
  analogWrite(35,mag);
  analogWrite(42,mag);

  delay(10);
}
