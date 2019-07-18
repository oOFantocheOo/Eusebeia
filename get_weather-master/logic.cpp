#include <Arduino.h>
#include <Servo.h>

Servo myservo;
int fanPin = 8;
int servoPin = 10;
int liquidPin = 0;
int pressurePin = 1;

bool umbrellaDetected() {
  if (analogRead(pressurePin) < 200) {
    return false;
  }
  return true;
}

bool liquidDetected() {
  for (int i = 0; i < 10; ++i) {
    if (analogRead(liquidPin) > 1000) {
      return false;
    }
  }
  return true;
}

void rotateServo() {
  for (int pos = 0; pos < 90; ++pos) {
    myservo.write(pos);
  }
  delay(5000);
  for (int pos = 90; pos > 0; --pos) {
    myservo.write(pos);
  }
  delay(1000);
}

void setup() {
  myservo.attach(servoPin);
  pinMode(fanPin, OUTPUT);
  pinMode(servoPin, OUTPUT);
  Serial.begin(9600);
  myservo.write(0);
}

void loop() {
  Serial.println(analogRead(pressurePin));
  if (umbrellaDetected()) {
    if (liquidDetected()) {
      digitalWrite(fanPin, HIGH);
      delay(500);
    } else {
      digitalWrite(fanPin, LOW);
      rotateServo();
    }
  }
}
