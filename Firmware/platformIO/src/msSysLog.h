//
// syslog, oh syslog, where art tho' syslog ..
// 
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <IPAddress.h>

#undef _DO_SERIAL_ECHO

// we depend on SCAN_FIRST_MODE

// .2 is the first client to connect in SoftAP ..
IPAddress syslogServer(192, 168, 4, 2);

class MagicShifterSysLog {

private:
  unsigned int localPort = 2390;

  String str;
  //long startMills;

  WiFiClient wifiClient;
  WiFiUDP udp;

public:

  void connect_wifi() {
#ifdef SCAN_FIRST_MODE
    WiFi.mode(WIFI_STA);

    Serial.print("sysLOG SSID:"); Serial.println(msGlobals.ggAPInfo.ssid);
    Serial.print("sysLOG pass:"); Serial.println(msGlobals.ggAPInfo.password);

    WiFi.begin(msGlobals.ggAPInfo.ssid, msGlobals.ggAPInfo.password);

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
#else
    Serial.println("ForceJoinAP disabled:");
#endif

    if (msGlobals.ggDebugSerial) {
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    }
  }

  void setup()
  {
    delay(20);
    connect_wifi();

    udp.begin(localPort);
    delay(500);
    if (WiFi.status() == WL_CONNECTED) {
      sendSysLogMsg("esp8266-02-syslog started");
    }
  }

  void step()
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
    String newMsg = " 009.local <45>" + aMsg; // !J! level/service?

    unsigned int msg_length = newMsg.length();
    byte* p = (byte*)malloc(msg_length);
    memcpy(p, (char*) newMsg.c_str(), msg_length);

Serial.print("SSLMSG:");
Serial.println(newMsg);

    udp.beginPacket(syslogServer, 514);
    udp.write(p, msg_length);
    udp.endPacket();
    free(p);
  }

};
