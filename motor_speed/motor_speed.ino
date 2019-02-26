/*
  Serial communicaton:
  (1) Send string from Arduino Pro-mini to PC (show on serial screen)
  (2) Receive string from PC (in serial screen), then show it again on PC
  
 Created 23 Dec. 2016
 This example code is in the public domain.

 http://engineer2you.blogspot.com
 */
 
const byte pin_a_1 = 2;   //for encoder pulse A
const byte pin_b_1 = 3;   //for encoder pulse B
const byte pin_fwd_1 = 4; //for H-bridge: run motor forward
const byte pin_bwd_1 = 5; //for H-bridge: run motor backward
const byte pin_pwm_1 = 6; //for H-bridge: motor speed
int encoder_1 = 0;
int m_direction_1 =0;
int sv_speed_1 =30;     //this value is 0~255
double pv_speed_1 = 0;
int timer1_counter; //for timer


const byte pin_a_2 = 18;   //for encoder pulse A
const byte pin_b_2 = 19;   //for encoder pulse B
const byte pin_fwd_2 = 22; //for H-bridge: run motor forward
const byte pin_bwd_2 = 24; //for H-bridge: run motor backward
const byte pin_pwm_2 = 7; //for H-bridge: motor speed
int encoder_2 = 0;
int m_direction_2 = 0;
int sv_speed_2 = 0;     //this value is 0~255
double pv_speed_2 =0;
int timer3_counter; //for timer



const byte pin_a_3 = 20;   //for encoder pulse A
const byte pin_b_3 = 21;   //for encoder pulse B
const byte pin_fwd_3 = 26; //for H-bridge: run motor forward
const byte pin_bwd_3 = 28; //for H-bridge: run motor backward
const byte pin_pwm_3 = 8; //for H-bridge: motor speed
int encoder_3 = 0;
int m_direction_3 = 0;
int sv_speed_3 = 0;     //this value is 0~255
double pv_speed_3 = 0;
int timer5_counter; //for timer



void setup() {
  pinMode(pin_a_1,INPUT_PULLUP);
  pinMode(pin_b_1,INPUT_PULLUP);
  pinMode(pin_fwd_1,OUTPUT);
  pinMode(pin_bwd_1,OUTPUT);
  pinMode(pin_pwm_1,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(pin_a_1), detect_a_1, RISING);


  pinMode(pin_a_2,INPUT_PULLUP);
  pinMode(pin_b_2,INPUT_PULLUP);
  pinMode(pin_fwd_2,OUTPUT);
  pinMode(pin_bwd_2,OUTPUT);
  pinMode(pin_pwm_2,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(pin_a_2), detect_a_2, RISING);


  pinMode(pin_a_3,INPUT_PULLUP);
  pinMode(pin_b_3,INPUT_PULLUP);
  pinMode(pin_fwd_3,OUTPUT);
  pinMode(pin_bwd_3,OUTPUT);
  pinMode(pin_pwm_3,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(pin_a_3), detect_a_3, RISING);
  
  
  // start serial port at 9600 bps:
  Serial.begin(9600);
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
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

}

void loop() {
    digitalWrite(pin_fwd_1,0);        //run motor backward
    digitalWrite(pin_bwd_1,1);        //run motor backward
    analogWrite(pin_pwm_1,sv_speed_1);  //set motor speed
    Serial.print("speed_1 (rpm) is : ");
    Serial.print(pv_speed_1);         //Print speed value to Computer
    Serial.print("  --  ");
    Serial.print("Direction_1 = ");
    Serial.println(m_direction_1);
    
    delay (500);

    digitalWrite(pin_fwd_2,0);        //run motor backward
    digitalWrite(pin_bwd_2,1);        //run motor backward
    analogWrite(pin_pwm_2,sv_speed_2);  //set motor speed
    Serial.print("speed_2 (rpm) is : ");
    Serial.print(pv_speed_2);         //Print speed value to Computer
    Serial.print("  --  ");
    Serial.print("Direction_2 = ");
    Serial.println(m_direction_2);

    delay (500);

    digitalWrite(pin_fwd_3,0);        //run motor backward
    digitalWrite(pin_bwd_3,1);        //run motor backward
    analogWrite(pin_pwm_3,sv_speed_3);  //set motor speed
    Serial.print("speed_3 (rpm) is : ");
    Serial.print(pv_speed_3);         //Print speed value to Computer
    Serial.print("  --  ");
    Serial.print("Direction_3 = ");
    Serial.println(m_direction_3);
    
    Serial.print('\n');
    
    delay(500);
}
 
void detect_a_1() {
  encoder_1 +=1;
  m_direction_1 = digitalRead(pin_b_1);
}
ISR(TIMER1_OVF_vect)        // interrupt service routine - tick every 0.5sec
{
  TCNT1 = timer1_counter;   // set timer
  pv_speed_1 = 60*(encoder_1 /200.0)*3*0.82/0.5;
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


