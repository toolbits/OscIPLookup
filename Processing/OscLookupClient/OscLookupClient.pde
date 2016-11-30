import netP5.*;
import oscP5.*;

LookupClient lookup;
// for TCP
OscP5 oscTCP;
// for UDP
OscP5 oscUDP;

void setup() {
  lookup = new LookupClient("disp1");
// for TCP
  oscTCP = new OscP5(this, 7600, OscP5.TCP);
// for UDP
  oscUDP = new OscP5(this, 8600);
  return;
}

void draw() {
  // need calling "update" to poll incoming signal
  lookup.update();
  return;
}

synchronized void oscEvent(OscMessage msg) {
  print("### received an osc message.");
  print(" addrpattern: "+msg.addrPattern());
  println(" typetag: "+msg.typetag());

  if (msg.checkAddrPattern("/my_test")) {
    if (msg.checkTypetag("s")) {
      println(msg.get(0).stringValue());
    }
  }
  return;
}