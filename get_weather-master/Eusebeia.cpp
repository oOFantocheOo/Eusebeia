#include <Arduino.h>
#include <ArduinoJson.h>
#include <Ethernet.h>
#include <SPI.h>
#include <Servo.h>
#include <SoftwareSerial.h>

#define MAX_STRING_LEN 512
#define MS_PER_SEC 1000
#define MS_PER_MIN 60 * MS_PER_SEC

#define fanPin 8
#define servoPin 10
#define liquidPin 0
#define pressurePin 1

#define PEOPLE_COUNT 2
#define weightPin 2
#define EPS 3.0

#define petWeight 10
#define hummenWeight 60

void SHUTDOWN() {
  while (true) {
    ;
  }
}

// Web client part
byte MAC[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
char WEATHER_SERVER[] = "api.seniverse.com";

EthernetClient HTTPClient;

void setUpHTTPClient() {
  Serial.println("Connecting to Internet...");

  if (Ethernet.begin(MAC) == 0) {
    Serial.println("Connection via DHCP failed");
    SHUTDOWN();
  }
}

void sendHTTPRequest() {
  HTTPClient.println(
      "GET "
      "/v3/weather/now.json?key=8mimj0qv7oqo5hkb&location=shanghai&language=en "
      "HTTP/1.1");
  HTTPClient.println("Host: api.seniverse.com");
  HTTPClient.println("Connection: close");
  HTTPClient.println();
}

void skipHTTPHeaders() {
  char endFlag[] = "\r\n\r\n";
  if (!HTTPClient.find(endFlag)) {
    Serial.println("Invailid respinse");
    SHUTDOWN();
  }
}

bool isRainyDay() {
  if (!HTTPClient.connect(WEATHER_SERVER, 80)) {
    Serial.println("Connection failed");
    SHUTDOWN();
  }

  sendHTTPRequest();
  skipHTTPHeaders();

  char json[MAX_STRING_LEN];
  HTTPClient.readBytesUntil(char(-1), json, sizeof(json));

  StaticJsonBuffer<MAX_STRING_LEN> jsonBuffer;

  JsonObject &root = jsonBuffer.parseObject(json);
  if (!root.success()) {
    Serial.println("Deparse JSON failed");
    SHUTDOWN();
  }
  int code = root["results"][0]["now"]["code"].as<int>();
  return (10 <= code && code <= 25);
}

// Servo part
Servo servo;

void setUpServo() {
  servo.attach(servoPin);
  pinMode(fanPin, OUTPUT);
  pinMode(servoPin, OUTPUT);
  servo.write(0);
}

bool umbrellaDetected() { return analogRead(pressurePin) >= 200; }

bool liquidDetected() {
  bool flag = true;
  for (int i = 0; i < 10 && flag; ++i) {
    if (analogRead(liquidPin) > 1000) {
      flag = false;
    }
  }
  return flag;
}

void rotateServo() {
  for (int pos = 0; pos < 45; ++pos) {
    servo.write(pos);
  }
  delay(MS_PER_SEC * 5);
  for (int pos = 45; pos > 0; --pos) {
    servo.write(pos);
  }
  delay(MS_PER_SEC);
}

// Bluetooth part
SoftwareSerial bluetooth(6, 7);

void setUpBluetooth() { bluetooth.begin(9600); }

bool isPerson() {
  const int weight = analogRead(weightPin);
  if (abs(weight - hummenWeight) < EPS) {
    return true;
  } else if (abs(weight - petWeight) < EPS) {
    return false;
  } else {
    return false;
  }
}

// Core
void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  setUpHTTPClient();
  setUpServo();
  setUpBluetooth();
}

void loop() {
  if (umbrellaDetected()) {
    if (liquidDetected()) {
      digitalWrite(fanPin, HIGH);
      delay(MS_PER_SEC * 5);
    } else {
      digitalWrite(fanPin, LOW);
    }
  }

  bluetooth.write(isPerson() ? "\1" : "\0");

  if (umbrellaDetected() && isPerson() && isRainyDay()) {
    rotateServo();
  }
}
