/*
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//#include <math.h>

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

long ch1;
long ch2;

int rot_in = 0;


void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  pinMode(22,OUTPUT);     //motor1 in1 
  pinMode(24,OUTPUT);     //motor1 in2
  pinMode(3,OUTPUT);     //motor1 enA
  pinMode(31,OUTPUT);     //motor2 in3 
  pinMode(33,OUTPUT);     //motor2 in4
  pinMode(4,OUTPUT);     //motor2 enB
  pinMode(38,OUTPUT);     //motor3 in3
  pinMode(40,OUTPUT);     //motor3 in4
  pinMode(5,OUTPUT);     //motor3 enB
}
void loop() {
  if(radio.available()) {
    radio.read(&k, sizeof(k));
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
      digitalWrite(22,LOW);
      digitalWrite(24,HIGH);
    }else{
      digitalWrite(22,HIGH);
      digitalWrite(24,LOW);
    }

    if(w1 < 0){
      digitalWrite(31,LOW);
      digitalWrite(33,HIGH);
    }else{
      digitalWrite(31,HIGH);
      digitalWrite(33,LOW);
    }

    if(w2 < 0){
      digitalWrite(38,LOW);
      digitalWrite(40,HIGH);
    }else{
      digitalWrite(38,HIGH);
      digitalWrite(40,LOW);
    }

    byte w0_speed = (byte) map(abs(w0), 0, 127, 0, 255);
    byte w1_speed = (byte) map(abs(w1), 0, 127, 0, 255);
    byte w2_speed = (byte) map(abs(w2), 0, 127, 0, 255);

    analogWrite(3,w0_speed);
    analogWrite(4,w1_speed);
    analogWrite(5,w2_speed);    
  }else if(rot_in < 100 || rot_in > 150){
    if(k.js2 < 127){
    mag = (127 - k.js2) * 2;
    digitalWrite(22,LOW);
    digitalWrite(24,HIGH);
    digitalWrite(31,LOW);
    digitalWrite(33,HIGH);
    digitalWrite(38,LOW);
    digitalWrite(40,HIGH);
  }else{
    mag = (k.js2 - 127) * 2;
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
  }else{
    analogWrite(3,0);
    analogWrite(4,0);
    analogWrite(5,0);
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
