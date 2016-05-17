//
// syslog, oh syslog, where art tho' syslog ..
// 
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <IPAddress.h>


// #define this to get syslog before normal AP autoconnect
#define SYSLOG_AUTO_CONNECT

IPAddress parseIPString(char *str)
{
// char str[] = "123 190 42 175";
	uint8_t values[4];
	uint8_t valuesCount = 0;

	char *p = strtok(str, ".");
	while (p != NULL && valuesCount < 4) {
		values[valuesCount++] = atoi(p);
		p = strtok(NULL, ".");
	}

	if (valuesCount == 4) {
		return IPAddress(values[0], values[1], values[2], values[3]);
	} else
		return IPAddress(0, 0, 0, 0);
}

class MagicShifterSysLog {

  private:
	unsigned int localPort = 2390;
	WiFiUDP sysLogUDP;
	IPAddress syslogServer;

  public:

	void connect_wifi() {

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
	} void setup(char *syslogHostIPStr) {

		syslogServer = parseIPString(syslogHostIPStr);

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

#ifndef CONFIG_MIDI_ONBOARD
		Serial.println(newMsg);
#endif

		sysLogUDP.beginPacket(syslogServer, 514);
		sysLogUDP.write(p, msg_length);
		sysLogUDP.endPacket();

		free(p);
	}

};
