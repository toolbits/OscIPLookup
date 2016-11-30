import netP5.*;
import oscP5.*;

LookupServer lookup;
// for TCP
OscP5 oscTCP;
// for UDP
OscP5 oscUDP;

void setup() {
  lookup = new LookupServer();
  // for UDP
  oscUDP = new OscP5(this, 8601);
  return;
}

void draw() {
  return;
}

void mousePressed()
{
  OscMessage msg;

  msg = new OscMessage("/my_test");
  msg.add("test");

  // send an OSC message via TCP
  if (lookup.hasUpdate("macbookpro")) {
    oscTCP = new OscP5(this, lookup.get("macbookpro"), 7600, OscP5.TCP);
  }
  if (oscTCP != null) {
    if (oscTCP.tcpClient().socket() == null) {
      oscTCP.tcpClient().reconnect();
    }
    oscTCP.send(msg);
  }

  // send an OSC message via UDP
  if (!lookup.get("arduino").isEmpty()) {
    oscUDP.send(msg, new NetAddress(lookup.get("arduino"), 8600));
  }
  return;
}