#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCBundle.h>
#include "LookupClient.h"

#define LOOKUP_NAME   ("disp1")
#define WIFI_SSID     ("!!! your wifi ssid !!!")
#define WIFI_PASS     ("!!! your wifi pass !!!")
#define OSC_PORT      (8600)

LookupClient _lookup(LOOKUP_NAME);
WiFiUDP _osc;

void setup() {
  Serial.begin(115200);
  return;
}

void loop() {
  OSCMessage msg;
  int size;

  updateWIFI();
  updateOSC();
  _lookup.update();
  if (_osc) {
    size = _osc.parsePacket();
    if (size > 0) {
      while (size--) {
        msg.fill(_osc.read());
      }
      if (!msg.hasError()) {
        msg.dispatch("/my_test", &handleTest);
      }
      else {
        Serial.println("OSC error: ");
        Serial.println(msg.getError());
      }
    }
  }
  return;
}

void handleTest(OSCMessage& msg) {
  char temp[256];

  msg.getString(0, temp, sizeof(temp));
  Serial.println(temp);
  return;
}

void updateWIFI() {
  int i;

  if (WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect();
    Serial.print("WIFI connect");
    WiFi.mode(WIFI_STA);
#ifdef WIFI_PASS
    WiFi.begin(WIFI_SSID, WIFI_PASS);
#else
    WiFi.begin(WIFI_SSID);
#endif
    for (i = 0; i < 60 && WiFi.status() != WL_CONNECTED; ++i) {
      Serial.print(".");
      delay(500);
    }
    Serial.println("");
    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("SSID: ");
      Serial.println(WiFi.SSID());
      Serial.print("IP: ");
      Serial.println(WiFi.localIP());
      Serial.print("RSSI: ");
      Serial.print(WiFi.RSSI());
      Serial.println(" dBm");
    }
    else {
      WiFi.disconnect();
      Serial.println("WIFI timeout");
    }
  }
  return;
}

void updateOSC() {
  if (WiFi.status() == WL_CONNECTED) {
    if (!_osc) {
      if (!_osc.begin(OSC_PORT)) {
        _osc.stop();
      }
    }
  }
  return;
}

