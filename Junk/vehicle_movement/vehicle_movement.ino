/*
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//#include <math.h>

RF24 radio(9, 10); // CE, CSN
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

long ch1;
long ch2;

int rot_in = 0;

int m1_in1 = 4;
int m1_in2 = 5;
int m1_en = 6;
int m2_in1 = 22;
int m2_in2 = 24;
int m2_en = 7;
int m3_in1 = 26;
int m3_in2 = 28;
int m3_en = 8;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();

  pinMode(m1_in1,OUTPUT);     //motor1 in1 
  pinMode(m1_in2,OUTPUT);     //motor1 in2
  pinMode(m1_en,OUTPUT);     //motor1 enA
  pinMode(m2_in1,OUTPUT);     //motor2 in3 
  pinMode(m2_in2,OUTPUT);     //motor2 in4
  pinMode(m2_en,OUTPUT);     //motor2 enB
  pinMode(m3_in1,OUTPUT);     //motor3 in3
  pinMode(m3_in2,OUTPUT);     //motor3 in4
  pinMode(m3_en,OUTPUT);     //motor3 enB

  pinMode(12,OUTPUT);
}
void loop() {
  if(radio.available()) {
    radio.read(&k, sizeof(k));
  }

  if(k.js_button){
    digitalWrite(12,HIGH);
  }else{
    digitalWrite(12,LOW);
  }

  ch1 = k.js0 - 127;
  ch2 = -(k.js1 - 127);
  rot_in = k.js2;

  if(ch1 < -127){
    ch1 = -127;
  }else if(ch1 > 127){
    ch1 = 127;
  }
  if(ch2 < -127){
    ch2 = -127;
  }else if(ch2 > 127){
    ch2 = 127;
  }

  float theta = atan2(ch2,ch1);
  Serial.print("Theta: ");
  Serial.print(theta);
  Serial.print(" ");
  float magnitude = sqrt((ch1*ch1) + (ch2*ch2));

  Serial.print("Magnitude: ");
  Serial.print(magnitude);
  Serial.println(" ");
  
  if(magnitude > 25.0f && (rot_in > 100 && rot_in < 150)){
    float Vx = magnitude * cos(theta);
    float Vy = magnitude * sin(theta);
    Serial.print("Vx: ");
    Serial.print(Vx);
    Serial.print(" ");
    Serial.print("Vy: ");
    Serial.print(Vy);
    Serial.println(" ");
    const float sqrt3o2 = 1.0 * sqrt(3) / 2;

    float w0 = -Vx;
    float w1 = 0.5*Vx - sqrt3o2 * Vy;
    float w2 = 0.5*Vx + sqrt3o2 * Vy; 

    if(w0 < 0){
      digitalWrite(m1_in1,LOW);
      digitalWrite(m1_in2,HIGH);
    }else{
      digitalWrite(m1_in1,HIGH);
      digitalWrite(m1_in2,LOW);
    }

    if(w1 < 0){
      digitalWrite(m2_in1,LOW);
      digitalWrite(m2_in2,HIGH);
    }else{
      digitalWrite(m2_in1,HIGH);
      digitalWrite(m2_in2,LOW);
    }

    if(w2 < 0){
      digitalWrite(m3_in1,LOW);
      digitalWrite(m3_in2,HIGH);
    }else{
      digitalWrite(m3_in1,HIGH);
      digitalWrite(m3_in2,LOW);
    }

    byte w0_speed = (byte) map(abs(w0), 0, 127, 0, 255);
    byte w1_speed = (byte) map(abs(w1), 0, 127, 0, 255);
    byte w2_speed = (byte) map(abs(w2), 0, 127, 0, 255);

    analogWrite(m1_en,w0_speed);
    analogWrite(m2_en,w1_speed);
    analogWrite(m3_en,w2_speed);    
  }else if(rot_in < 100 || rot_in > 150){
    if(k.js2 < 127){
    mag = (127 - k.js2) * 2;
    digitalWrite(m1_in1,LOW);
    digitalWrite(m1_in2,HIGH);
    digitalWrite(m2_in1,LOW);
    digitalWrite(m2_in2,HIGH);
    digitalWrite(m3_in1,LOW);
    digitalWrite(m3_in2,HIGH);
  }else{
    mag = (k.js2 - 127) * 2;
    digitalWrite(m1_in1,HIGH);
    digitalWrite(m1_in2,LOW);
    digitalWrite(m2_in1,HIGH);
    digitalWrite(m2_in2,LOW);
    digitalWrite(m3_in1,HIGH);
    digitalWrite(m3_in2,LOW);
  }
  
  
  if(mag>255){
    mag = 255;
  }
  else if(mag<0){
     mag = 0;
  }

  Serial.println(mag);

  analogWrite(m1_en,mag);
  analogWrite(m2_en,mag);
  analogWrite(m3_en,mag);
  }else{
    analogWrite(m1_en,0);
    analogWrite(m2_en,0);
    analogWrite(m3_en,0);
  }
  
/* rotational motion by using a joystick axis:
  Serial.println(k.js3);
    
  if(k.js3 < 127){
    mag = (127 - k.js3) * 2;
    digitalWrite(22,LOW);
    digitalWrite(24,HIGH);
    digitalWrite(31,LOW);
    digitalWrite(33,HIGH);
    digitalWrite(38,LOW);
    digitalWrite(40,HIGH);
  }else{
    mag = (k.js3 - 127) * 2;
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

  analogWrite(3,mag);
  analogWrite(4,mag);
  analogWrite(5,mag);
*/
  delay(10);
}
