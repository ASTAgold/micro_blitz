#include <HCSR04.h>

typedef UltraSonicDistanceSensor HCSR04;

//SENSOR PINS
float C_THRESH = 6; 
// TO DO
float S_THRESH = 11;
float ERR_TRESH = 2;

float OUT_V = 255;
float IN_V = 100;

float R_dist;
float L_dist;
float C_dist;

int R_ECHO = 4;
int R_TRIG = 5;
HCSR04 R_hc(R_TRIG, R_ECHO);

int L_ECHO = 2;
int L_TRIG = 3;
HCSR04 L_hc(L_TRIG, L_ECHO);

int C_ECHO  = 6;
int C_TRIG  = 7;
HCSR04 C_hc(C_TRIG, C_ECHO);

// MOTOR PINS
int Vmax = 255;

int R_EN = 9; 
int R_IN1 = 8;
int R_IN2 = 10;
int R_v = 0;

int L_EN = 11;
int L_IN1 = 12;
int L_IN2 = 13;
int L_v = 0;

enum states
{
  advancing,
  turning_left,
  turning_right,
  stopping
};
enum states state = advancing; 

void advance()
{
  //stabilize
  if((R_dist - L_dist) > ERR_TRESH)
  {
  R_v = (float)Vmax*((float)L_dist/(L_dist + R_dist));
  L_v = (float)Vmax*((float)R_dist/(L_dist + R_dist));

  Serial.println("sadboy");  
  }else
    R_v = L_v = 255;

  Serial.println("straight");  
}
void turn_left()
{
  if(R_dist > S_THRESH)
  {
    while(!(L_dist < S_THRESH && R_dist < S_THRESH))
    {
      R_v = OUT_V;
      L_v = IN_V;

      analogWrite(R_EN, R_v);
      analogWrite(L_EN, L_v);
    }
  }else
  {
  while(!(L_dist < S_THRESH))
    {
      R_v = OUT_V;
      L_v = IN_V;
    
      analogWrite(R_EN, R_v);
      analogWrite(L_EN, L_v);
    }
  }
}

void setup()
{ 
  state = advancing;

  Serial.begin(9600);

  pinMode(L_EN, OUTPUT);
  pinMode(L_IN1, OUTPUT);
  pinMode(L_IN2, OUTPUT);


}


void loop()
{
    
  digitalWrite(R_IN1, LOW);
  digitalWrite(R_IN2, HIGH);
  digitalWrite(L_IN1, LOW);
  digitalWrite(L_IN2, HIGH);

  R_dist = R_hc.measureDistanceCm();
  R_dist = R_dist >= 0? R_dist : 0;
  L_dist = L_hc.measureDistanceCm();
  L_dist = L_dist >= 0? L_dist : 0;
  C_dist = C_hc.measureDistanceCm();
  C_dist = C_dist >= 0? C_dist : 0;

  // PRIORIRY: LEFT > FORWARD > RIGHT
  state = (L_dist < S_THRESH) ? advancing : turning_left;

  if(state == advancing)
  {
    advance();
  }else if (state == turning_left)
  {
    turn_left();
  }

  analogWrite(R_EN, R_v);
  analogWrite(L_EN, L_v);


  // Serial.print(R_v);
  // Serial.print("  ");
  // Serial.println(L_v); 
  
}



