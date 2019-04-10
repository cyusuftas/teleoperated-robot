/*
* Arduino Wireless Communication Tutorial
*     Example 2 - Transmitter Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9,10); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};

struct my_js{
  int js0;
  int js1;
  int js2;
  int js3;
  int button;
  int count;
  int rate;
  int ct2;
};
my_js k = {127,127,127,127,0,0,8,0};
boolean sent = 0;

struct state{
  int a;
  int b;
};
state st;
int potValue = 0;
void setup() {
  //pinMode(12, OUTPUT);
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(addresses[1]); // 00002
  radio.openReadingPipe(1, addresses[0]); // 00001
  radio.setPALevel(RF24_PA_MIN);
  pinMode(LED_BUILTIN,OUTPUT);
}
void loop() {
  if(k.count == k.rate && !sent){
    Serial.print(k.js0);
    Serial.print(",");
    Serial.print(k.js1);
    Serial.print(",");
    Serial.print(st.a);
    Serial.print(",");
    Serial.println(st.b);
    sent = 1;  
  }else{
    if(Serial.available()>6){
      k.js0 = Serial.read();
      k.js1 = Serial.read();
      k.js2 = Serial.read();
      k.js3 = Serial.read();
      k.button = Serial.read();
      k.count = Serial.read();
      k.rate = Serial.read() - 1;  
    }
    sent = 0;   
  }
  
  delay(5);
  radio.stopListening();
  radio.write(&k, sizeof(k));
  k.ct2 += 1;
  delay(5);
  if(k.ct2 == 11){
    k.ct2 = 0;
    radio.startListening();
    while (!radio.available());
    radio.read(&st, sizeof(st));  
  }
  
//  Serial.print(st.a);
//  Serial.print(" ");
//  Serial.println(st.b);
  analogWrite(LED_BUILTIN, k.js0);
}
