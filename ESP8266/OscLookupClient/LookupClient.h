#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCBundle.h>

class LookupClient {
  private:
    WiFiUDP _udp;
    String _name;
    unsigned long _time;

  public:
    explicit LookupClient(String const& name) {
      _name = name;
      _time = 0;
      return;
    }

    void update(void) {
      String ip;
      OSCMessage msg;
      unsigned long time;

      time = millis();
      if (time >= _time) {
        if (WiFi.status() == WL_CONNECTED) {
          if (!_udp) {
            if (!_udp.begin(5556)) {
              _udp.stop();
            }
          }
          if (_udp) {
            ip = toString(WiFi.localIP());
            msg.empty();
            msg.setAddress("/lookup");
            msg.add(_name.c_str());
            msg.add(ip.c_str());
            _udp.beginPacket(IPAddress(255, 255, 255, 255), 5555);
            msg.send(_udp);
            _udp.endPacket();
            Serial.println("[LookupClient] " + _name + " : " + ip);
            _time = time + 10000 + (int(micros() % 11) - 5) * 200;
          }
          else {
            _time = time + 1000;
          }
        }
        else {
          if (_udp) {
            _udp.stop();
          }
          _time = time + 1000;
        }
      }
      return;
    }
  private:
    String toString(IPAddress const& ip) {
      int i;
      String result;

      result += ip[0];
      for (i = 1; i < 4; ++i) {
        result += ".";
        result += ip[i];
      }
      return result;
    }
  private:
    LookupClient(LookupClient const&);
    LookupClient& operator=(LookupClient const&);
};

