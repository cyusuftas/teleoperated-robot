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
const byte addresses[][6] = {"00001", "00002"};

struct Message{
  uint8_t js0;
  uint8_t js1;
  uint8_t js2;
  uint8_t js3;
  uint8_t js_button;
  int count;
  uint8_t rate;
  uint8_t speed_mode;
} k;

struct Telemetry{
  uint8_t a;
  uint8_t b;
  uint8_t m1_rpm;
  uint8_t m2_rpm;
  uint8_t m3_rpm;
  uint8_t speed_mode;
  int battery;
} telem;

unsigned long t_minus_1 = 0;
unsigned long t;
unsigned long time_dif;



void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(addresses[1]); // 00002
  radio.openReadingPipe(1, addresses[0]); // 00001
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(3,OUTPUT);
  k.count = 0;
  telem.a = 1;
  k.rate = 10;
}
void loop() {
//  t = millis();
//  time_dif = t - t_minus_1;
//  t_minus_1 = t;
//  Serial.println(time_dif);
  if(Serial.available()> 6){
    k.js0 = Serial.read();
    k.js1 = Serial.read();
    k.js2 = Serial.read();
    k.js3 = Serial.read();
    k.js_button = Serial.read();
    k.rate = Serial.read();
    k.speed_mode = Serial.read();
  }
  
  if(k.count == k.rate){
    radio.startListening();
    t_minus_1 = millis();
    while(!radio.available()){
      t = millis();
      time_dif = t - t_minus_1;
      if(time_dif > 100){
        radio.stopListening();
        Serial.println("break");
        break;
      }
    }
    radio.read(&telem, sizeof(telem));
    Serial.print(telem.a);
    Serial.print(",");
    Serial.print(telem.b);
    Serial.print(",");
    Serial.print(telem.m1_rpm);
    Serial.print(",");
    Serial.print(telem.m2_rpm);
    Serial.print(",");
    Serial.print(telem.m3_rpm);
    Serial.print(",");
    Serial.println(telem.battery);
    k.count = 0;
  }else{
    radio.stopListening();
    radio.write(&k, sizeof(k));
    k.count += 1;
  }
  
  delay(10);
  
}
