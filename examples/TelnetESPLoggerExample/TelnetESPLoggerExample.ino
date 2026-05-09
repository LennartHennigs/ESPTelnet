// An example showing how to use ESPTelnet with the standard ESP logger

/* ------------------------------------------------- */

#include <Arduino.h>
#include <esp32-hal-log.h>
#include "ESPTelnet.h"

/* ------------------------------------------------- */

#define SERIAL_SPEED    9600
#define WIFI_SSID       "MY SSID"
#define WIFI_PASSWORD   "MY PASS"

/* ------------------------------------------------- */

ESPTelnet telnet;
IPAddress ip;
uint16_t  port = 23;
static const char LOG_TAG[] = "TelnetLog";
static TaskHandle_t hTelnet = NULL;

/* ------------------------------------------------- */

static void telnet_task(void*) {
    while(1) {
        telnet.loop();
        delay(100);
    }
    vTaskDelete(NULL);
}

// This function will be called by the ESP log library every time ESP_LOG needs to be performed when the telnet server is started.
// Do NOT use the ESP_LOG* macro's in this function or things will explodify!
int _telnet_log_vprintf(const char *fmt, va_list args) {
    telnet.vprintf(fmt, args);
    return vprintf(fmt, args);
}

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
  Serial.print("- Telnet: ");
  if (telnet.begin(port)) {
    Serial.println("running");
  } else {
    Serial.println("error.");
    errorMsg("Will reboot...");
  }
}

void setupLogging() {
  xTaskCreate(
      telnet_task,
      "TELOG",
      4096,
      NULL,
      1,
      &hTelnet
  );
  esp_log_set_vprintf(_telnet_log_vprintf);
}

/* ------------------------------------------------- */

void setup() {
  setupSerial(SERIAL_SPEED, "Telnet Test");
  
  Serial.print("- Wifi: ");
  connectToWiFi(WIFI_SSID, WIFI_PASSWORD);
  
  if (isConnected()) {
    ip = WiFi.localIP();
    Serial.println();
    Serial.print("- Telnet: "); Serial.print(ip); Serial.print(":"); Serial.println(port);
    setupTelnet();
    setupLogging();
  } else {
    Serial.println();    
    errorMsg("Error connecting to WiFi");
  }
}

/* ------------------------------------------------- */

void loop() {
  ESP_LOGI(LOG_TAG, "Info!");
  delay(1000);
  ESP_LOGW(LOG_TAG, "Warning!");
  delay(1000);
  ESP_LOGE(LOG_TAG, "Error!");
  delay(1000);
}

//* ------------------------------------------------- */