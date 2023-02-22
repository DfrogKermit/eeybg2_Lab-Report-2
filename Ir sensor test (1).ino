// Pins that connect the pair of sensors 
#define green 34   
#define pink 35
#define white 32
#define yellow 33
#define blue 25
#define purple 26

void setup() {
  // put your setup code here, to run once:
  // define the pins we want to read
  // the intials attached correlete to the colour
  pinMode(green,INPUT);
  pinMode(pink,INPUT);
  pinMode(white,INPUT);
  pinMode(yellow,INPUT);
  pinMode(blue,INPUT);
  pinMode(purple,INPUT);
  Serial.begin(9600);
}

//this loop relates the reading of values and outputs it 
void loop() {
  Serial.print(analogRead(green));
  Serial.print(",");
  Serial.print(analogRead(pink));
  Serial.print(",");
  Serial.print(analogRead(white));
  Serial.print(",");
  Serial.print(analogRead(yellow));
  Serial.print(",");
  Serial.print(analogRead(blue));
  Serial.print(",");
  Serial.print(analogRead(purple));
  Serial.println();

  delay(20);
}
