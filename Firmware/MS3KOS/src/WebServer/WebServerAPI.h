#ifndef _WEBSERVERAPI_H
#define _WEBSERVERAPI_H

#include "Base64.h"

class SettingsManager {
  private:
	// used in resetAPList & getNextAP
	int apListIndex = -1;
	File apListFile;

  private:
	 bool loadData(String path, void *config, int len) {
		if (SPIFFS.exists((char *) path.c_str())) {
			File file = SPIFFS.open((char *) path.c_str(), "r");
			 file.read((uint8_t *) config, len);
			 file.close();
			 return true;
		} else {
			msSystem.
				slog("webserver: loadData: can not open config file ");
			msSystem.slogln((char *) path.c_str());
		}
		return false;
	}

	bool saveData(String path, void *config, int len) {
		File file = SPIFFS.open((char *) path.c_str(), "w");
		if (file) {
			file.write((uint8_t *) config, len);
			file.close();
			return true;
		} else {
			msSystem.slog("webserver: can not open config file ");
			msSystem.slogln((char *) path.c_str());
			return false;
		}
	}

  public:
	const String apConfigPath = "settings/ap.bin";
	const String apServerConfigPath = "settings/server1.bin";
	const String apListConfigPath = "settings/aplist1.bin";
	const String apSysLogConfigPath = "settings/syslog.bin";
	const String preferredAPConfigPath = "settings/preferredap.bin";
	const String uiSettingsConfigPath = "settings/ui.bin";

	bool getUIConfig(struct ServerConfig *config) {
		// msSystem.slog("config: sizeof ");
		//  msSystem.slogln(sizeof(*config));
		return loadData(uiSettingsConfigPath, config, sizeof(*config));
	}

	bool setUIConfig(struct ServerConfig *config) {
		return saveData(uiSettingsConfigPath, config, sizeof(*config));
	}

	bool getServerConfig(struct ServerConfig *config) {

		String path = apServerConfigPath;
		if (SPIFFS.exists((char *) path.c_str())) {
			File file = SPIFFS.open((char *) path.c_str(), "r");
			file.read((uint8_t *) config, sizeof(*config));
			file.close();
			return true;
		} else {
			msSystem.slog("webserver: no server config file? ");
			msSystem.slogln((char *) path.c_str());
		}

		return false;
	}

	void setServerConfig(struct ServerConfig *config) {

		String path = apServerConfigPath;
		File file = SPIFFS.open((char *) path.c_str(), "w");
		file.write((uint8_t *) config, sizeof(*config));

		file.close();

	}

	bool getSyslogConfig(struct ServerConfig *config) {

		String path = apSysLogConfigPath;
		if (SPIFFS.exists((char *) path.c_str())) {
			File file = SPIFFS.open((char *) path.c_str(), "r");
			file.read((uint8_t *) config, sizeof(*config));
			file.close();
			return true;
		} else {
			msSystem.slog("webserver: no syslog config file? ");
			msSystem.slogln((char *) path.c_str());
		}

		return false;
	}

	void setSyslogConfig(struct ServerConfig *config) {

		String path = apSysLogConfigPath;
		File file = SPIFFS.open((char *) path.c_str(), "w");
		file.write((uint8_t *) config, sizeof(*config));

		file.close();

	}

	bool getAPConfig(struct APInfo *config) {

		String path = apConfigPath;
		if (SPIFFS.exists((char *) path.c_str())) {
			File file = SPIFFS.open((char *) path.c_str(), "r");
			file.read((uint8_t *) config, sizeof(*config));
			file.close();
			return true;
		} else {
			msSystem.slogln("webserver: AP config missing:");
			msSystem.slogln((char *) path.c_str());
		}


#ifdef AP_NAME_OVERRIDE
		l_safeStrncpy(config->ssid, AP_NAME_OVERRIDE,
					  sizeof(config->ssid));
		l_safeStrncpy(config->password, "", sizeof(config->password));
#endif

		return false;
	}

