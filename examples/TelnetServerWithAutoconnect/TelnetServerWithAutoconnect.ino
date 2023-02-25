/* ------------------------------------------------- */

#include "ESPTelnet.h"
#include "AutoConnect.h"

/* ------------------------------------------------- */

#define SERIAL_SPEED 9600
// AP-PASSWORD 12345678

/* ------------------------------------------------- */
ESPTelnet   telnet;
IPAddress   ip;
WebServer   server;
AutoConnect portal(server);

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

static void rootPage() {
  char content[] = "Welcome to ESP Telnet";
  server.send(200, "text/plain", content);
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

// Type your browser:http://{{local_ip_address}}/_ac
void useAutoConnect() {
  server.on("/", rootPage);
  if (portal.begin()) {
    ip = WiFi.localIP();
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
    setupTelnet();
  } else {
    Serial.println();
    errorMsg("Error connecting to WiFi");
  }
}

/* ------------------------------------------------- */

void setupTelnet() {
  // passing on functions for various telnet events
  telnet.onConnect(onTelnetConnect);
  telnet.onConnectionAttempt(onTelnetConnectionAttempt);
  telnet.onReconnect(onTelnetReconnect);
  telnet.onDisconnect(onTelnetDisconnect);
  telnet.onInputReceived(onInput);

  Serial.print("- Telnet: ");
  if (telnet.begin()) {
    Serial.println("running");
  } else {
    Serial.println("error.");
    errorMsg("Will reboot...");
  }
}

/* ------------------------------------------------- */

void onInput(String str) {
  // checks for a certain command
  if (str == "ping") {
    telnet.println("> pong");
    Serial.println("- Telnet: pong");
  // disconnect the client
  } else if (str == "bye") {
    telnet.println("> disconnecting you...");
    telnet.disconnectClient();
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

/* ------------------------------------------------- */

void onTelnetDisconnect(String ip) {
  Serial.print("- Telnet: ");
  Serial.print(ip);
  Serial.println(" disconnected");
}

/* ------------------------------------------------- */

void onTelnetReconnect(String ip) {
  Serial.print("- Telnet: ");
  Serial.print(ip);
  Serial.println(" reconnected");
}

/* ------------------------------------------------- */

void onTelnetConnectionAttempt(String ip) {
  Serial.print("- Telnet: ");
  Serial.print(ip);
  Serial.println(" tried to connected");
}

/* ------------------------------------------------- */

void setup() {
  setupSerial(SERIAL_SPEED, "Telnet Test");

  Serial.print("- Wifi: ");
  useAutoConnect();

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
  portal.handleClient();
  telnet.loop();

  if (Serial.available()) {
    telnet.print(Serial.read());
  }
}

/* ------------------------------------------------- */
