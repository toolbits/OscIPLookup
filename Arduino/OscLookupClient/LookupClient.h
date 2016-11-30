#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCBundle.h>

class LookupClient {
  private:
    WiFiUDP _udp;
    String _name;
    int _time;

  public:
    explicit LookupClient(String const& name) {
      _name = name;
      _time = -1;
      return;
    }
    void update(void) {
      OSCMessage msg;
      int time;

      time = millis();
      if (time >= _time) {
        _time = time + 10000;
        if (WiFi.status() == WL_NO_SSID_AVAIL || WiFi.status() == WL_CONNECTION_LOST || WiFi.status() == WL_DISCONNECTED) {
          _udp.stop();
          wifiStart();
          _udp.begin(5556);
        }
        msg.setAddress("/lookup");
        msg.add(_name.c_str());
        msg.add(WiFi.localIP());
        _udp.beginPacket(IPAddress(255, 255, 255, 255), 5555);
        Serial.println("[LookupClient] " + _name + " : " + WiFi.localIP());
      }
      return;
    }
  private:
    void wifiStart(void) {
      int status;

      do {
        status = WiFi.begin();
        delay(1000);
        Serial.print(".");
      } while (status != WL_CONNECTED);
      Serial.println("");
      Serial.println("WiFi connected");
      printWifiStatus();
    }
    void printWifiStatus(void)
    {
      // print the SSID of the network you're attached to:
      Serial.print("SSID: ");
      Serial.println(WiFi.SSID());

      // print your WiFi shield's IP address:
      IPAddress ip = WiFi.localIP();
      Serial.print("IP Address: ");
      Serial.println(ip);

      // print the received signal strength:
      long rssi = WiFi.RSSI();
      Serial.print("signal strength (RSSI):");
      Serial.print(rssi);
      Serial.println(" dBm");
    }
  private:
    LookupClient(LookupClient const&);
    LookupClient& operator=(LookupClient const&);
};

