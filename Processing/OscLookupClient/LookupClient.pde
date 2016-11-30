import netP5.*;
import oscP5.*;
import java.lang.reflect.Field;

class LookupClient {
  private OscP5 _oscP5;
  private NetAddress _addr;
  private String _name;
  private int _time;

  public LookupClient(String name) {
    _oscP5 = new OscP5(this, 5556);
    enableBroadcast();
    _addr = new NetAddress("255.255.255.255", 5555);
    _name = name;
    _time = -1;
    return;
  }

  public void update() {
    OscMessage msg;
    int time;

    time = second();
    if (time >= _time) {
      _time = time + 10;
      msg = new OscMessage("/lookup");
      msg.add(_name);
      msg.add(_oscP5.ip());
      _oscP5.send(msg, _addr);
      println("[LookupClient] " + _name + " : " + _oscP5.ip());
    }
    return;
  }

  private void enableBroadcast() {
    Field fields[];
    OscNetManager manager;
    UdpClient client;
    int i;

    fields = OscP5.class.getDeclaredFields();
    for (i = 0; i < fields.length; ++i) {
      if (fields[i].getName().equals("_myOscNetManager")) {
        fields[i].setAccessible(true);
        try {
          manager = (OscNetManager)fields[i].get(_oscP5);
          fields = OscNetManager.class.getDeclaredFields();
          for (i = 0; i < fields.length; ++i) {
            if (fields[i].getName().equals("_myUdpClient")) {
              fields[i].setAccessible(true);
              try {
                client = (UdpClient)fields[i].get(manager);
                client.socket().setBroadcast(true);
              }
              catch (Exception e) {
                e.printStackTrace();
              }
              break;
            }
          }
        }
        catch (Exception e) {
          e.printStackTrace();
        }
        break;
      }
    }
    return;
  }
}