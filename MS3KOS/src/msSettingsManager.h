#ifndef _MSSETTINGSMANAGER_H
#define _MSSETTINGSMANAGER_H

class MagicShifterSystem;
extern MagicShifterSystem msSystem;


class SettingsManager {

private:
	// used in resetAPList & getNextAP
	int apListIndex = -1;
	File smAPListFile;

private:

	bool loadData(String path, void *config, int len) {
		if (SPIFFS.exists((char *) path.c_str())) {
			File file = SPIFFS.open((char *) path.c_str(), "r");
			file.read((uint8_t *) config, len);
			file.close();

			return true;
		} else {
			msSystem.slog("webserver: loadData: can not open config file ");
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

	String getUniqueSystemName() 
	{
		uint8_t mac[WL_MAC_ADDR_LENGTH];
		WiFi.softAPmacAddress(mac);
		String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) + String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);

		macID.toUpperCase();
		String UniqueSystemName = String(AP_NAME_OVERRIDE) + String("-") + macID;

		return UniqueSystemName;
	}


public:

	const String apConfigPath = "settings/ap.bin";
	const String apServerConfigPath = "settings/server1.bin";
	const String apListConfigPath = "settings/aplist1.bin";
	const String apSysLogConfigPath = "settings/syslog.bin";
	const String preferredAPConfigPath = "settings/preferredap.bin";
	const String uiSettingsConfigPath = "settings/ui.bin";

	bool getUIConfig(struct UIConfig *config) {
		//msSystem.slog("config: sizeof ");
		//msSystem.slogln(String(sizeof(*config)));
		bool result = loadData(uiSettingsConfigPath, config, sizeof(*config));
		if (!result) {
			config->timeoutHighPower = 0;
			config->timeoutLowPower =  10 * 60 * 1000; // 10 minutes
			config->defaultBrightness = 2;
		}
		return result;
	}

