#include <MPU6050.h>

#include <HCSR04.h>

float F_THRESH = 10; 
float S_THRESH = 10;


int Vmax =  140;
int OUT_V = Vmax;
int IN_V = 80;


float R_dist;
float L_dist;
float F_dist;

int R_ECHO = 4;
int R_TRIG = 5;
HCSR04 R_hc(R_TRIG, R_ECHO);

int L_ECHO = 2;
int L_TRIG = 3;
HCSR04 L_hc(L_TRIG, L_ECHO);

int F_ECHO  = 6;
int F_TRIG  = 7;
HCSR04 F_hc(F_TRIG, F_ECHO);

int R_EN = 9; 
int R_IN1 = 8;
int R_IN2 = 10;
int R_v = 0;

int L_EN = 11;
int L_IN1 = 12;
int L_IN2 = 13;
int L_v = 0;



void advance()
{
if (L_dist < S_THRESH && L_v<180){
     L_v+=5;
     R_v-=5;
}
else if (L_v>=180){
    L_v-=5;
    R_v-=15;
}

if (R_dist < S_THRESH && R_v<180){
    R_v+=5;
    L_v-=5;
}
else if (R_v>=180){
    R_v-=5;
    L_v-=15;
}
}
void turn_right()
{     
   L_v = OUT_V;
   R_v = IN_V;
}
void turn_left()
{
   R_v = OUT_V;
   L_v = IN_V;
}
void U_turn()
{
  digitalWrite(R_IN1, LOW);
  digitalWrite(R_IN2, HIGH);
  digitalWrite(L_IN1, HIGH);
  digitalWrite(L_IN2, LOW);
      
  R_v = IN_V;
  L_v = IN_V;
}
void setup()
{ 

  Serial.begin(9600);

  pinMode(L_EN,OUTPUT);
  pinMode(L_IN1,OUTPUT);
  pinMode(L_IN2,OUTPUT);
  pinMode(R_EN,OUTPUT);
  pinMode(R_IN1,OUTPUT);
  pinMode(R_IN2,OUTPUT);
}


void loop()
{
  digitalWrite(R_IN1, LOW);
  digitalWrite(R_IN2, HIGH);
  digitalWrite(L_IN2, LOW);
  digitalWrite(L_IN1, HIGH);

  R_dist = R_hc.dist();
  //R_dist = R_dist >= 0? R_dist : 0;
  L_dist = L_hc.dist();
  //L_dist = L_dist >= 0? L_dist : 0;
  F_dist = F_hc.dist();
  //F_dist = F_dist >= 0? F_dist : 0;

    if(L_dist > S_THRESH)
      turn_left();
    else if(F_dist > F_THRESH)
      advance();
    else if(R_dist > S_THRESH)
      turn_right();
    else
      U_turn();

  analogWrite(R_EN, R_v);
  analogWrite(L_EN, L_v);
  
}



