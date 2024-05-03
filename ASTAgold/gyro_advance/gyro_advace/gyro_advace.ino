#include <Wire.h>
#include <MPU6050.h>

#include <HCSR04.h>

int Vmax =  120;
int R_v = Vmax;
int L_v = Vmax;

// MOTOR PINS

int R_EN = 9; 
int R_IN1 = 8;
int R_IN2 = 10;

int L_EN = 11;
int L_IN1 = 12;
int L_IN2 = 13;
analogue_write(map(R_v,0,255,50,120));
// ultrasons PINS
[0,255] -> [50,120]
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

// gyroscope 

float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float roll, pitch, yaw, ref_roll;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
float elapsedTime, currentTime, previousTime;

int A_THRESH = 10;

void advance()
{
  ang_diff = roll - ref_roll;
  if( abs(ang_diff) > A_ANG_THRESH ||  (R_dist > A_DIST_THRESH ||  L_dist > A_DIST_THRESH) )
  {
    // angle correction
    float ang_error = ang_diff - ref_roll;
    int L_v = L_v + ang_kp * error;
    int R_v = R_v - ang_kp * error;

    // distance correction
    float dist_error = R_dist - distanceLeft;
    int L_v = L_v + dist_kp * error;
    int R_v = R_v - dist_kp * error;

    // TODO : make it non lineare
    float error = 128 - L_v;
    int L_v = L_v + v_kp * error;

    float error = 128 - R_v;
    int R_v = R_v + v_kp * error;

  
    R_v = constrain(R_v,0,255);
    L_v = constrain(L_v,0,255);
  }
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

  calc_angle();

  advance();

  analogue_write(map(R_v,0,255,50,120), R_EN);
  analogue_write(map(L_v,0,255,50,120), L_EN);
}


void calc_angle()
{

}








