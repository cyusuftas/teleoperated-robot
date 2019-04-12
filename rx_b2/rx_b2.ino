#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//////////////////////////////////
bool start = true;
int encoder_1 = 0;
int m_direction_1 = 0;
double pv_speed_1 = 0;
int timer1_counter; //for timer
double set_speed_1 = 0;
double e_speed_1 = 0; //error of speed = set_speed - pv_speed
double e_speed_pre_1 = 0;  //last error of speed
double e_speed_sum_1 = 0;  //sum error of speed
double pwm_pulse_1 = 0;     //this value is 0~255
double kp_1 = 3.3;
double ki_1 = 1.5;
double kd_1 = 0;


int encoder_2 = 0;
int m_direction_2 = 0;
double pv_speed_2 =0;
int timer3_counter; //for timer
double set_speed_2 = 0;
double e_speed_2 = 0; //error of speed = set_speed - pv_speed
double e_speed_pre_2 = 0;  //last error of speed
double e_speed_sum_2 = 0;  //sum error of speed
double pwm_pulse_2 = 0;     //this value is 0~255
double kp_2 = 3.3;
double ki_2 = 1.5;
double kd_2 = 0;


int encoder_3 = 0;
int m_direction_3 = 0;
double pv_speed_3 = 0;
int timer5_counter; //for timer
double set_speed_3 = 0;
double e_speed_3 = 0; //error of speed = set_speed - pv_speed
double e_speed_pre_3 = 0;  //last error of speed
double e_speed_sum_3 = 0;  //sum error of speed
double pwm_pulse_3 = 0;     //this value is 0~255
double kp_3 = 3.3;
double ki_3 = 1.5;
double kd_3 = 0;
//////////////////////////

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

//////


RF24 radio(9,10); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};
struct state{
  int m0_rpm;
  int m0_dir;
  int m1_rpm;
  int m1_dir;
  int m2_rpm;
  int m2_dir;
  int battery;
};
state st = {0,0,0,87};

struct my_js{
  int js0;
  int js1;
  int js2;
  int js3;
  int js_button;
  int count;
  int rate;
  int ct2;
  int rate2;
  int rot_speed;
  int veh_speed;
};
my_js k = {126,126,126,126,0,0,8,0,11,80,100};
int batt_pin = A4;

void setup() {
  /////
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

  
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  
  timer1_counter = 59286;   // preload timer 65536-16MHz/256/2Hz
  
  TCNT1 = timer1_counter;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  
  
  interrupts();            // enable all interrupts
  //--------------------------timer setup
  ///////
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
  radio.setPALevel(RF24_PA_MIN);
  ///////
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
  delay(5);
  //Serial.println(k.js0);
  radio.startListening();
  delay(5);
  if ( radio.available()) {
    while (radio.available()) {
      radio.read(&k, sizeof(k));
//      if(k.js0 > 150)
//        st.a = 1;
//      else
//        st.a = 0;
    }
    delay(5);
  }
  //st.b = k.rate2;
  Serial.println(k.rate2);
  if(k.ct2 == (k.rate2 - 1)){
    st.m0_rpm = pv_speed_1;
    st.m1_rpm = pv_speed_2;
    st.m2_rpm = pv_speed_3;
    st.m0_dir = m_direction_1;
    st.m1_dir = m_direction_2;
    st.m2_dir = m_direction_3;
    
    st.battery = int(analogRead(batt_pin)*500.0/1023.0);
    radio.stopListening();
    radio.write(&st, sizeof(st));
    Serial.println("sent");  
  }

  //////
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
  //Serial.print("Theta: ");
  //Serial.print(theta);
  //Serial.print(" ");
  float magnitude = sqrt((ch1*ch1) + (ch2*ch2));

  //Serial.print("Magnitude: ");
  //Serial.print(magnitude);
  //Serial.println(" ");
  
  if(magnitude > 25.0f && (rot_in > 100 && rot_in < 150)){
    float Vx = magnitude * cos(theta);
    float Vy = magnitude * sin(theta);
//    Serial.print("Vx: ");
//    Serial.print(Vx);
//    Serial.print(" ");
//    Serial.print("Vy: ");
//    Serial.print(Vy);
//    Serial.println(" ");
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
//previous
//    byte w0_speed = (byte) map(abs(w0), 0, 127, 0, 255);
//    byte w1_speed = (byte) map(abs(w1), 0, 127, 0, 255);
//    byte w2_speed = (byte) map(abs(w2), 0, 127, 0, 255);
    //for pid
    byte w0_speed = (byte) map(abs(w0), 0, 127, 0, k.veh_speed);
    byte w1_speed = (byte) map(abs(w1), 0, 127, 0, k.veh_speed);
    byte w2_speed = (byte) map(abs(w2), 0, 127, 0, k.veh_speed);

//    analogWrite(m1_en,w0_speed);
//    analogWrite(m2_en,w1_speed);
//    analogWrite(m3_en,w2_speed);

    set_speed_1 = w0_speed;
    set_speed_2 = w1_speed;
    set_speed_3 = w2_speed;
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
  
//    analogWrite(m1_en,mag);
//    analogWrite(m2_en,mag);
//    analogWrite(m3_en,mag);
    set_speed_1 = k.rot_speed;
    set_speed_2 = k.rot_speed;
    set_speed_3 = k.rot_speed;
  }else{
//    analogWrite(m1_en,0);
//    analogWrite(m2_en,0);
//    analogWrite(m3_en,0);
    set_speed_1 = 0;
    set_speed_2 = 0;
    set_speed_3 = 0;
  }
  
  delay(10);
  //////  
}

