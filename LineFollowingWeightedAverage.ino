#include <WiFi.h>
#include <Wire.h>
#define I2C_SLAVE_ADDR 0x04 

// THESE WILL BE THE 2 USED SENSORS 
#define SENSOR_LEFT 25 
#define SENSOR_RIGHT 33

#define SENSOR_LEFT_WEIGHT 37
#define SENSOR_RIGHT_WEIGHT 20
//THRESHOLD OF THE RANGE OF THE SENSORS
#define SENSOR_THRESHOLD 3500


void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(SENSOR_LEFT,INPUT);
  pinMode(SENSOR_RIGHT,INPUT);
}
/*This function is in charge of thE movemnet of the car, including data being 
sent to the Arduino to adjust thE motor speeds and the servo angle. */
void moveCar(int servoAngleint, leftMotor_speed, int rightMotor_speed) {
  Wire.beginTransmission(I2C_SLAVE_ADDR); 
  Wire.write((byte)((leftMotor_speed & 0x0000FF00) >> 8));
  Wire.write((byte)(leftMotor_speed & 0x000000FF));
  Wire.write((byte)((rightMotor_speed & 0x0000FF00) >> 8));
  Wire.write((byte)(rightMotor_speed & 0x000000FF)); 
  Wire.endTransmission();
  delay(200);
}
//Here the use of thw weighted average formula is seen.
void loop() {
  int sensorLeftValue = analogRead(SENSOR_LEFT);
  int sensorRightValue = analogRead(SENSOR_RIGHT);

  double error = 0;
  if (sensorLeftValue < SENSOR_THRESHOLD && sensorRightValue < SENSOR_THRESHOLD) {
    error = 0;
  } else if (sensorLeftValue < SENSOR_THRESHOLD) {
    error = -1;
  } else if (sensorRightValue < SENSOR_THRESHOLD) {
    error = 1;
  } else {
    error = ((double)sensorLeftValue / (double)sensorRightValue) - 1;
  }

  int leftMotorSpeed = 0;
  int rightMotorSpeed = 0;

  if (error == 0) {
    leftMotorSpeed = 120;
    rightMotorSpeed = 100;
  } else {
    leftMotorSpeed = 120 + (error * 10);
    rightMotorSpeed = 100 - (error * 10);
  }

  moveCar(leftMotorSpeed, rightMotorSpeed);

  delay(50);
}
