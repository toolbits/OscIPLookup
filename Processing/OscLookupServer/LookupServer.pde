import netP5.*;
import oscP5.*;

class LookupServer implements OscEventListener {
  private OscP5 _oscP5;
  private StringDict _dict;
  private IntDict _update;

  public LookupServer() {
    _oscP5 = new OscP5(this, 5555);
    _dict = new StringDict();
    _update = new IntDict();
    return;
  }

  public String get(String name) {
    String result = "";

    try {
      if (_dict.get(name) != null) {
        result = _dict.get(name);
      }
    }
    catch (Exception e) {
    }
    return result;
  }

  public boolean hasUpdate(String name) {
    boolean result = false;

    try {
      result = _update.get(name) != 0;
      _update.set(name, 0);
    }
    catch (Exception e) {
    }
    return result;
  }

  public void oscEvent(OscMessage msg) {
    String name;
    String ip;

    if (msg.checkAddrPattern("/lookup")) {
      if (msg.checkTypetag("ss")) {
        name = msg.get(0).stringValue();
        ip = msg.get(1).stringValue();
        if (!name.isEmpty() && !ip.isEmpty()) {
          if (!get(name).equals(ip)) {
            _dict.set(name, ip);
            _update.set(name, 1);
          }
          println("[LookupServer] " + name + " : " + ip);
        }
      }
    }
    return;
  }

  public void oscStatus(OscStatus status) {
    return;
  }
}