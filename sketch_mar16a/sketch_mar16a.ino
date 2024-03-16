int R_EN = 9; 
int R_IN1 = 8;
int R_IN2 = 10;

int L_EN = 11;
int L_IN1 = 12;
int L_IN2 = 13;

int v1 = 50;
int v2 = 50;

int button1 = 0;
int button2 = 0;


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  pinMode(button1, INPUT);
  pinMode(button2, INPUT);

  digitalWrite(R_IN1, LOW);
  digitalWrite(R_IN2, LOW);
  digitalWrite(L_IN2, LOW);
  digitalWrite(L_IN1, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (digitalRead(button1) == HIGH)
  {
    v1 += 5;

    analogWrite(R_EN, v1);
    analogWrite(L_EN, v2); 

    Serial.println("R_UP"); 

    delay(200);
  }else if(digitalRead(button2) == HIGH)
  {
    v2 += 5;

    analogWrite(R_EN, v1);
    analogWrite(L_EN, v2);  

    Serial.println("L_UP"); 

    delay(200);
  }

  digitalWrite(R_IN1, LOW);
  digitalWrite(R_IN2, HIGH);
  digitalWrite(L_IN2, LOW);
  digitalWrite(L_IN1, HIGH);
}
