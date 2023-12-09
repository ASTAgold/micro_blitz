#include <HCSR04>

//SENSOR PINS
float C_THRESH = 6; 
float S_THRESH = 13;  

int R_ECHO = 0;
int R_TRIG = 1;
HCSR04 R_hc(R_TRIG, R_ECHO);

int L_ECHO = 2;
int L_TRIG = 3;
HCSR04 L_hc(L_TRIG, L_ECHO);

int C_ECHO = 4;
int C_TRIG = 5;
HCSR04 C_hc(C_TRIG, C_ECHO);

// MOTOR PINS
int R_EN = 6; 
int R_IN1 = 7;
int R_IN2 = 8;

int L_EN = 9;
int L_IN1 = 10;
int L_IN2 = 11;

void setup()
{ 
  pinMode(L_EN, OUTPUT);
  pinMode(L_IN1, OUTPUT);
  pinMode(L_IN2, OUTPUT);

  digitalWrite(R_IN1, LOW);
  digitalWrite(R_IN2, LOW);
  digitalWrite(L_IN1, LOW);
  digitalWrite(L_IN2, LOW);

}

void loop()
{

}




