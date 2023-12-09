#include <HCSR04.h>

typedef UltraSonicDistanceSensor HCSR04;

//SENSOR PINS
float C_THRESH = 6; 
float S_THRESH = 10;
float ERR_TRESH = 2;

float R_dist;
float L_dist;

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


void advance(float R_dist, float L_dist)
{
  if((R_dist - L_dist) > ERR_TRESH)
  {
  R_v = (float)Vmax*((float)L_dist/(L_dist + R_dist));
  L_v = (float)Vmax*((float)R_dist/(L_dist + R_dist));

  Serial.println("sadboy");  
  }else
    R_v = L_v = 255;

  Serial.println("straight");  
}

void setup()
{ 
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

  if(L_dist <= S_THRESH)
  {
    advance(R_dist, L_dist);
  }
  analogWrite(R_EN, R_v);
  analogWrite(L_EN, L_v);


  // Serial.print(R_v);
  // Serial.print("  ");
  // Serial.println(L_v); 
  
}



