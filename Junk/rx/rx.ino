/*Cahit Yusuf Tas - cyusuftas@gmail.com
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
//#include <nRF24L01.h>
#include <RF24.h>

struct Message{
  int a;
  int b;
  float x;
  float z;
};
unsigned long time1,time2;
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
Message k; 
int val;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);      //set as receiver
  radio.setPALevel(RF24_PA_MAX);          //set max power level
  radio.startListening();                 //ready to receive signals
  pinMode(3,OUTPUT);                      //led pin
  pinMode(4,OUTPUT);                      //led pin
  digitalWrite(3,LOW);  
  pinMode(5,OUTPUT);                      //set pulse pin as output
}

void loop() {
  digitalWrite(5,LOW);                    //set led pin low
  if (radio.available()) {   
    radio.read(&k, sizeof(k));            //read message
    digitalWrite(5,HIGH);                 //after read, set pulse pin high
    delay(10);  
    
    //Serial.println(k.b);
    //Serial.println(val);
    val = map(k.a,0,1023,0,255);          //map received integer value
    analogWrite(3,val);                   //set brightness of led
    if(k.b == 49){                        //'1' = ascii 49
      digitalWrite(4,HIGH); 
    }else if( k.b == 48)                  //'0' = ascii 48
    {
      digitalWrite(4,LOW);
    }
  }else{
    digitalWrite(5,LOW);
    Serial.println("no signal");
  }
}