	void setAPConfig(struct APInfo *config) {

		String path = apConfigPath;
		File file = SPIFFS.open((char *) path.c_str(), "w");
		file.write((uint8_t *) config, sizeof(*config));
		file.close();

		msSystem.slogln("webserver: saved:");
		msSystem.slogln(config->ssid);

	}

	bool getPreferredAP(struct APInfo *config) {

		String path = preferredAPConfigPath;
		if (SPIFFS.exists((char *) path.c_str())) {
			File file = SPIFFS.open((char *) path.c_str(), "r");
			file.read((uint8_t *) config, sizeof(*config));
			file.close();
			return true;
		}
		l_safeStrncpy(config->ssid, "", sizeof(config->ssid));
		l_safeStrncpy(config->password, "", sizeof(config->password));
		return false;

	}

	void setPreferredAP(struct APInfo *config) {

		String path = preferredAPConfigPath;
		File file = SPIFFS.open((char *) path.c_str(), "w");
		file.write((uint8_t *) config, sizeof(*config));
		file.close();

	}

	void deleteAP(char *ssid) {

		String path = apListConfigPath;
		// bug in FS WRITE define!!!!
		//FSFile apListFile = FS.open((char *)path.c_str(), (SPIFFS_RDONLY | SPIFFS_WRONLY | SPIFFS_CREAT));
		File apListFile = SPIFFS.open((char *) path.c_str(), "a+");

		APInfo apInfoDummy;
		const int requiredBytes = sizeof(apInfoDummy);
		int apListIndex = 0;

		int lastPos = apListFile.position();

		while (apListFile.read((uint8_t *) & apInfoDummy, requiredBytes) ==
			   requiredBytes) {
			if (strcmp(apInfoDummy.ssid, ssid) == 0) {
				msSystem.slogln("webserver: deleting wifi:");
				msSystem.slogln(ssid);

				apInfoDummy.clear();
				int calcPos = apListIndex * requiredBytes;
				apListFile.seek(calcPos, SeekSet);
				apListFile.write((uint8_t *) & apInfoDummy, requiredBytes);
				break;
			}
			apListIndex++;
			lastPos = apListFile.position();
		}
		apListFile.close();

	}

	void addAP(struct APInfo *apInfo) {

		String path = apListConfigPath;
		const int requiredBytes = sizeof(*apInfo);
		APInfo apInfoDummy;
		int apListIndex = 0;
		int firstFreePos = -1;
		File apListFile = SPIFFS.open((char *) path.c_str(), "r");

		if (!apListFile) {
			msSystem.slog("webserver: cannot open file:");
			msSystem.slogln(path);
		} else {
			while (apListFile.
				   read((uint8_t *) & apInfoDummy,
						requiredBytes) == requiredBytes) {
				if (firstFreePos < 0
					&& msSystem.msEEPROMs.
					memcmpByte((byte *) & apInfoDummy, 0, requiredBytes)) {
					firstFreePos = apListIndex * requiredBytes;
				} else if (strcmp(apInfoDummy.ssid, apInfo->ssid) == 0) {
					firstFreePos = apListIndex * requiredBytes;
					break;
				}
				apListIndex++;
			}
			apListFile.close();
		}

		if (firstFreePos >= 0) {
			msSystem.slogln("found hole!");
			msSystem.slogln(String(firstFreePos));
			apListFile = SPIFFS.open((char *) path.c_str(), "a+");
			apListFile.seek(firstFreePos, SeekSet);
		} else {
			msSystem.slogln("appending at end");
			apListFile = SPIFFS.open((char *) path.c_str(), "A");

			if (!apListFile) {
				msSystem.slogln("creating new file");
				apListFile = SPIFFS.open((char *) path.c_str(), "w");
			}
		}

		if (!apListFile) {
			msSystem.slog("webserver: creation failed: ");
			msSystem.slogln(path);
		} else {
			apListFile.write((uint8_t *) apInfo, requiredBytes);
			apListFile.close();
			msSystem.slog("webserver: configuration saved: ");
			msSystem.slogln(path);
		}


	}

