#include <HCSR04.h>

//SENSOR P  INS
float F_THRESH = 12; 
// TO DO
float S_THRESH = 9;
float ERR_TRESH = 2;

// MOTOR PINS
int Vmax =  100;

int OUT_V = Vmax;
int IN_V = 60;

int Vs = {60, 80, 100};

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


float ease(float x)
{
//  return  x < 0.5 ? 2 * x * x : 1 - (-2 * x + 2)*(-2 * x + 2) / 2;
  return sin((x * PI) / 2); 
}

void advance()
{
  //stabilize
  R_v = Vs[floor(L_dist/(L_dist + R_dist) * (n-1))];
  L_v = Vs[floor(R_dist/(L_dist + R_dist) * (n-1))];
}
void turn_right()
{
 // while((R_dist > S_THRESH || F_dist < F_THRESH))
 // {
    L_v = OUT_V;
    R_v = IN_V;

    analogWrite(R_EN, R_v);
    analogWrite(L_EN, L_v);
 // }
}
void turn_left()
{
    //while((L_dist > S_THRESH && F_dist < F_THRESH))
    //{
      R_v = OUT_V;
      L_v = IN_V;

      analogWrite(R_EN, R_v);
      analogWrite(L_EN, L_v);
   // }
}
void U_turn(){
   // while((F_dist < F_THRESH))
  //  {
      digitalWrite(R_IN1, LOW);
      digitalWrite(R_IN2, HIGH);
      digitalWrite(L_IN1, HIGH);
      digitalWrite(L_IN2, LOW);
      
      R_v = IN_V;
      L_v = IN_V;

      analogWrite(R_EN, R_v);
      analogWrite(L_EN, L_v);
  //  }
}
void setup()
{ 
  // state = advancing;

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

  
  Serial.print(L_dist);
  Serial.print("   ");
  Serial.print(F_dist);
  Serial.print("   ");
  Serial.print(R_dist);
  Serial.print("            ");
  Serial.print(R_v);
  Serial.print("   ");
  Serial.println(L_v);  

  advance();
  
  analogWrite(R_EN, R_v);
  analogWrite(L_EN, L_v);
}



