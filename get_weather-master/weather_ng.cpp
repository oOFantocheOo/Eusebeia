#include <Arduino.h>
#include <ArduinoJson.h>
#include <Ethernet.h>
#include <SPI.h>

#define MAX_STRING_LEN 512
#define MS_PER_MIN 60000

void SHUTDOWN() {
  while (true) {
    ;
  }
}

int count = 0;

byte MAC[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
char WEATHER_SERVER[] = "api.seniverse.com";

EthernetClient client;

void sendHTTPRequest() {
  client.println(
      "GET "
      "/v3/weather/now.json?key=8mimj0qv7oqo5hkb&location=shanghai&language=en "
      "HTTP/1.1");
  client.println("Host: api.seniverse.com");
  client.println("Connection: close");
  client.println();
}

void skipHTTPHeaders() {
  char endFlag[] = "\r\n\r\n";
  if (!client.find(endFlag)) {
    Serial.println("Invailid respinse");
    SHUTDOWN();
  }
}

bool isRainyDay() {
  if (!client.connect(WEATHER_SERVER, 80)) {
    Serial.println("Connection failed");
    SHUTDOWN();
  }

  sendHTTPRequest();
  skipHTTPHeaders();

  char json[MAX_STRING_LEN];
  client.readBytesUntil(char(-1), json, sizeof(json));

  StaticJsonBuffer<MAX_STRING_LEN> jsonBuffer;

  JsonObject &root = jsonBuffer.parseObject(json);
  if (!root.success()) {
    Serial.println("Deparse JSON failed");
    SHUTDOWN();
  }
  int code = root["results"][0]["now"]["code"].as<int>();
  return 10 <= code && code <= 25;
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  Serial.println("Connecting to Internet...");

  if (Ethernet.begin(MAC) == 0) {
    Serial.println("Connection via DHCP failed");
    SHUTDOWN();
  }
}

void loop() {
  if (isRainyDay()) {
    Serial.println("Rainy");
  } else {
    Serial.println("No");
  }
  delay(MS_PER_MIN * 15);
}