	void resetAPList() {

		apListIndex = -1;
		apListFile.close();

	}

	bool getNextAP(struct APInfo *apInfo) {

		if (apListIndex < 0) {
			String path = apListConfigPath;
			if (SPIFFS.exists((char *) path.c_str())) {
				apListFile = SPIFFS.open((char *) path.c_str(), "r");
				apListIndex = 0;
			}
		}

		if (apListIndex >= 0) {
			const int requiredBytes = sizeof(*apInfo);
			do {
				if (apListFile.read((uint8_t *) apInfo, requiredBytes) ==
					requiredBytes) {
					apListIndex++;
					if (!msSystem.msEEPROMs.
						memcmpByte((byte *) apInfo, 0, requiredBytes))
						return true;
				} else {
					return false;
				}
			} while (true);
		} else {
			return false;
		}


		///hack
		return false;
	}
};

SettingsManager Settings;


void handleGETInfo(void)
{
	msSystem.slogln("handleGETInfo");

	String response = "{";
	response += "\"api\":{";
	response += "\"versions\":[1],";
	response += "\"prefix\":\"v\"";
	response += "},";
	response += "\"init\": {";
	response += "\"settings\":[";
	response += "\"server\",";
	response += "\"ap\",";
	response += "\"wifi/list\",";
	response += "\"wifi/preferred\"";
	response += "],";
	response += "\"info\":[";
	response += "\"about\",";
	response += "\"status\"";
	response += "]";
	response += "}";
	response += "}";
	msSystem.msESPServer.send(200, "text/plain", response);
}


void handleGETAbout(void)
{
	msSystem.slogln("handleGETAbout");

	String response = "{";
	response += "\"type\":\"MagicShifter3000\",";
	response += "\"format\":\"BGRA\",";
	response += "\"version\":" + String(MS3KOS_VERSION) + ",";
	response += "\"leds\":" + String(MAX_LEDS) + ",";
	response += "\"id\":" + String(ESP.getChipId()) + ",";
	response += "\"flashid\":" + String(ESP.getFlashChipId()) + ",";
	response += "\"flashsize\":" + String(ESP.getFlashChipSize());
	response += "}";
	msSystem.msESPServer.send(200, "text/plain", response);
}

void handleGETStatus(void)
{
	msSystem.slogln("handleGETStatus");

	int adValue = analogRead(A0);

	int ad1V = 1023;

/*
// Metalab wih Flo (Gamma)
#define R601_VAL 200
#define R602_VAL 820
*/
// with jqVic (Alpha, Beta)
#define R601_VAL 270
#define R602_VAL 1000

	float r1 = R601_VAL, r2 = R602_VAL, r3 = 0;

	float voltage = ((float) (r1 + r2 + r3) * adValue) / (r1 * ad1V);


	msSystem.msSensor.readAccelData(msGlobals.ggAccelCount);

	for (int i = 0; i < 3; i++) {
		msGlobals.ggAccel[i] = (float) msGlobals.ggAccelCount[i] / ((1 << 12) / (2 * GSCALE));	// get actual g value, this depends on scale being set
	}

	uint32_t ip = (uint32_t) msSystem.getIP();


	String response = "{";
	response += "\"id\":" + String(ESP.getChipId()) + ",";
	response +=
		"\"uptime\":" + String(millis() - msGlobals.ggBootTime) + ",";
	response += "\"adValue\":" + String(adValue) + ",";
	response += "\"voltage\":" + String(voltage) + ",";
	response += "\"ggAccelTime\":" + String(msGlobals.ggAccelTime) + ",";
	response += "\"ggLFrameTime\":" + String(msGlobals.ggLFrameTime) + ",";
	response +=
		"\"ggLastFrameMicros\":" + String(msGlobals.ggLastFrameMicros) +
		",";
	response +=
		"\"ggCurrentMicros\":" + String(msGlobals.ggCurrentMicros) + ",";
	response += "\"accelRaw\":[";
	response += String(msGlobals.ggAccelCount[0]) + ",";
	response += String(msGlobals.ggAccelCount[1]) + ",";
	response += String(msGlobals.ggAccelCount[2]);
	response += "],";
	response += "\"msGlobals.ggAccel\":[";
	response += String(msGlobals.ggAccel[0]) + ",";
	response += String(msGlobals.ggAccel[1]) + ",";
	response += String(msGlobals.ggAccel[2]);
	response += "],";
	response += "\"ip\":\"";
	response += String(0xFF & (ip >> 0)) + ".";
	response += String(0xFF & (ip >> 8)) + ".";
	response += String(0xFF & (ip >> 16)) + ".";
	response += String(0xFF & (ip >> 24)) + "\"";
	response += "}";
	msSystem.msESPServer.send(200, "text/plain", response);
}

