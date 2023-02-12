#include <WiFi.h>
#include <Wire.h>
#define green 35 
#define pink 26
#define purple 32
#define yellow 34
#define blue 25
#define white 33

//3 pairs of sensors for each side
const int leftSensorsPins[] = {26, 32, 25};
const int rightSensorsPins[] = {35, 34, 33};
const int leftMotorPin = 14;
const int rightMotorPin = 5;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  for (int i = 0; i < 3; i++) {
    pinMode(leftSensorsPins[i], INPUT);
    pinMode(rightSensorsPins[i], INPUT);
  }
  pinMode(leftMotorPin, OUTPUT);
  pinMode(rightMotorPin, OUTPUT);
}

void loop() {
  int weightedAverage = 0;
  for (int i = 0; i < 3; i++) {
    int leftSensorsValue = digitalRead(leftSensorsPins[i]);
    int rightSensorsValue = digitalRead(rightSensorsPins[i]);
    // Use the weighted average formula to calculate the direction of the line
    weightedAverage += (leftSensorsValue * -1 + rightSensorsValue * 1) / 2;
  }
  weightedAverage /= 3;
  int motorCommand = 0;

  if (weightedAverage == 0) {
    // Line is in the center, move forward
    Serial.println("Move Forward");
    motorCommand = 1;
  } else if (weightedAverage > 0) {
    // Line is on the right side, steer left
    Serial.println("Steer Left");
    motorCommand = 2;
  } else if (weightedAverage < 0) {
    // Line is on the left side, steer right
    Serial.println("Steer Right");
    motorCommand = 3;
  }
  Wire.beginTransmission(8);
  Wire.write(motorCommand);
  Wire.endTransmission();
}