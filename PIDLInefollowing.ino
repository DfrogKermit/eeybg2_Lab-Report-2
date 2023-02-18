//*************************************//
// BEATRIZ GAVINA                     //
// UNI OF NOTTINGHAM EEE             //
// 20446616                         //
//*********************************//

#include <Wire.h>
#define I2C_SLAVE_ADDR 0x04 // 4 in hexadecimal

// PINS FOR EACH IR SENSOR
#define SENSOR1 33 // Sensor 1 pin
#define SENSOR2 35 // Sensor 2 pin

// PID constants
#define KP 0.1 // Proportional constant
#define KD 0.2 // Derivative constant
#define KI 0.0 // Integral constant
#define MAX_SPEED 255 // Maximum speed for motors

// Target position for the line
#define TARGET_POS 1500

// Thresholds for detecting black/white surface
#define SENSOR1_THRESHOLD 3500
#define SENSOR2_THRESHOLD 3500

// Variables for PID calculation
int last_error = 0;
int integral = 0;

void setup()
{
  Serial.begin(9600);
  Wire.begin();   // Join i2c bus (address optional for the master) - on the Arduino NANO the default I2C pins are A4 (SDA), A5 (SCL)
  pinMode(SENSOR1, INPUT);
  pinMode(SENSOR2, INPUT);
}

// Function to read sensor data and calculate error
int readSensors() {
  int sensor1_value = analogRead(SENSOR1);
  int sensor2_value = analogRead(SENSOR2);
  
  int error = sensor1_value - sensor2_value;
  
  return error;
}

// Function to calculate motor speeds using PID
void pidControl(int error) {
  int derivative = error - last_error;
  last_error = error;
  integral += error;

  int motor_speed = KP * error + KD * derivative + KI * integral;
  
  int left_speed = constrain(MAX_SPEED - motor_speed, 0, MAX_SPEED);
  int right_speed = constrain(MAX_SPEED + motor_speed, 0, MAX_SPEED);

  Wire.beginTransmission(I2C_SLAVE_ADDR);
  Wire.write((byte)((left_speed & 0x0000FF00) >> 8));
  Wire.write((byte)(left_speed & 0x000000FF));
  Wire.write((byte)((right_speed & 0x0000FF00) >> 8));
  Wire.write((byte)(right_speed & 0x000000FF)); 
  Wire.write((byte)((90 & 0x0000FF00) >> 8));
  Wire.write((byte)(90 & 0x000000FF));
  Wire.endTransmission();
}

void loop() {
  int error = readSensors();

  if (abs(error) < SENSOR1_THRESHOLD || abs(error) < SENSOR2_THRESHOLD) {
    // If the error is small, go straight
    pidControl(0);
  } else if (error > 0) {
    // If the error is positive, turn right
    pidControl(error);
  } else {
    // If the error is negative, turn left
    pidControl(error);
  }
  
  delay(10);
}