bool parseAPInfoFromServerArgs(APInfo & apInfo)
{
	bool success = true;
	for (int i = 0; i < msSystem.msESPServer.args(); i++) {
		msSystem.slogln("argName: ");
		msSystem.slogln(msSystem.msESPServer.argName(i));

		msSystem.slogln("arg: ");
		msSystem.slogln(msSystem.msESPServer.arg(i));

		if (strcmp(msSystem.msESPServer.argName(i).c_str(), "ssid") == 0) {
			l_safeStrncpy(apInfo.ssid, msSystem.msESPServer.arg(i).c_str(),
						  sizeof(apInfo.ssid));
		} else if (strcmp(msSystem.msESPServer.argName(i).c_str(), "pwd")
				   == 0) {
			l_safeStrncpy(apInfo.password,
						  msSystem.msESPServer.arg(i).c_str(),
						  sizeof(apInfo.password));
		} else {
			msSystem.slogln("arg is not known!");
			success = false;
		}
	}
	return success;
}

bool parseSysLogInfoFromServerArgs(ServerConfig & sysLogHost)
{
	bool success = true;
	for (int i = 0; i < msSystem.msESPServer.args(); i++) {
		msSystem.slogln("argName: ");
		msSystem.slogln(msSystem.msESPServer.argName(i));

		msSystem.slogln("arg: ");
		msSystem.slogln(msSystem.msESPServer.arg(i));

		if (strcmp(msSystem.msESPServer.argName(i).c_str(), "host") == 0) {
			l_safeStrncpy(sysLogHost.hostname,
						  msSystem.msESPServer.arg(i).c_str(),
						  sizeof(sysLogHost.hostname));
		} else if (strcmp(msSystem.msESPServer.argName(i).c_str(), "port")
				   == 0) {
			sysLogHost.port = atoi(msSystem.msESPServer.arg(i).c_str());
		} else {
			msSystem.slogln("arg is not known!");
			success = false;
		}
	}
	return success;
}


void handleGetSettings(void)
{
	msSystem.slogln("handleGetSettings");

	ServerConfig config;
	Settings.getServerConfig(&config);

	String response = "<!DOCTYPE html><html><head></head><body>";;
	response += "<h3>Urls</h3>";
	response += "<ul>";
	response += "<li><a href=\"/settings\">this page (/settings)</a></li>";
	response +=
		"<li><a href=\"/settings/server\">server settings (/settings/server)</a></li>";
	response +=
		"<li><a href=\"/settings/ap\">accesspoint settings (/settings/ap)</a></li>";

	response += "<li><a href=\"/info\">the schema page (/info)</a></li>";

	response +=
		"<li><a href=\"/info/about\">about info page (/info/about)</a></li>";
	response +=
		"<li><a href=\"/info/status\">status output page (/info/status)</a></li>";

	response +=
		"<li><a href=\"/settings/wifi/list\">list of saved wifis (/settings/wifi/list)</a></li>";
	response +=
		"<li><a href=\"/settings/wifi/preferred\">preferred wifi (/settings/wifi/preferred)</a></li>";

	response += "</ul>";
	response += "</body></html>";
	msSystem.msESPServer.send(200, "text/html", response);
}

