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

//int speed_mode = 0;
int mode_changed = 0;

const int solenoid_pin = 11;

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
} k, lm;

struct Telemetry{
  uint8_t a;
  uint8_t b;
  uint8_t m1_rpm;
  uint8_t m2_rpm;
  uint8_t m3_rpm;
  uint8_t speed_mode;
  int battery;
} telem;

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

//encoder connections
const int pin_a_1 = 2;
const int pin_b_1 = 3;
const int pin_a_2 = 18;
const int pin_b_2 = 19;
const int pin_a_3 = 20;
const int pin_b_3 = 21;

//encoder variables
int encoder_1 = 0;
int m_direction_1 =0;
double pv_speed_1 = 0;
int timer1_counter; //for timer

int encoder_2 = 0;
int m_direction_2 = 0;
double pv_speed_2 =0;
int timer3_counter; //for timer

int encoder_3 = 0;
int m_direction_3 = 0;
double pv_speed_3 = 0;
int timer5_counter; //for timer

void setup() {
  pinMode(solenoid_pin, OUTPUT);
  digitalWrite(solenoid_pin, HIGH);
  
  pinMode(LED_BUILTIN, OUTPUT);
  
  pinMode(pin_a_1,INPUT_PULLUP);
  pinMode(pin_b_1,INPUT_PULLUP);
  pinMode(m1_in1,OUTPUT);
  pinMode(m1_in2,OUTPUT);
  pinMode(m1_en,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(pin_a_1), detect_a_1, RISING);

  pinMode(pin_a_2,INPUT_PULLUP);
  pinMode(pin_b_2,INPUT_PULLUP);
  pinMode(m2_in1,OUTPUT);
  pinMode(m2_in2,OUTPUT);
  pinMode(m2_en,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(pin_a_2), detect_a_2, RISING);

  pinMode(pin_a_3,INPUT_PULLUP);
  pinMode(pin_b_3,INPUT_PULLUP);
  pinMode(m3_in1,OUTPUT);
  pinMode(m3_in2,OUTPUT);
  pinMode(m3_en,OUTPUT);  
  attachInterrupt(digitalPinToInterrupt(pin_a_3), detect_a_3, RISING);

  //--------------------------timer setup
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  
  timer1_counter = 34286;   // preload timer 65536-16MHz/256/2Hz
  
  TCNT1 = timer1_counter;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt


  TCCR3A = 0;
  TCCR3B = 0;
  
  timer3_counter = 34286;   // preload timer 65536-16MHz/256/2Hz
  
  TCNT3 = timer3_counter;   // preload timer
  TCCR3B |= (1 << CS32);    // 256 prescaler 
  TIMSK3 |= (1 << TOIE3);   // enable timer overflow interrupt


  TCCR5A = 0;
  TCCR5B = 0;
  
  timer5_counter = 34286;   // preload timer 65536-16MHz/256/2Hz
  
  TCNT5 = timer5_counter;   // preload timer
  TCCR5B |= (1 << CS52);    // 256 prescaler 
  TIMSK5 |= (1 << TOIE5);   // enable timer overflow interrupt
  
  
  interrupts();             // enable all interrupts
  //--------------------------timer setup
  
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
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
  
  k.js0 = 127;
  k.js1 = 127;
  k.js2 = 127;
  k.js3 = 127;

  telem.a = 3;
  telem.b = 10;
  telem.battery = 89;
  k.speed_mode = 2;  
}
void loop() {
  if(radio.available()) {
    lm = k;
    radio.read(&k, sizeof(k));
  }else{
    //Serial.println("a");
  }
  
  if(k.count == k.rate - 1){
    radio.stopListening();
    //Serial.println(telem.a);
    telem.m1_rpm = pv_speed_1;
    telem.m2_rpm = pv_speed_2;
    telem.m3_rpm = pv_speed_3;
    telem.battery = map(analogRead(A4),0,1023,0,500);
    radio.write(&telem, sizeof(telem));
    if(telem.a > 50){
      telem.a = 0;
    }else{
      telem.a += 1;
    }
    //Serial.println("telemetry sent");
    radio.startListening();
  }

  
  
  if(k.js0 == 0 && k.js1 == 0 && k.js2 == 0 && k.js3 == 0){
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
  Serial.print(k.js_button);
  Serial.println(" ");
  
  if(k.js_button == 1){
//    if(mode_changed == 0){
//      if(k.speed_mode == 3){
//        k.speed_mode = 0;
//        mode_changed = 1;
//      }else{
//        k.speed_mode += 1;
//        mode_changed = 1;
//      }
//    }
    digitalWrite(solenoid_pin,LOW);
  }else{
    digitalWrite(solenoid_pin,HIGH);
    //mode_changed = 0;
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

//    Serial.print(w0);
//    Serial.print(" ");
//    Serial.print(w1);
//    Serial.print(" ");
//    Serial.println(w2);

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
      //Serial.println("forward");
      if(k.speed_mode == 0){
        set_speed_1 = 0;
        set_speed_2 = 78;
        set_speed_3 = 75;
      }else if(k.speed_mode == 1){
        set_speed_1 = 0;
        set_speed_2 = 98;
        set_speed_3 = 95;
      }else if(k.speed_mode == 2){
        set_speed_1 = 0;
        set_speed_2 = 125;
        set_speed_3 = 128;
      }else if(k.speed_mode == 3){
        set_speed_1 = 0;
        set_speed_2 = 175;
        set_speed_3 = 175;
      }
      
    }else if(theta > 0.785 && theta < 2.355){
      //Serial.println("backward");
      if(k.speed_mode == 0){
        set_speed_1 = 0;
        set_speed_2 = 82;
        set_speed_3 = 78;
      }else if(k.speed_mode == 1){
        set_speed_1 = 0;
        set_speed_2 = 103;
        set_speed_3 = 98;
      }else if(k.speed_mode == 2){
        set_speed_1 = 0;
        set_speed_2 = 135;
        set_speed_3 = 128;
      }else if(k.speed_mode == 3){
        set_speed_1 = 0;
        set_speed_2 = 185;
        set_speed_3 = 175;
      }
      
    }else if(theta > -0.785 && theta < 0.785){
      //Serial.println("right");
      if(k.speed_mode == 0){
        set_speed_1 = 75;
        set_speed_2 = 53;
        set_speed_3 = 53;
      }else if(k.speed_mode == 1){
        set_speed_1 = 103;
        set_speed_2 = 68;
        set_speed_3 = 68;
      }else if(k.speed_mode == 2){
        set_speed_1 = 123;
        set_speed_2 = 78;
        set_speed_3 = 78;
      }else if(k.speed_mode == 3){
        set_speed_1 = 213;
        set_speed_2 = 100;
        set_speed_3 = 100;
      }
      
    }else if(theta > 2.355 || theta < -2.355){
      //Serial.println("left");
      if(k.speed_mode == 0){
        set_speed_1 = 75;
        set_speed_2 = 53;
        set_speed_3 = 53;
      }else if(k.speed_mode == 1){
        set_speed_1 = 110;
        set_speed_2 = 60;
        set_speed_3 = 60; 
      }else if(k.speed_mode == 2){
        //Serial.println("mode: 2");
        set_speed_1 = 135;
        set_speed_2 = 75;
        set_speed_3 = 75;
      }else if(k.speed_mode == 3){
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
  delay(20);
}

void detect_a_1() {
  encoder_1 +=1;
  m_direction_1 = digitalRead(pin_b_1);
}
ISR(TIMER1_OVF_vect)        // interrupt service routine - tick every 0.5sec
{
  TCNT1 = timer1_counter;   // set timer
  pv_speed_1 = 60*(encoder_1 /200.0)*3*0.82f/0.5;
  encoder_1 =0;
}

void detect_a_2() {
  encoder_2 +=1;
  m_direction_2 = digitalRead(pin_b_2);
}
ISR(TIMER3_OVF_vect)        // interrupt service routine - tick every 0.5sec
{
  TCNT3 = timer3_counter;   // set timer
  pv_speed_2 = 60*(encoder_2 /200.0)*3*0.82 /0.5;
  encoder_2 =0;
}

void detect_a_3() {
  encoder_3 +=1;
  m_direction_3 = digitalRead(pin_b_3);
}
ISR(TIMER5_OVF_vect)        // interrupt service routine - tick every 0.5sec
{
  TCNT5 = timer5_counter;   // set timer
  pv_speed_3 = 60*(encoder_3 /200.0)*3*0.82/0.5;
  encoder_3 =0;
}