void detect_a_1() {
  encoder_1 +=1;
  m_direction_1 = digitalRead(pin_b_1);
}
ISR(TIMER1_OVF_vect)        // interrupt service routine - tick every 0.5sec
{
  TCNT1 = timer1_counter;   // set timer
  pv_speed_1 = 60*(encoder_1 /200.0)*3*0.82f/0.1;
  encoder_1 =0;

  pv_speed_2 = 60*(encoder_2 /200.0)*3*0.82 /0.1;
  encoder_2 =0;

  pv_speed_3 = 60*(encoder_3 /200.0)*3*0.82/0.1;
  encoder_3 =0;

  

  //PID program
  if (start){
    e_speed_1 = set_speed_1 - pv_speed_1;
    pwm_pulse_1 = e_speed_1*kp_1 + e_speed_sum_1*ki_1 + (e_speed_1 - e_speed_pre_1)*kd_1;
    e_speed_pre_1 = e_speed_1;  //save last (previous) error
    e_speed_sum_1 += e_speed_1; //sum of error
    if (e_speed_sum_1 >4000) e_speed_sum_1 = 4000;
    if (e_speed_sum_1 <-4000) e_speed_sum_1 = -4000;

    e_speed_2 = set_speed_2 - pv_speed_2;
    pwm_pulse_2 = e_speed_2*kp_2 + e_speed_sum_2*ki_2 + (e_speed_2 - e_speed_pre_2)*kd_2;
    e_speed_pre_2 = e_speed_2;  //save last (previous) error
    e_speed_sum_2 += e_speed_2; //sum of error
    if (e_speed_sum_2 >4000) e_speed_sum_2 = 4000;
    if (e_speed_sum_2 <-4000) e_speed_sum_2 = -4000;

    e_speed_3 = set_speed_3 - pv_speed_3;
    pwm_pulse_3 = e_speed_3*kp_3 + e_speed_sum_3*ki_3 + (e_speed_3 - e_speed_pre_3)*kd_3;
    e_speed_pre_3 = e_speed_3;  //save last (previous) error
    e_speed_sum_3 += e_speed_3; //sum of error
    if (e_speed_sum_3 >4000) e_speed_sum_3 = 4000;
    if (e_speed_sum_3 <-4000) e_speed_sum_3 = -4000;
  }
  else{
    e_speed_1 = 0;
    e_speed_pre_1 = 0;
    e_speed_sum_1 = 0;
    pwm_pulse_1 = 0;

    e_speed_2 = 0;
    e_speed_pre_2 = 0;
    e_speed_sum_2 = 0;
    pwm_pulse_2 = 0;

    e_speed_3 = 0;
    e_speed_pre_3 = 0;
    e_speed_sum_3 = 0;
    pwm_pulse_3 = 0;    
  }

  //update new speed
  if (pwm_pulse_1 <255 & pwm_pulse_1 >0){
    analogWrite(m1_en,pwm_pulse_1);  //set motor speed  
  }
  else{
    if (pwm_pulse_1>255){
      analogWrite(m1_en,255);
    }
    else{
      analogWrite(m1_en,0);
    }
  }

  //update new speed
  if (pwm_pulse_2 <255 & pwm_pulse_2 >0){
    analogWrite(m2_en,pwm_pulse_2);  //set motor speed  
  }
  else{
    if (pwm_pulse_2>255){
      analogWrite(m2_en,255);
    }
    else{
      analogWrite(m2_en,0);
    }
  }

  //update new speed
  if (pwm_pulse_3 <255 & pwm_pulse_3 >0){
    analogWrite(m3_en,pwm_pulse_3);  //set motor speed  
  }
  else{
    if (pwm_pulse_3>255){
      analogWrite(m3_en,255);
    }
    else{
      analogWrite(m3_en,0);
    }
  }  
}

void detect_a_2() {
  encoder_2 +=1;
  m_direction_2 = digitalRead(pin_b_2);
}

void detect_a_3() {
  encoder_3 +=1;
  m_direction_3 = digitalRead(pin_b_3);
}
