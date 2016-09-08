//
// syslog, oh syslog, where art tho' syslog ..
// 
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <IPAddress.h>

// #define this to get syslog before normal AP autoconnect
#define SYSLOG_AUTO_CONNECT

class MagicShifterSysLog {

  private:
	unsigned int localPort = 2390;
	WiFiUDP sysLogUDP;
	IPAddress syslogServer;

  public:

  	//
	void connect_wifi() {

	// we may wish to configure syslog on/off for dev/user modes
#ifdef SYSLOG_AUTO_CONNECT
		WiFi.mode(WIFI_AP_STA);

		if (WiFi.status() != WL_CONNECTED) {

			WiFi.begin(msGlobals.ggAPConfig.apInfo.auth.ssid,
					   msGlobals.ggAPConfig.apInfo.auth.password);

			int Attempt = 0;
			while (WiFi.status() != WL_CONNECTED) {
				delay(100);
				Attempt++;
				if (msGlobals.ggDebugSerial) {
					Serial.print(".");
				}
				if (Attempt == 200) {
					ESP.restart();	// !J! todo: wtf
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
	};

	// 
	void setup(char *syslogHostIPStr) {

		syslogServer = syslogServer.fromString(syslogHostIPStr);

		Serial.print("syslog: configured host is:");
		Serial.println(syslogServer);

		delay(20);
		connect_wifi();

		sysLogUDP.begin(localPort);
		delay(500);
		if (WiFi.status() == WL_CONNECTED) {
			sendSysLogMsg("MagicShifter3000 reporting for duty!");
		}
	}

	// poll serial and route it to syslog server
	void pollSerial() {
		if (WiFi.status() == WL_CONNECTED) {
		} else {
			connect_wifi();
		}
	}

	void sendSysLogMsg(String aMsg) {
		// String newMsg = " 009.local <45>MAGICSHIFTER:" + aMsg;	// !J! todo: fix level/service?
		String newMsg = "" + aMsg;	// !J! todo: fix level/service?

		unsigned int msg_length = newMsg.length();
		byte *p = (byte *) malloc(msg_length);
		memcpy(p, (char *) newMsg.c_str(), msg_length);

#ifdef _DO_SERIAL_OUTPUT
#warning "SERIAL OUTPUT HAS BEEN ENABLED"
		Serial.println(newMsg);
#endif

		sysLogUDP.beginPacket(syslogServer, 514);
		sysLogUDP.write(p, msg_length);
		sysLogUDP.endPacket();

		free(p);
	}

};
