#include "ArduinoJson.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

const char *read_json(const char *weather_json) {
  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject &json_root = jsonBuffer.parseObject(weather_json);

  if (!json_root.success()) {
    throw; // Description failed
  }

  const char *res = json_root["results"][0]["now"]["text"];
  printf("JSON is: %s\n", res);
  return res;
}

int main() {
  ifstream fin("weather_example.json");
  string json;
  while (fin) {
    string temp;
    getline(fin, temp);
    json += temp;
  }

  const char *res = read_json(json.c_str());
  cout << res << endl;
  return 0;
}
