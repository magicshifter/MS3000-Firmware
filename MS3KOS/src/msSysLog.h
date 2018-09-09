//
// syslog, oh syslog, where art tho' syslog ..
// 
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <IPAddress.h>

// #define this to get syslog before normal AP autoconnect
#undef SYSLOG_AUTO_CONNECT

class MagicShifterSysLog {

  private:
	unsigned int localPort = 2390;
	WiFiUDP sysLogUDP;
	IPAddress syslogServerIP;

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
#ifndef CONFIG_MIDI_SERIAL_ENABLE
		Serial.println("syslog: JoinAP disabled:");
#endif
#endif

#ifndef CONFIG_MIDI_SERIAL_ENABLE
		if (msGlobals.ggDebugSerial) {
			Serial.println("syslog: serial enable");
			Serial.println("syslog: WiFi: " + 
				String((WiFi.status() == WL_CONNECTED) ? "connected" : "not connected" ));
			Serial.print("syslog: local address: ");
			Serial.println(WiFi.localIP());
			Serial.print("syslog: host address: ");
			Serial.println(syslogServerIP);
		}
#endif

	};

	// 
	void setup(char *syslogServerIPStr) {

		bool validIPConfig = syslogServerIP.fromString(syslogServerIPStr);

#ifndef CONFIG_MIDI_SERIAL_ENABLE
		Serial.println("syslog: Serial Console Messages ENABLED");
		Serial.println("syslog: user-configured host: " + String(syslogServerIPStr));
		Serial.println("syslog: configured host is IP: " + syslogServerIP.toString());

		if (validIPConfig) {
			Serial.println("syslog: IP Config appears to be valid");
		} else {
			Serial.println("syslog: IP Config appears to be INVALID");
		}
#endif

		delay(20);

		connect_wifi();

		sysLogUDP.begin(localPort);

		delay(125);

		if (WiFi.status() == WL_CONNECTED) {
			sendSysLogMsg("MagicShifter3000 reporting for duty!");
		} else {
#ifndef CONFIG_MIDI_SERIAL_ENABLE
		Serial.println("syslog: cannot send messages - no network");
#endif
		}

	}

	void sendSysLogMsg(String aMsg) {
		// String newMsg = " 009.local <45>MAGICSHIFTER:" + aMsg;	// !J! todo: fix level/service?
		String newMsg = "MS3K:" + aMsg;	// !J! todo: fix level/service?

		unsigned int msg_length = newMsg.length();
		byte *p = (byte *) malloc(msg_length);
		memcpy(p, (char *) newMsg.c_str(), msg_length);

#ifdef _DO_SERIAL_OUTPUT
#warning "SERIAL OUTPUT HAS BEEN ENABLED"
		Serial.println(newMsg);
#endif

		sysLogUDP.beginPacket(syslogServerIP, 10514);
		sysLogUDP.write(p, msg_length);
		sysLogUDP.endPacket();

		free(p);
	}

};
