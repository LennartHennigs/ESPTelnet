/* ------------------------------------------------- */
/*
    This example uses the WiFiManager class (https://github.com/tzapu/WiFiManager), 
    IMO the most convenient way to connect an ESP to a WiFi.
    
    It stores WiFi credentials in the ESPs. 
    If it has no credentials stored, it will open a WiFi AP.

    In this example a telnet server is created.
    In the loop a "." is sent to the server.
    You probably want to use the telnet server to send debug / error logs.
*/
/* ------------------------------------------------- */
#include "WiFiManager.h"
#include "ESPTelnet.h"          

#define SERIAL_SPEED    9600
#define PORTAL_TIMEOUT  10 * 60 // seconds
#define AP_NAME         "MY CAPTIVE PORTAL"
#define AP_PASSWORD     ""

/* ------------------------------------------------- */
WiFiManager wifiManager;
ESPTelnet telnet;

IPAddress ip_addr;
/* ------------------------------------------------- */
void connectToWifi() {
  // wifiManager.resetSettings();  // this will delete all credentials
  wifiManager.setDebugOutput(false);
  wifiManager.setConfigPortalTimeout(PORTAL_TIMEOUT);
  wifiManager.setAPCallback([] (WiFiManager *myWiFiManager) {
    Serial.println("- No known wifi found");
    Serial.print("- Starting AP: ");
    Serial.println(myWiFiManager->getConfigPortalSSID());
    Serial.println(WiFi.softAPIP());
  });
  // enable autoconnect
  if (!(AP_PASSWORD == "" ? 
    wifiManager.autoConnect(AP_NAME) : 
    wifiManager.autoConnect(AP_NAME, AP_PASSWORD))
   ) {
    Serial.println("- Failed to connect and hit timeout");
    ESP.reset();
    delay(1000); 
  // known wifi found & connected
  }  else {
    ip_addr = WiFi.localIP();
    String mac_addr = WiFi.macAddress();
    String wifi = WiFi.SSID();

    Serial.println("- WiFi: " + wifi);
    Serial.println("- IP: " + ip_addr.toString());
  }
}
/* ------------------------------------------------- */
void setupTelnet() {
if (telnet.begin()) {
      telnet.onConnect(onTelnetConnect);
      telnet.onConnectionAttempt(onTelnetConnectionAttempt);
      telnet.onReconnect(onTelnetReconnect);
      telnet.onDisconnect(onTelnetDisconnect);
      Serial.println("- Telnet is running. Connect to Telnet using:");
      Serial.println("telnet " + ip_addr.toString());
    
    // could not create server
    } else {
      Serial.println("- Failed to create Telnet server. Is device connected to WiFi?");
      delay(10000);
      ESP.reset();
      delay(1000); 
    }
}
/* ------------------------------------------------- */
// (optional) callback functions for server
void onTelnetConnect() {
  Serial.print("- Telnet: ");
  Serial.print(telnet.getIP());
  Serial.println(" connected");
  telnet.println("\nWelcome " + telnet.getIP());
  telnet.println("(Use ^] + q  to disconnect.)");
}
void onTelnetDisconnect() {
  Serial.print("- Telnet: ");
  Serial.print(telnet.getIP());
  Serial.println(" disconnected");
}
void onTelnetReconnect() {
  Serial.print("- Telnet: ");
  Serial.print(telnet.getIP());
  Serial.println(" reconnected");
}
void onTelnetConnectionAttempt() {
  Serial.print("- Telnet: ");
  Serial.print(telnet.getLastAttemptIP());
  Serial.println(" tried to connected");
}
/* ------------------------------------------------- */
void setup() {
  // setup serial output
  Serial.begin(SERIAL_SPEED);
  while (!Serial) {
    delay(10);
  }
  Serial.println("\n\nTelnet Test");
  connectToWifi();
  setupTelnet();
  Serial.println();
}
/* ------------------------------------------------- */
int counter = 0;

void loop() {
  telnet.loop();
  telnet << ".";            // same as: telnet.print(".");
  if(counter++ % 10 == 0) telnet.println();
  delay(1000);
}
//* ------------------------------------------------- */