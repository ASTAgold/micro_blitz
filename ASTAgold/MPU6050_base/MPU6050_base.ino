
#include "Wire.h"


#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

#define LED_PIN 13
bool blinkState = false;

void setup() {
    // INITIALIZATION
    Wire.begin();

    Serial.begin(38400);

    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // PIN CONFIG
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    // read raw accel/gyro data
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);



    // print accel/gyro data
    Serial.print(ax); Serial.print("\t");
    Serial.print(ay); Serial.print("\t");
    Serial.print(az); Serial.print("\t");
    Serial.print(gx); Serial.print("\t");
    Serial.print(gy); Serial.print("\t");
    Serial.println(gz);

    // blink LED
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
}
