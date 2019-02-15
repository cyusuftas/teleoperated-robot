/*Cahit Yusuf Tas - cyusuftas@gmail.com
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

struct Message{
  int a;
  int b;
  float x;
  float z;
};
unsigned long time1,time2;
//int val;

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";    //address

Message k = {5,10,0.513,-0.252};    //create message

void setup() {
  radio.begin();
  radio.openWritingPipe(address);   //set as tx
  radio.setPALevel(RF24_PA_MAX);    //set max power level
  radio.stopListening();            //it is not set as rx
  Serial.begin(9600);
  pinMode(5,OUTPUT);                //set pulse pin as output
}

void loop() {
  digitalWrite(5,LOW);
  
  if(Serial.available()>0){
    k.b = Serial.read();            //read from serial
  } 
  
  k.a = analogRead(3);              //k.a defines brightness of led
  Serial.println(k.b);

  time1 = micros();
  radio.write(&k, sizeof(k));       //send message
  time2 = micros();
  digitalWrite(5,HIGH);             //pulse pin goes to high
  delay(10);
  
  Serial.println(time2-time1);      //print delay
}
