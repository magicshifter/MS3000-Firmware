//
// syslog, oh syslog, where art tho' syslog ..
// 
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <IPAddress.h>

#undef _DO_SERIAL_ECHO
// SYSLOG_AUTO_CONNECT

// .2 is the first client to connect in SoftAP ..
IPAddress syslogServer(192, 168, 2, 186);

class MagicShifterSysLog {

private:
  unsigned int localPort = 2390;

  String str;
  //long startMills;

  WiFiClient wifiClient;
  WiFiUDP udp;

public:

  void connect_wifi() {
#ifdef SYSLOG_AUTO_CONNECT
    WiFi.mode(WIFI_AP_STA);

    // Serial.print("sysLOG SSID:"); Serial.println(msGlobals.ggAPConfig.apInfo.ssid);
    // Serial.print("sysLOG pass:"); Serial.println(msGlobals.ggAPConfig.apInfo.password);

    if (WiFi.status() != WL_CONNECTED) {

      WiFi.begin(msGlobals.ggAPConfig.apInfo.ssid, msGlobals.ggAPConfig.apInfo.password);

      int Attempt = 0;
      while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Attempt++;
        if (msGlobals.ggDebugSerial) {
          Serial.print(".");
        }
        if (Attempt == 200)
        {
          ESP.restart(); // !J! todo: wtf
        }
      }
    }
#else
    Serial.println("syslog: JoinAP disabled:");
#endif

    if (msGlobals.ggDebugSerial) {
      Serial.println("syslog: serial enable");
      Serial.println("syslog: WiFi connected");
      Serial.print("syslog: local address: ");
      Serial.println(WiFi.localIP());
      Serial.print("syslog: host address: ");
      Serial.println(syslogServer);
    }
  }

  void setup()
  {
    delay(20);
    connect_wifi();

    udp.begin(localPort);
    delay(500);
    if (WiFi.status() == WL_CONNECTED) {
      sendSysLogMsg("MagicShifter3000 reporting for duty!");
    }
  }

  // poll serial and route it to syslog server
  void pollSerial()
  {
    if (WiFi.status() == WL_CONNECTED) {
#if _DO_SERIAL_ECHO
      str = Serial.readStringUntil('\n');
      str.trim();
      unsigned int msg_length = str.length();
      if ( msg_length > 0 ) {
        sendSysLogMsg(str);
      }
#endif
    } else {
      connect_wifi();
    }
  }

  void sendSysLogMsg(String aMsg)
  {
    String newMsg = " 009.local <45>" + aMsg; // !J! todo: fix level/service?

    unsigned int msg_length = newMsg.length();
    byte* p = (byte*)malloc(msg_length);
    memcpy(p, (char*) newMsg.c_str(), msg_length);

    udp.beginPacket(syslogServer, 514);
    udp.write(p, msg_length);
    udp.endPacket();
    free(p);
  }

};
