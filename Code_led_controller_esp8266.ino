// ON/OFF Debug serial
#define ESP_DEBUG

#include "configure.h"

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <RTClib.h>
#include <Wire.h>

#ifndef STASSID
  #define STASSID "Ba Tu"
  #define STAPSK  "giadinhlaso1"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

RTC_DS3231 rtc;

void setup() {
  Serial.begin(115200);

  if (! rtc.begin()) {
    _DEBBUG("Couldn't find RTC");
    abort();
  }
  if (rtc.lostPower()) {
    _DEBBUG("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  _DEBBUG("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    _DEBBUG("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    _DEBBUG("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    _DEBBUG("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    _DEBBUG("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    _DEBBUG("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      _DEBBUG("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      _DEBBUG("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      _DEBBUG("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      _DEBBUG("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      _DEBBUG("End Failed");
    }
  });
  ArduinoOTA.begin();
  _DEBBUG("Ready");
  _DEBBUG("IP address: ");
  _DEBBUG(WiFi.localIP());
}

void loop() {
  ArduinoOTA.handle();
}