void handleGETServerSettings(void)
{
	msSystem.slogln("handleGETServerSettings");

	ServerConfig config;
	Settings.getServerConfig(&config);

	String response = "{";
	response += "\"host\":";
	response += "\"";
	response += config.hostname;
	response += "\"";
	response += ",";
	response += "\"port\":";
	response += config.port;
	response += "}";
	msSystem.msESPServer.send(200, "text/plain", response);
}

void handlePOSTServerSettings(void)
{
	msSystem.slogln("handlePOSTServerSettings");
	if (msSystem.msESPServer.args() >= 2) {
		ServerConfig config;
		Settings.getServerConfig(&config);

		bool success = true;
		for (int i = 0; i < msSystem.msESPServer.args(); i++) {
			msSystem.slogln("argName: ");
			msSystem.slogln(msSystem.msESPServer.argName(i));

			msSystem.slogln("arg: ");
			msSystem.slogln(msSystem.msESPServer.arg(i));

			if (strcmp(msSystem.msESPServer.argName(i).c_str(), "host") ==
				0) {
				l_safeStrncpy(config.hostname,
							  msSystem.msESPServer.arg(i).c_str(),
							  sizeof(config.hostname));
			} else
				if (strcmp(msSystem.msESPServer.argName(i).c_str(), "port")
					== 0) {
				config.port = atoi(msSystem.msESPServer.arg(i).c_str());
			} else {
				success = false;
			}
		}

		if (success) {
			Settings.setServerConfig(&config);
			msSystem.msESPServer.send(200, "text/plain", "OK");
		} else {
			msSystem.msESPServer.send(500, "text/plain", "invalid args!");
		}
	} else {
		msSystem.msESPServer.send(500, "text/plain", "argument missing!");
	}
}

void handleGETAPSettings(void)
{
	msSystem.slogln("handleGETAPSettings");

	APInfo apInfo;
	Settings.getAPConfig(&apInfo);

	String response = "{";
	response += "\"ssid\":";
	response += "\"";
	response += apInfo.ssid;
	response += "\"";
	//response += ",";
	//response += "\"pwd\":";
	//response += "\"" + apInfo.pwd + "\"";
	response += "}";
	msSystem.msESPServer.send(200, "text/plain", response);
}

void handlePOSTAPSettings(void)
{
	msSystem.slogln("handlePOSTAPSettings");

	if (msSystem.msESPServer.args() >= 2) {
		// load old settings
		APInfo apInfo;
		Settings.getAPConfig(&apInfo);

		if (parseAPInfoFromServerArgs(apInfo)) {
			msSystem.slogln("saving setAPConfig");
			Settings.setAPConfig(&apInfo);
			msSystem.msESPServer.send(200, "text/plain", "OK");
		} else {
			msSystem.msESPServer.send(500, "text/plain", "unknown args!");
		}
	} else {
		msSystem.msESPServer.send(500, "text/plain", "argument missing!");
	}
}


void handleGETSysLogHostSettings(void)
{
	msSystem.slogln("handleGETSysLogHostSettings");

	ServerConfig sysLogInfo;
	Settings.getSyslogConfig(&sysLogInfo);

	String response = "{";
	response += "\"syslog_host\":";
	response += "\"";
	response += sysLogInfo.hostname;
	response += "\"";
	response += "}";
	msSystem.msESPServer.send(200, "text/plain", response);
}

