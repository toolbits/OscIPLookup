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
      result = _dict.get(name);
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
    String prev;

    if (msg.checkAddrPattern("/lookup")) {
      if (msg.checkTypetag("ss")) {
        name = msg.get(0).stringValue();
        ip = msg.get(1).stringValue();
        if (!name.isEmpty() && !ip.isEmpty()) {
          prev = "";
          try {
            prev = _dict.get(name);
          }
          catch (Exception e) {
          }
          if (!ip.equals(prev)) {
            _update.set(name, 1);
          }
          _dict.set(name, ip);
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