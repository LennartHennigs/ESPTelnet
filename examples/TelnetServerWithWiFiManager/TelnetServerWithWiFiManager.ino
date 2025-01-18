/* ------------------------------------------------- */
// ESP8266 only - as WiFiManager is not working (yet) on ESP32
#ifdef ESP8266
/* ------------------------------------------------- */

#include <Arduino.h>
#include "ESPTelnet.h"
#include "WiFiManager.h"

/* ------------------------------------------------- */

#define PORTAL_TIMEOUT  10 * 60 // seconds
#define AP_NAME         "MY CAPTIVE PORTAL"
#define AP_PASSWORD     ""

#define SERIAL_SPEED    115200
#define WIFI_SSID       "MY SSID"
#define WIFI_PASSWORD   "MY PASS"

/* ------------------------------------------------- */

ESPTelnet telnet;
IPAddress ip;
WiFiManager wifiManager;

/* ------------------------------------------------- */

void setupSerial(long speed, String msg = "") {
  Serial.begin(speed);
  while (!Serial) {
  }
  delay(200);  
  Serial.println();
  Serial.println();
  if (msg != "") Serial.println(msg);
}

/* ------------------------------------------------- */

bool isConnected() {
  return (WiFi.status() == WL_CONNECTED);
}

/* ------------------------------------------------- */

void useWiFiManager() {
  // wifiManager.resetSettings();  // this will delete all credentials
  wifiManager.setDebugOutput(false);
  // wifiManager.resetSettings(); // wipe settings

  wifiManager.setConfigPortalTimeout(PORTAL_TIMEOUT);
  wifiManager.setAPCallback([] (WiFiManager *myWiFiManager) {
    Serial.println("- No known wifi found");
    Serial.print("- Starting AP: ");
    Serial.println(myWiFiManager->getConfigPortalSSID());
    Serial.println(WiFi.softAPIP());
  });
  // enable autoconnect
  if (!(String(AP_PASSWORD) == "" ? 
    wifiManager.autoConnect(AP_NAME) : 
    wifiManager.autoConnect(AP_NAME, AP_PASSWORD))
   ) {
    Serial.println("- Failed to connect and hit timeout");
    ESP.reset();
    delay(1000); 
  }
}

/* ------------------------------------------------- */

void errorMsg(String error, bool restart = true) {
  Serial.println(error);
  if (restart) {
    Serial.println("Rebooting now...");
    delay(2000);
    ESP.restart();
    delay(2000);
  }
}

/* ------------------------------------------------- */

void setupTelnet() {  
  // passing on functions for various telnet events
  telnet.onConnect(onTelnetConnect);
  telnet.onConnectionAttempt(onTelnetConnectionAttempt);
  telnet.onReconnect(onTelnetReconnect);
  telnet.onDisconnect(onTelnetDisconnect);
  
  // passing a lambda function
  telnet.onInputReceived([](String str) {
    // checks for a certain command
    if (str == "ping") {
      telnet.println("> pong");
      Serial.println("- Telnet: pong");
    }
  });

  Serial.print("- Telnet: ");
  if (telnet.begin()) {
    Serial.println("running");
  } else {
    Serial.println("error.");
    errorMsg("Will reboot...");
  }
}

/* ------------------------------------------------- */

// (optional) callback functions for telnet events
void onTelnetConnect(String ip) {
  Serial.print("- Telnet: ");
  Serial.print(ip);
  Serial.println(" connected");
  telnet.println("\nWelcome " + telnet.getIP());
  telnet.println("(Use ^] + q  to disconnect.)");
}

void onTelnetDisconnect(String ip) {
  Serial.print("- Telnet: ");
  Serial.print(ip);
  Serial.println(" disconnected");
  Serial.println();
}

void onTelnetReconnect(String ip) {
  Serial.print("- Telnet: ");
  Serial.print(ip);
  Serial.println(" reconnected");
}

void onTelnetConnectionAttempt(String ip) {
  Serial.print("- Telnet: ");
  Serial.print(ip);
  Serial.println(" tried to connected");
}

/* ------------------------------------------------- */

void setup() {
  setupSerial(SERIAL_SPEED, "Telnet Test");
  
  Serial.print("- Wifi: ");
  useWiFiManager();

  if (isConnected()) {
    ip = WiFi.localIP();
    Serial.print(" ");
    Serial.println(ip);
    setupTelnet();
  } else {
    Serial.println();    
    errorMsg("Error connecting to WiFi");
  }
}

/* ------------------------------------------------- */

void loop() {
  telnet.loop();

  // send serial input to telnet as output
  if (Serial.available()) {
    telnet.print(Serial.read());
  }
}
//* ------------------------------------------------- */
#else
#error "This example needs an ESP8266"
#endif
//* ------------------------------------------------- */