void handlePOSTSysLogSettings(void)
{
	msSystem.slogln("handlePOSTSysLogSettings");

	if (msSystem.msESPServer.args() >= 2) {
		// load old settings
		ServerConfig sysLogInfo;
		Settings.getSyslogConfig(&sysLogInfo);

		if (parseSysLogInfoFromServerArgs(sysLogInfo)) {
			msSystem.slogln("saving setSysLogConfig");
			Settings.setSyslogConfig(&sysLogInfo);
			msSystem.msESPServer.send(200, "text/plain", "OK");
		} else {
			msSystem.msESPServer.send(500, "text/plain", "unknown args!");
		}
	} else {
		msSystem.msESPServer.send(500, "text/plain", "argument missing!");
	}
}



void handleGETPreferredAPSettings(void)
{
	msSystem.slogln("handleGETPreferredAPSettings");

	APInfo apInfo;
	Settings.getPreferredAP(&apInfo);

	String response = "{";
	response += "\"ssid\":";
	response += "\"";
	response += apInfo.ssid;
	response += "\"";
	//response += ",";
	//response += "\"pwd\":";
	//response += "\"" + apInfo.pwd + "\"";
	response += "}";
	msSystem.msESPServer.send(200, "text/plain", response);
}

void handlePOSTPreferredAPSettings(void)
{
	msSystem.slogln("handlePOSTPreferredAPSettings");

	if (msSystem.msESPServer.args() >= 2) {
		APInfo apInfo;
		Settings.getPreferredAP(&apInfo);

		if (parseAPInfoFromServerArgs(apInfo)) {
			msSystem.slogln("saving setAPConfig");
			Settings.setPreferredAP(&apInfo);
			msSystem.msESPServer.send(200, "text/plain", "OK");
		} else {
			msSystem.msESPServer.send(500, "text/plain", "unknown args!");
		}
	} else {
		msSystem.msESPServer.send(500, "text/plain", "argument missing!");
	}
}


///////////////////////////////////////
//////////////////////////////////////////

void handleGETAPList(void)
{
	msSystem.slogln("handleGETAPList");

	APInfo apInfo;
	Settings.getPreferredAP(&apInfo);

	String response = "[";

	Settings.resetAPList();
	bool firstAP = true;
	while (Settings.getNextAP(&apInfo)) {
		if (!firstAP) {
			response += ",";
		}
		firstAP = false;

		response += "{\"ssid\":";
		response += "\"";
		response += apInfo.ssid;
		response += "\"";
		//response += ",";
		//response += "\"pwd\":";
		//response += "\"" + apInfo.pwd + "\"";
		response += "}";
	}
	response += "]";
	Settings.resetAPList();

	msSystem.msESPServer.send(200, "text/plain", response);
}

void handleGETWLANList(void)
{
	msSystem.slogln("handleGETWLANList");

	// if (msGlobals.ggModeAP)
	// {
	//   msSystem.msESPServer.send(200, "text/plain", "crash in AP mode...so diusabled for now");
	//   return;
	// }

	String response = "[";

	int n = WiFi.scanNetworks();
	msSystem.slogln("scan done");
	if (n == 0) {
		msSystem.slogln("\"no networks found\"");
	} else {
		//msSystem.slogln(n);
		msSystem.slogln(" networks found");
		bool firstAP = true;
		for (int i = 0; i < n; ++i) {
			if (!firstAP) {
				response += ",";
			}
			firstAP = false;

			response += "{\"ssid\":";
			response += "\"";
			response += WiFi.SSID(i);
			response += "\"";
			response += ",";
			response += "\"rssi\":";
			response += WiFi.RSSI(i);
			response += ",";
			response += "\"free\":";
			response +=
				(WiFi.encryptionType(i) ==
				 ENC_TYPE_NONE) ? "true" : "false";
			response += "}";
		}
	}


	response += "]";
	Settings.resetAPList();

	msSystem.msESPServer.send(200, "text/plain", response);
}

