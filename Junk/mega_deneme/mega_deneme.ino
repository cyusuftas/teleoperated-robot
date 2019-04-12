struct Message{
  int js0;
  int js1;
  int js2;
  int js3;
  int js_button;
};

Message k;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(8,OUTPUT);  //in1
  pinMode(9,OUTPUT);  //in2
  pinMode(5,OUTPUT);  //enA
  digitalWrite(8,LOW);
  digitalWrite(9,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()> 4){
    k.js0 = Serial.read();
    k.js1 = Serial.read();
    k.js2 = Serial.read();
    k.js3 = Serial.read();
    k.js_button = Serial.read();

    if(k.js_button == 1){
      digitalWrite(LED_BUILTIN, LOW);  
    }else{
      digitalWrite(LED_BUILTIN, HIGH);
    }

    analogWrite(5, k.js0);
  }

  delay(10);
}
