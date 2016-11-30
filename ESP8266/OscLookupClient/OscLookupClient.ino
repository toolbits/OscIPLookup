#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCBundle.h>
#include "LookupClient.h"

LookupClient _lookup("disp1");
WiFiUDP _osc;

void setup()
{
  Serial.begin(9600);
  WiFi.begin("art.idd");

  _osc.begin(8600);
  return;
}

void loop()
{
  OSCMessage msg;
  int size;

  _lookup.update();
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
  return;
}

void handleTest(OSCMessage& msg)
{
  char temp[256];

  msg.getString(0, temp, sizeof(temp));
  Serial.println(temp);
  return;
}