void handlePOSTAPListAdd(void)
{
	msSystem.slogln("handlePOSTAPListAdd");

	if (msSystem.msESPServer.args() >= 2) {
		APInfo apInfo;
		memset(apInfo.ssid, 0, sizeof(apInfo.ssid));
		memset(apInfo.password, 0, sizeof(apInfo.password));

		if (parseAPInfoFromServerArgs(apInfo)) {
			if (!strcmp(apInfo.ssid, "") == 0) {
				msSystem.slogln("adding wifi");
				Settings.addAP(&apInfo);
			}
			msSystem.msESPServer.send(200, "text/plain", "OK");
		} else {
			msSystem.msESPServer.send(500, "text/plain", "invalid args!");
		}
	} else {
		msSystem.msESPServer.send(500, "text/plain", "args missing!");
	}
}

void handlePOSTAPListDelete(void)
{
	msSystem.slogln("handlePOSTAPListDelete");

	if (msSystem.msESPServer.args() >= 1) {
		APInfo apInfo;
		strcpy(apInfo.ssid, "");
		strcpy(apInfo.password, "");

		if (parseAPInfoFromServerArgs(apInfo)) {
			if (!strcmp(apInfo.ssid, "") == 0) {
				msSystem.slogln("deleting wifi");
				Settings.deleteAP(apInfo.ssid);
			}
			msSystem.msESPServer.send(200, "text/plain", "OK");
		} else {
			msSystem.msESPServer.send(500, "text/plain", "invalid args!");
		}
	} else {
		msSystem.msESPServer.send(500, "text/plain", "args missing!");
	}
}


void handleSetMode(void)
{
	msSystem.slogln("handleSetMode");
	if (msSystem.msESPServer.args() == 1) {
		ServerConfig config;
		Settings.getServerConfig(&config);

		bool success = true;
		for (int i = 0; i < msSystem.msESPServer.args(); i++) {
			msSystem.slogln("argName: ");
			msSystem.slogln(msSystem.msESPServer.argName(i));

			msSystem.slogln("arg: ");
			msSystem.slogln(msSystem.msESPServer.arg(i));

			msGlobals.ggCurrentMode =
				atoi(msSystem.msESPServer.arg(i).c_str());
		}

		if (success) {
			String response = "{";
			response += "\"mode\":";
			response += "\"";
			response += msGlobals.ggCurrentMode;
			response += "\"";
			response += "}";
			msSystem.msESPServer.send(200, "text/plain", response);
		} else {
			msSystem.msESPServer.send(500, "text/plain", "invalid args!");
		}
	} else {
		msSystem.msESPServer.send(500, "text/plain", "argument missing!");
	}
}

void respondREQTime()
{
	int currentTime =
		msGlobals.ggTime + (millis() - msGlobals.ggTimePostedAt);

	String response = "{";
	response += "\"time\":" + String(currentTime) + ",";
	response += "\"postedAt\":" + String(msGlobals.ggTimePostedAt);
	response += "}";

	msSystem.msESPServer.send(200, "text/plain", response);
}

void handleGETTime()
{
	msSystem.slogln("handleGETTime");

	respondREQTime();
}

void handlePOSTTime()
{
	msSystem.slogln("handlePOSTTime");

	if (msSystem.msESPServer.argName(0) == "t") {
		msGlobals.ggTime = atoi(msSystem.msESPServer.arg(0).c_str());
		msGlobals.ggTimePostedAt = millis();
	}

	respondREQTime();
}

