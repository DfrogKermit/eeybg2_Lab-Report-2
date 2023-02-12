#include <PID.h>

#include <Wire.h>
#include <PID_v1.h>

const int leftSensorsPins[] = {26, 32, 25};
const int rightSensorsPins[] = {35, 34, 33};
double setpoint = 0.5; // Setpoint for the center of the line
double input, output;

PID linePID(&input, &output, &setpoint, 2, 0.1, 0.1, DIRECT);

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
   for (int i = 0; i < 3; i++) {
  pinMode(leftSensorsPins[i], INPUT);
  pinMode(rightSensorsPins[i], INPUT);
   }  
  linePID.SetMode(AUTOMATIC);
}

void loop() {
   for (int i = 0; i < 3; i++) {
  input = (analogRead(leftSensorsPins[i]) / 1024.0) - (analogRead(rightSensorsPins[i]) / 1024.0);
  linePID.Compute();
   }

  int motorCommand = 0;
  if (output > 0) {
    // Line is on the left side, steer right
    motorCommand = 1;
  } else if (output < 0) {
    // Line is on the right side, steer left
    motorCommand = 2;
  } else {
    // Line is in the center, move forward
    motorCommand = 3;
  }
  
  Wire.beginTransmission(8);
  Wire.write(motorCommand);
  Wire.endTransmission();
}
