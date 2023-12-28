/* ------------------------------------------------- */

#include "ESPTelnetStream.h"

/* ------------------------------------------------- */

#define SERIAL_SPEED  9600
#define INFRA_SSID    "MY SSID"
#define INFRA_PSWD    "MY PASS"

/* ------------------------------------------------- */

ESPTelnetStream telnet;

/* ------------------------------------------------- */

void telnetConnected(String ip) {
  Serial.print(ip);
  Serial.println(" connected.");
}

void telnetDisconnected(String ip) {
  Serial.print(ip);
  Serial.println(" disconnected.");
}

void telnetReconnect(String ip) {
  Serial.print(ip);
  Serial.println(" reconnected.");
}

/* ------------------------------------------------- */

bool connectToWiFi(const char* ssid, const char* password, int max_tries = 20, int pause = 500) {
  int i = 0;
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  #if defined(ARDUINO_ARCH_ESP8266)
    WiFi.forceSleepWake();
    delay(200);
  #endif
  WiFi.begin(ssid, password);
  do {
    delay(pause);
    Serial.print(".");
    i++;
  } while (!isConnected() && i < max_tries);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  return isConnected();
}

/* ------------------------------------------------- */

void setup() {
  Serial.begin(SERIAL_SPEED);
  Serial.println("ESP Telnet Test");

  connectToWiFi(WIFI_SSID, WIFI_PASSWORD);
  
  if (isConnected()) {
    ip = WiFi.localIP();
    Serial.println();
    Serial.print("- Telnet: "); Serial.print(ip); Serial.print(":"); Serial.println(port);
    setupTelnet();
  } else {
    Serial.println();    
    errorMsg("Error connecting to WiFi");
  }
}

/* ------------------------------------------------- */

void loop() {
  telnet.loop();
  if(Serial.available() > 0) {
    telnet.write(Serial.read());
  }
  if (telnet.available() > 0) {    
    Serial.print(char(telnet.read()));
  }
}

/* ------------------------------------------------- */
