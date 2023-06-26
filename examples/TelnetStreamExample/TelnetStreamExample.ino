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

void setup() {
  Serial.begin(SERIAL_SPEED);
  Serial.println("ESP Telnet Test");

  WiFi.mode(WIFI_STA);
  WiFi.begin(INFRA_SSID, INFRA_PSWD);
  while(WiFi.status() != WL_CONNECTED) {
    delay(100);
  }

  telnet.onConnect(telnetConnected);
  telnet.onDisconnect(telnetDisconnected);
  telnet.onReconnect(telnetReconnect);

  Serial.print("Telnet.begin: ");
  if(telnet.begin()) {
    Serial.println("Successful");
  } else {
    Serial.println("Failed");
  }

  IPAddress ip = WiFi.localIP();
  Serial.println();
  Serial.print("Telnet Server IP: "); Serial.print(ip);

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