	bool setUIConfig(struct UIConfig *config) {
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

	bool getAPConfig(struct APAuth *config) {

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

		// don't have a config file, so we return a default
		l_safeStrncpy(config->ssid, getUniqueSystemName().c_str(),
			sizeof(config->ssid));
		l_safeStrncpy(config->password, "", sizeof(config->password));

		return false;
	}

	void setAPConfig(struct APAuth *config) {

		String path = apConfigPath;
		File file = SPIFFS.open((char *) path.c_str(), "w");
		file.write((uint8_t *) config, sizeof(*config));
		file.close();

		msSystem.slogln("webserver: saved:");
		msSystem.slogln(config->ssid);

	}

	bool getPreferredAP(struct APAuth *config) {

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

	void setPreferredAP(struct APAuth *config) {

		String path = preferredAPConfigPath;
		File file = SPIFFS.open((char *) path.c_str(), "w");
		file.write((uint8_t *) config, sizeof(*config));
		file.close();

	}

	//
	// delete an AP Auth structure from the list
	// if its not our ssid, add it to the list,
	// otherwise ignore it 
	// and save the list again
	//
	void deleteAP(char *ssid) {
		String path = apListConfigPath;

		typedef std::map<String, String> AuthItems;
		typedef std::map<String, String>::iterator AuthItems_it;
		AuthItems authItems;

		File inFile;
		APAuth inAPAuth;

		// open the existing AP settings file if we can, and construct the map
		if (SPIFFS.exists((char *)path.c_str()))  {
			inFile = SPIFFS.open((char *) path.c_str(), "r");
		}
		if (inFile) {
			msSystem.slog("webserver: opened AP config file:");
			msSystem.slogln(path);

			// create a list of AP entries already in the file
			while (inFile.read((uint8_t *) &inAPAuth, sizeof (APAuth)) == sizeof(APAuth)) {
				// exclude the one we want to delete
				if (strncmp(inAPAuth.ssid, ssid, MAX_AP_LEN) == 0)
					continue;
				else
					authItems[inAPAuth.ssid] = inAPAuth.password;
			}

			inFile.close();

		} else {
			msSystem.slog("webserver: couldn't open AP inFile:");
			msSystem.slogln(path);
		}

		// dump the map back to the file
		File outFile = SPIFFS.open((char *) path.c_str(), "w+");
		if (outFile) {
			AuthItems_it it;
			for(it = authItems.begin(); it != authItems.end(); it++)
			{
				APAuth t_Auth;
				l_safeStrncpy(t_Auth.ssid, it->first.c_str(), MAX_AP_LEN);
				l_safeStrncpy(t_Auth.password, it->second.c_str(), MAX_AP_LEN);
				outFile.write((uint8_t *)&t_Auth, sizeof(APAuth));
			}

			outFile.close();
			msSystem.slog("webserver: saved AP configuration");
			msSystem.slogln(path);
		} else {
			msSystem.slog("webserver: couldn't save AP outFile:");
			msSystem.slogln(path);
		}

	}

	//
	// add an AP Auth structure to the list
	// if its there already, update the password
	// if its not there, add it to the list
	//
	void addAP(struct APAuth *apInfo) {
		String path = apListConfigPath;

		typedef std::map<String, String> AuthItems;
		typedef std::map<String, String>::iterator AuthItems_it;
		AuthItems authItems;

		File inFile;
		APAuth inAPAuth;

		// open the existing AP settings file if we can, and construct the map
		if (SPIFFS.exists((char *)path.c_str()))  {
			inFile = SPIFFS.open((char *) path.c_str(), "r");
		}
		if (inFile) {
			msSystem.slog("webserver: opened AP config file:");
			msSystem.slogln(path);

			// create a list of AP entries already in the file
			while (inFile.read((uint8_t *) &inAPAuth, sizeof (APAuth)) == sizeof(APAuth)) {
				authItems[inAPAuth.ssid] = inAPAuth.password;
			}

			inFile.close();

		} else {
			msSystem.slog("webserver: couldn't open AP inFile:");
			msSystem.slogln(path);
		}

		// check if there is an entry in the map for the incoming apInfo
		// (map could also be empty)
		auto existingAP = authItems.find(apInfo->ssid);

		if (existingAP != authItems.end()) {
	    	// we found it, so set the new password
			existingAP->second = apInfo->password;
	    } else { 	// .. otherwise, add one and set the password
	    	authItems[apInfo->ssid] = apInfo->password;
	    }

		// dump the map back to the file
	    File outFile = SPIFFS.open((char *) path.c_str(), "w+");
	    if (outFile) {
	    	AuthItems_it it;
	    	for(it = authItems.begin(); it != authItems.end(); it++)
	    	{
	    		APAuth t_Auth;
	    		l_safeStrncpy(t_Auth.ssid, it->first.c_str(), MAX_AP_LEN);
	    		l_safeStrncpy(t_Auth.password, it->second.c_str(), MAX_AP_LEN);
	    		outFile.write((uint8_t *)&t_Auth, sizeof(APAuth));
	    	}

	    	outFile.close();
	    	msSystem.slog("webserver: saved AP configuration");
	    	msSystem.slogln(path);
	    } else {
	    	msSystem.slog("webserver: couldn't save AP outFile:");
	    	msSystem.slogln(path);
	    }

	}

	void resetAPList() {
		apListIndex = -1;
		smAPListFile.close();

	}

	bool getNextAP(struct APAuth *apInfo) {
		if (apListIndex < 0) {
			String path = apListConfigPath;
			if (SPIFFS.exists((char *) path.c_str())) {
				smAPListFile = SPIFFS.open((char *) path.c_str(), "r");
				apListIndex = 0;
			}
		}

		if (apListIndex >= 0) {
			const int requiredBytes = sizeof(*apInfo);
			do {
				if (smAPListFile.read((uint8_t *) apInfo, requiredBytes) ==
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

#endif
