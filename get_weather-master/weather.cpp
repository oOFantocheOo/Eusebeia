#include <Arduino.h>
#include <Ethernet.h>
#include <SPI.h>

#define DEBUG

#define MAX_STRING_LEN 250
#define MS_PER_MIN 60000

int count = 0;

byte MAC[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
char WEATHER_SERVER[] = "api.seniverse.com";

EthernetClient client;

void serialEvent() {
  bool flag = false;
  char inChar = client.read();
  if (flag && inChar != '"') {
    Serial.print(inChar);
  }
  if (flag && inChar == '"')
    flag = false;

  if (!flag && inChar == 't')
    if (client.read() == 'e')
      if (client.read() == 'x')
        if (client.read() == 't') {
          flag = true;
          for (int i = 0; i < 3; i++) {
            client.read();
          }
        }
}

void makeHTTPRequest() {
  client.println(
      "GET /v3/weather/now.json?key=8mimj0qv7oqo5hkb&location=ip&language=en "
      "HTTP/1.1");
  client.println("Host: api.seniverse.com");
  client.println("Connection: close");
  client.println();
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  Serial.println("Connecting to Internet...");
  if (Ethernet.begin(MAC) == 0) {
    Serial.println("Connection via DHCP failed");
    while (true) {
      ;
    }
  }
  if (!client.connect(WEATHER_SERVER, 80)) {
    println("Connection failed");
    while (true) {
      ;
    }
  }
}

void loop() {
  makeHTTPRequest();

  if (client.available()) {
    serialEvent();
    for (size_t i = 0; i != 3; ++i) {
      Serial.print(char(client.read()));
    }
    Serial.println();
  } else {
    client.stop();
    while (true) {
      ;
    }
  }

#ifdef DEBUG
  delay(10 * 1000); // Sleep 10 sec
#else
  delay(MS_PER_MIN * 15);
#endif
}
