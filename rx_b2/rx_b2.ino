/*
* Arduino Wireless Communication Tutorial
*     Example 2 - Receiver Code
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
struct state{
  int a;
  int b;
};
state st = {0,5};

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
my_js k = {126,126,126,126,0,0,8,0};

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
  radio.setPALevel(RF24_PA_MIN);
}
void loop() {
  delay(5);
  Serial.println(k.js0);
  radio.startListening();
  delay(100);
  if ( radio.available()) {
    while (radio.available()) {
      radio.read(&k, sizeof(k));
      if(k.js0 > 150)
        st.a = 1;
      else
        st.a = 0;
    }
    delay(5);
  }
  st.b = k.ct2;
  Serial.println(k.ct2);
  if(k.ct2 == 10){
    radio.stopListening();
    radio.write(&st, sizeof(st));  
  }
}
