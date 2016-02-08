//
// syslog, oh syslog, where art tho' syslog ..
// 
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <IPAddress.h>

IPAddress syslogServer(192, 168, 10, 10);

class MagicShifterSysLog {

private:
  unsigned int localPort = 2390;

  String str;
  //long startMills;

  WiFiClient wifiClient;
  WiFiUDP udp;

public:
  void connect_wifi() {

    WiFi.mode(WIFI_STA);
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
        ESP.restart();
      }
    }
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

  void loop()
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

  void sendSysLogMsg(String msgtosend)
  {
    unsigned int msg_length = msgtosend.length();
    byte* p = (byte*)malloc(msg_length);
    memcpy(p, (char*) msgtosend.c_str(), msg_length);

    udp.beginPacket(syslogServer, 514);
    udp.write("esp8266-02-syslog ");
    udp.write(p, msg_length);
    udp.endPacket();
    free(p);
  }

};
