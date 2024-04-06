#include <HCSR04.h>
//Distances
float R_dist; 
float L_dist;
float F_dist;

//Distances Between Sensors and the Wall
int F_THRESH = 12; //Fornt
int S_THRESH = 10; //Max Side
int SC_THRESH = 3;  //Min Side

//Vitess
int Vs_L = 70;
float Vs_R = 71.754744;
int VCmin = 60;
int Vcmax = 120;

int R_ECHO = 4;
int R_TRIG = 5;
HCSR04 R_hc(R_TRIG, R_ECHO);

int L_ECHO = 2;
int L_TRIG = 3;
HCSR04 L_hc(L_TRIG, L_ECHO);

int F_ECHO  = 7;
int F_TRIG  = 6;
HCSR04 F_hc(F_TRIG, F_ECHO);

// Right Motor
int R_ENB = 9; 
int R_IN3 = 8;
int R_IN4 = 11;
float L_v = Vs_L;
float R_v = Vs_R;

//Left Motor
int L_ENA = 10;
int L_IN1 = 12;
int L_IN2 = 13;


void setup(){
  Serial.begin(9600);

  pinMode(L_ENA,OUTPUT);
  pinMode(L_IN1,OUTPUT);
  pinMode(L_IN2,OUTPUT);
  pinMode(R_ENB,OUTPUT);
  pinMode(R_IN3,OUTPUT);
  pinMode(R_IN4,OUTPUT);

  digitalWrite(L_IN1,LOW);
  digitalWrite(L_IN2,LOW);
  digitalWrite(R_IN3,LOW);
  digitalWrite(R_IN4,LOW);

  TCCR1B = TCCR1B & B11111000 | B00000101;
}
void stop(int time){
  digitalWrite(L_IN1,LOW);
  digitalWrite(L_IN2,LOW);
  digitalWrite(R_IN3,LOW);
  digitalWrite(R_IN4,LOW);
  delay(time);
}
void advance(){
  digitalWrite(L_IN1,LOW);
  digitalWrite(L_IN2,HIGH);
  digitalWrite(R_IN3,LOW);
  digitalWrite(R_IN4,HIGH);

  analogWrite(R_ENB,Vs_R);
  analogWrite(L_ENA,Vs_L);
}
void advance_st(){
  digitalWrite(L_IN1,LOW);
  digitalWrite(L_IN2,HIGH);
  digitalWrite(R_IN3,LOW);
  digitalWrite(R_IN4,HIGH);  

if (L_dist < SC_THRESH)
  {
    if (L_v < Vcmax)
    {
      L_v+=5;
      R_v-=5;
    }else
    {
      L_v-=5;
      R_v-=15;
    }
  }else if (R_dist < SC_THRESH)
  {
    if(R_v < Vcmax)
    {
      R_v+=5;
      L_v-=5;
    }else
    {
      R_v-=5;
      L_v-=15;
    }
  }

  L_v = max(min(L_v, Vcmax), VCmin);
  R_v = max(min(R_v, Vcmax), VCmin);

  

  Serial.print(" | R-v = ");
  Serial.print(R_v);
  Serial.print(" <> L-V = ");
  Serial.print(L_v);

  analogWrite(R_ENB,R_v);
  analogWrite(L_ENA,L_v);
}
void turn_left(){
  digitalWrite(L_IN1,LOW);
  digitalWrite(L_IN2,LOW);
  digitalWrite(R_IN3,LOW);
  digitalWrite(R_IN4,HIGH);

  analogWrite(R_ENB,Vs_R);
  analogWrite(L_ENA,Vs_L);
}
void turn_right(){
  digitalWrite(L_IN1,LOW);
  digitalWrite(L_IN2,HIGH);
  digitalWrite(R_IN3,LOW);
  digitalWrite(R_IN4,LOW);

  analogWrite(R_ENB,Vs_R);
  analogWrite(L_ENA,Vs_L);
}
void u_turn(){
  digitalWrite(L_IN1,HIGH);
  digitalWrite(L_IN2,LOW);
  digitalWrite(R_IN3,LOW);
  digitalWrite(R_IN4,HIGH);
  analogWrite(R_ENB,Vs_R);
  analogWrite(L_ENA,Vs_L);
}
void loop() {
  R_dist = R_hc.dist();
  L_dist = L_hc.dist();
  F_dist = F_hc.dist();

  Serial.print("\nRight = ");
  Serial.print(R_dist);
  Serial.print(" | Front = ");
  Serial.print(F_dist);
  Serial.print(" | Left = ");
  Serial.print(L_dist);

  if(F_dist >= F_THRESH){
    if(L_dist >= S_THRESH){
      stop(1000);
      turn_left();
      delay(560);
      Serial.print(" | Left");
      advance();
      delay(350);
    }
    advance_st();
    Serial.print(" | Advance");
  }else if(F_dist < F_THRESH && L_dist >= S_THRESH){
    stop(1000);
    turn_left();
    delay(560);
    Serial.print(" | Left");
    advance();
    delay(350);  
  }else if(F_dist < F_THRESH && R_dist >= S_THRESH){
    stop(1000);
    turn_right();
    delay(558);
    Serial.print(" | Right");
    advance();
    delay(350);
  }else if (F_dist < F_THRESH && R_dist < S_THRESH && L_dist < S_THRESH) {
    stop(1000);
    u_turn();
    delay(560);
    Serial.print(" | U-Turn");
    advance();
    delay(350);
  }
}


