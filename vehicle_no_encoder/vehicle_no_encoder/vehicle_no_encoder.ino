/*
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

int forw_speed_1 = 0;
int forw_speed_2 = 0;
int forw_speed_3 = 0;


int set_speed_1 = 0;
int set_speed_2 = 0;
int set_speed_3 = 0;

int speed_mode = 0;
int mode_changed = 0;

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00011";

struct Message{
  uint8_t js0;
  uint8_t js1;
  uint8_t js2;
  uint8_t js3;
  uint8_t js_button;
};
Message k,lm;

int mag = 0;

long ch1;
long ch2;

int rot_in = 0;

const int m1_in1 = 4;
const int m1_in2 = 5;
const int m1_en = 6;
const int m2_in1 = 22;
const int m2_in2 = 24;
const int m2_en = 7;
const int m3_in1 = 26;
const int m3_in2 = 28;
const int m3_en = 8;

//encoders
const int pin_a_1 = 2;
const int pin_b_1 = 3;
const int pin_a_2 = 18;
const int pin_b_2 = 19;
const int pin_a_3 = 20;
const int pin_b_3 = 21;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  pinMode(pin_a_1,INPUT_PULLUP);
  pinMode(pin_b_1,INPUT_PULLUP);
  pinMode(m1_in1,OUTPUT);
  pinMode(m1_in2,OUTPUT);
  pinMode(m1_en,OUTPUT);

  pinMode(pin_a_2,INPUT_PULLUP);
  pinMode(pin_b_2,INPUT_PULLUP);
  pinMode(m2_in1,OUTPUT);
  pinMode(m2_in2,OUTPUT);
  pinMode(m2_en,OUTPUT);

  pinMode(pin_a_3,INPUT_PULLUP);
  pinMode(pin_b_3,INPUT_PULLUP);
  pinMode(m3_in1,OUTPUT);
  pinMode(m3_in2,OUTPUT);
  pinMode(m3_en,OUTPUT);     
  
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
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
  k.js0 = 127;
  k.js1 = 127;
  k.js2 = 127;
  k.js3 = 127;
  
}
void loop() {  
  if(radio.available()) {
    lm = k;
    radio.read(&k, sizeof(k));
  }

  if(k.js0 == 0 && k.js1 == 0 && k.js2 == 0 && k.js3 == 0){
//    k.js0 = 127;
//    k.js1 = 127;
//    k.js2 = 127;
//    k.js3 = 127;
    k = lm;
  }

  //Debug
  Serial.print(k.js0);
  Serial.print(" ");
  Serial.print(k.js1);
  Serial.print(" ");
  Serial.print(k.js2);
  Serial.print(" ");
  Serial.print(k.js3);
  Serial.print(" ");
  Serial.print(speed_mode);
  Serial.println(" ");
  
  if(k.js_button){
    if(mode_changed == 0){
      if(speed_mode == 3){
        speed_mode = 0;
        mode_changed = 1;
      }else{
        speed_mode += 1;
        mode_changed = 1;
      }
    }
    digitalWrite(12,HIGH);
  }else{
    digitalWrite(12,LOW);
    mode_changed = 0;
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
//  Serial.print("Theta: ");
//  Serial.print(theta);
//  Serial.println(" ");
  float magnitude = sqrt((ch1*ch1) + (ch2*ch2));
  
  if(magnitude > 25.0f && (rot_in > 100 && rot_in < 150)){
    float Vx = magnitude * cos(theta);
    float Vy = magnitude * sin(theta);
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
    

    if(theta > -2.355 && theta < -0.785){
      if(speed_mode == 0){
        set_speed_1 = 0;
        set_speed_2 = 78;
        set_speed_3 = 75;
      }else if(speed_mode == 1){
        set_speed_1 = 0;
        set_speed_2 = 98;
        set_speed_3 = 95;
      }else if(speed_mode == 2){
        set_speed_1 = 0;
        set_speed_2 = 125;
        set_speed_3 = 128;
      }else if(speed_mode == 3){
        set_speed_1 = 0;
        set_speed_2 = 225;
        set_speed_3 = 228;
      }
      
    }else if(theta > 0.785 && theta < 2.355){
      if(speed_mode == 0){
        set_speed_1 = 0;
        set_speed_2 = 75;
        set_speed_3 = 78;
      }else if(speed_mode == 1){
        set_speed_1 = 0;
        set_speed_2 = 95;
        set_speed_3 = 98;
      }else if(speed_mode == 2){
        set_speed_1 = 0;
        set_speed_2 = 125;
        set_speed_3 = 128;
      }else if(speed_mode == 3){
        set_speed_1 = 0;
        set_speed_2 = 225;
        set_speed_3 = 228;
      }
      
    }else if(theta > -0.785 && theta < 0.785){
      if(speed_mode == 0){
        set_speed_1 = 75;
        set_speed_2 = 53;
        set_speed_3 = 53;
      }else if(speed_mode == 1){
        set_speed_1 = 103;
        set_speed_2 = 68;
        set_speed_3 = 68;
      }else if(speed_mode == 2){
        set_speed_1 = 123;
        set_speed_2 = 78;
        set_speed_3 = 78;
      }else if(speed_mode == 3){
        set_speed_1 = 213;
        set_speed_2 = 100;
        set_speed_3 = 100;
      }
      
    }else if(theta > 2.355 || theta < -2.355){
      if(speed_mode == 0){
        set_speed_1 = 75;
        set_speed_2 = 53;
        set_speed_3 = 53;
      }else if(speed_mode == 1){
        set_speed_1 = 110;
        set_speed_2 = 60;
        set_speed_3 = 60; 
      }else if(speed_mode == 2){
        set_speed_1 = 135;
        set_speed_2 = 75;
        set_speed_3 = 75;
      }else if(speed_mode == 3){
        set_speed_1 = 245;
        set_speed_2 = 95;
        set_speed_3 = 95;
      }
      
    }else{
      set_speed_1 = 0;
      set_speed_2 = 0;
      set_speed_3 = 0;
    }
    
    analogWrite(m1_en,set_speed_1);
    analogWrite(m2_en,set_speed_2);
    analogWrite(m3_en,set_speed_3);
  }else if(rot_in < 100 || rot_in > 150){
    if(k.js2 < 127){
      mag = (127 - k.js2);
      digitalWrite(m1_in1,LOW);
      digitalWrite(m1_in2,HIGH);
      digitalWrite(m2_in1,LOW);
      digitalWrite(m2_in2,HIGH);
      digitalWrite(m3_in1,LOW);
      digitalWrite(m3_in2,HIGH);
    }else{
      mag = (k.js2 - 127);
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
  
    //Serial.println(mag);
  
    analogWrite(m1_en,mag);
    analogWrite(m2_en,mag);
    analogWrite(m3_en,mag);
  }else{
    set_speed_1 = 0;
    set_speed_2 = 0;
    set_speed_3 = 0;
    analogWrite(m1_en,set_speed_1);
    analogWrite(m2_en,set_speed_2);
    analogWrite(m3_en,set_speed_3);
  }
  delay(10);
}