void handleLedSet()
{
	byte ledData[MAX_LEDS * 5];

	String message = "LedSet\n\n";

	if (msSystem.msESPServer.args() == 1) {
		const char *input = msSystem.msESPServer.arg(0).c_str();
		unsigned int inputLen = (int) msSystem.msESPServer.arg(0).length();
		msSystem.slogln("inputLen: ");
		msSystem.slogln(String(inputLen));

		if (inputLen > sizeof(ledData))
			inputLen = sizeof(ledData);

		msSystem.slogln(String(inputLen));

		unsigned int dataLen =
			base64_decode((char *) ledData, input, inputLen);

		for (int i = 0; i < dataLen; i += 5) {
			//msSystem.msLEDs.setLED(ledData[i], ledData[i+1], ledData[i+2], ledData[i+3], ledData[i+4]);
			byte idx = ledData[i];
			msSystem.slogln("idx: ");
			msSystem.slogln(String((int) idx));
			if (idx >= MAX_LEDS)
				continue;
			msSystem.slogln("data+1: ");
			msSystem.slogln(String((int) ledData[i + 1]));

			msGlobals.ggRGBLEDBuf[idx * 4] = ledData[i + 1];
			msGlobals.ggRGBLEDBuf[idx * 4 + 1] = ledData[i + 2];
			msGlobals.ggRGBLEDBuf[idx * 4 + 2] = ledData[i + 3];
			msGlobals.ggRGBLEDBuf[idx * 4 + 3] = ledData[i + 4];
		}
	} else {
		message += "argument missing or too many arguments!";
	}
	msSystem.msESPServer.send(200, "text/plain", message);
}


void handleLedsSet()
{
	msSystem.slogln("handleLedsSet");

	String message = "LedsSet\n\n";

	if (msSystem.msESPServer.args() >= 1) {
		const char *input = msSystem.msESPServer.arg(0).c_str();
		int inputLen = BASE64_ENC_LEN(RGB_BUFFER_SIZE);

		base64_decode((char *) msGlobals.ggRGBLEDBuf, input, inputLen);

		message += "done";
	} else {
		message += "argument missing!";
	}
	msSystem.msESPServer.send(200, "text/plain", message);
}


/*
struct UIConfig
{
  int powerdownTimeUSB;
  int powerdownTimeBattery;
  int defaultBrightness;
};
*/

// void handleGetUISettings(void)
// {
//   msSystem.slogln("handleGetUISettings");

//   UIConfig config;
//   Settings.getServerConfig(&config);

//   String response = "{";
//     response += "\"powerdownTimeUSB\":";
//     response += config.powerdownTimeUSB;
//     response += ",";
//     response += "\"powerdownTimeBattery\":";
//     response += config.powerdownTimeBattery;
//     response += ",";
//     response += "\"defaultBrightness\":";
//     response += config.defaultBrightness;
//   response += "}";
//   msSystem.msESPServer.send(200, "text/plain", response);
// }

// void handleSetUISettings(void)
// {
//   msSystem.slogln("handleSetUISettings");
//   if (msSystem.msESPServer.args() >= 1)
//   {
//     ServerConfig config;
//     Settings.getServerConfig(&config);

//     bool success = true;
//     for (int i = 0; i < msSystem.msESPServer.args(); i++)
//     {
//       msSystem.slogln("argName: ");
//       msSystem.slogln(msSystem.msESPServer.argName(i));

//       msSystem.slogln("arg: ");
//       msSystem.slogln(msSystem.msESPServer.arg(i));

//       if (strcmp(msSystem.msESPServer.argName(i).c_str(), "powerdownTimeUSB") == 0)
//       {
//         config.powerdownTimeUSB = atoi(msSystem.msESPServer.arg(i).c_str());
//       }
//       else if (strcmp(msSystem.msESPServer.argName(i).c_str(), "powerdownTimeBattery") == 0)
//       {
//         config.powerdownTimeBattery = atoi(msSystem.msESPServer.arg(i).c_str());
//       }
//       else if (strcmp(msSystem.msESPServer.argName(i).c_str(), "defaultBrightness") == 0)
//       {
//         config.defaultBrightness = atoi(msSystem.msESPServer.arg(i).c_str());
//       }
//       else
//       {
//         success = false;
//       }
//     }

//     if (success)
//     {
//       Settings.setServerConfig(&config);
//       msSystem.msESPServer.send (200, "text/plain", "OK");
//     }
//     else
//     {
//       msSystem.msESPServer.send(500, "text/plain", "invalid args!");
//     }
//   }
//   else
//   {
//     msSystem.msESPServer.send ( 500, "text/plain", "argument(s) missing!");
//   }
// }

#endif