/*
  Arduino and MPU6050 Accelerometer and Gyroscope Sensor Tutorial
  by Dejan, https://howtomechatronics.com
*/
#include <Wire.h>
const int MPU = 0x68; // MPU6050 I2C address
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float roll, pitch, yaw;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
float elapsedTime, currentTime, previousTime;
int c = 0;
void setup() {
 Serial.begin(19200);
 Wire.begin();                      // Initialize comunication
 Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
 Wire.write(0x6B);                  // Talk to the register 6B
 Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
 Wire.endTransmission(true);        //end the transmission
 /*
 // Configure Accelerometer Sensitivity - Full Scale Range (default +/- 2g)
 Wire.beginTransmission(MPU);
 Wire.write(0x1C);                  //Talk to the ACCEL_CONFIG register (1C hex)
 Wire.write(0x10);                  //Set the register bits as 00010000 (+/- 8g full scale range)
 Wire.endTransmission(true);
 // Configure Gyro Sensitivity - Full Scale Range (default +/- 250deg/s)
 Wire.beginTransmission(MPU);
 Wire.write(0x1B);                   // Talk to the GYRO_CONFIG register (1B hex)
 Wire.write(0x10);                   // Set the register bits as 00010000 (1000deg/s full scale)
 Wire.endTransmission(true);
 delay(20);
 */
 // Call this function if you need to get the IMU error values for your module
 calculate_IMU_error();
 delay(20);
}
void loop() {
 // === Read acceleromter data === //
 Wire.beginTransmission(MPU);
 Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
 Wire.endTransmission(false);
 Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
 //For a range of +-2g, we need to divide the raw values by 16384, according to the datasheet
 AccX = (Wire.read() << 8 | Wire.read()) / 16384.0; // X-axis value
   = (Wire.read() << 8 | Wire.read()) / 16384.0; // Y-axis value
 AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0; // Z-axis value
 // Calculating Roll and Pitch from the accelerometer data
 accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI) - AccErrorX ; // AccErrorX ~(0.58) See the calculate_IMU_error()custom function for more details
 accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) - AccErrorY ; // AccErrorY ~(-1.58)
 // === Read gyroscope data === //
 previousTime = currentTime;        // Previous time is stored before the actual time read
 currentTime = millis();            // Current time actual time read
 elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds
 Wire.beginTransmission(MPU);
 Wire.write(0x43); // Gyro data first register address 0x43
 Wire.endTransmission(false);
 Wire.requestFrom(MPU, 6, true); // Read 4 registers total, each axis value is stored in 2 registers
 GyroX = (Wire.read() << 8 | Wire.read()) / 131.0; // For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
 GyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
 GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;
 // Correct the outputs with the calculated error values
 GyroX = GyroX - GyroErrorX ; // GyroErrorX ~(-0.56)
 GyroY = GyroY - GyroErrorY ; // GyroErrorY ~(2)
 GyroZ = GyroZ - GyroErrorZ ; // GyroErrorZ ~ (-0.8)
 // Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by sendonds (s) to get the angle in degrees
 gyroAngleX = gyroAngleX + GyroX * elapsedTime; // deg/s * s = deg
 gyroAngleY = gyroAngleY + GyroY * elapsedTime;
 yaw =  yaw + GyroZ * elapsedTime;
 // Complementary filter - combine acceleromter and gyro angle values
 roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
 pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;
 
 // Print the values on the serial monitor
 Serial.print(roll);
 Serial.print("/");
 Serial.print(pitch);
 Serial.print("/");
 Serial.println(yaw);
}
void calculate_IMU_error() {
 // We can call this funtion in the setup section to calculate the accelerometer and gyro data error. From here we will get the error values used in the above equations printed on the Serial Monitor.
 // Note that we should place the IMU flat in order to get the proper values, so that we then can the correct values
 // Read accelerometer values 200 times
 while (c < 200) {
   Wire.beginTransmission(MPU);
   Wire.write(0x3B);
   Wire.endTransmission(false);
   Wire.requestFrom(MPU, 6, true);
   AccX = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
   AccY = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
   AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
   // Sum all readings
   AccErrorX = AccErrorX + ((atan((AccY) / sqrt(pow((AccX), 2) + pow((AccZ), 2))) * 180 / PI));
   AccErrorY = AccErrorY + ((atan(-1 * (AccX) / sqrt(pow((AccY), 2) + pow((AccZ), 2))) * 180 / PI));
   c++;
 }
 //Divide the sum by 200 to get the error value
 AccErrorX = AccErrorX / 200;
 AccErrorY = AccErrorY / 200;
 c = 0;
 // Read gyro values 200 times
 while (c < 200) {
   Wire.beginTransmission(MPU);
   Wire.write(0x43);
   Wire.endTransmission(false);
   Wire.requestFrom(MPU, 6, true);
   GyroX = Wire.read() << 8 | Wire.read();
   GyroY = Wire.read() << 8 | Wire.read();
   GyroZ = Wire.read() << 8 | Wire.read();
   // Sum all readings
   GyroErrorX = GyroErrorX + (GyroX / 131.0);
   GyroErrorY = GyroErrorY + (GyroY / 131.0);
   GyroErrorZ = GyroErrorZ + (GyroZ / 131.0);
   c++;
 }
 //Divide the sum by 200 to get the error value
 GyroErrorX = GyroErrorX / 200;
 GyroErrorY = GyroErrorY / 200;
 GyroErrorZ = GyroErrorZ / 200;
 // Print the error values on the Serial Monitor
 Serial.print("AccErrorX: ");
 Serial.println(AccErrorX);
 Serial.print("AccErrorY: ");
 Serial.println(AccErrorY);
 Serial.print("GyroErrorX: ");
 Serial.println(GyroErrorX);
 Serial.print("GyroErrorY: ");
 Serial.println(GyroErrorY);
 Serial.print("GyroErrorZ: ");
 Serial.println(GyroErrorZ);
}


