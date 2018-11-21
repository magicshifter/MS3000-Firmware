#ifndef _WEBSERVERAUTOCONNECT_H
#define _WEBSERVERAUTOCONNECT_H

void printIPInfo()
{
	msSystem.slogln("wifi: IP address: " + WiFi.localIP().toString());
	msSystem.slogln("wifi: soft AP IP: " + WiFi.softAPIP().toString());

	// WiFi.printDiag(Serial);
}

void WifiOnStationModeConnected(const WiFiEventStationModeConnected WifiEvent)
{
	msSystem.slogln("WiFi STA connect");
}

void WifiOnStationModeDisconnected(const WiFiEventStationModeDisconnected WifiEvent)
{
	msSystem.slogln("WiFi STA disconnect");
}

void WifiOnStationModeAuthModeChanged(const WiFiEventStationModeAuthModeChanged WifiEvent)
{
	msSystem.slogln("WiFi AUTH MODE Change");
}

void WifiOnStationModeGotIP(const WiFiEventStationModeGotIP WifiEvent)
{
	msSystem.slog("WiFi Got IP Address:");
	msSystem.slogln(WiFi.localIP().toString());
}

void WifiOnStationModeDHCPTimeout()
{
	msSystem.slogln("WiFi DHCP Timeout");
}

void WifiOnSoftAPModeStationConnected(const WiFiEventSoftAPModeStationConnected WifiEvent)
{
	msSystem.slogln("WiFi Soft AP STA connect");
}

void WifiOnSoftAPModeStationDisconnected(const WiFiEventSoftAPModeStationDisconnected WifiEvent)
{
	msSystem.slogln("WiFi Soft AP STA disconnect");
}

// deprecated
void WiFiEvent(WiFiEvent_t event) {
    // msSystem.slogf("[WiFi-event] event: %d\n", event);

    switch(event) {
        case WIFI_EVENT_STAMODE_GOT_IP:
            msSystem.slogln("WiFi connected - IP address: " + WiFi.localIP().toString());
            break;
        case WIFI_EVENT_STAMODE_DISCONNECTED:
            msSystem.slogln("WiFi lost connection");
            break;
		case WIFI_EVENT_STAMODE_CONNECTED:
            msSystem.slogln("WiFi STA connected");
		break;
		case WIFI_EVENT_STAMODE_AUTHMODE_CHANGE:
            msSystem.slogln("WiFi AUTH change");
		break;
		case WIFI_EVENT_STAMODE_DHCP_TIMEOUT:
            msSystem.slogln("WiFi DHCP Timeout");
		break;
		case WIFI_EVENT_SOFTAPMODE_STACONNECTED:
            msSystem.slogln("WiFi SoftAP STA connected");
		break;
		case WIFI_EVENT_SOFTAPMODE_STADISCONNECTED:
            msSystem.slogln("WiFi SoftAP STA disconnected");
		break;
		case WIFI_EVENT_SOFTAPMODE_PROBEREQRECVED:
            msSystem.slogln("WiFi PROBE Request recv'd");
		break;
		case WIFI_EVENT_MAX:
            msSystem.slogln("WiFi EVENT MAX");
		break;
        default:
            msSystem.slogln("WiFi something else");
        break;
    }
}


void setDebugCallbacks()
{

	// !J! deprecated
#if 0
    WiFi.onEvent(WiFiEvent); // deprecated
#endif

	WiFi.onStationModeConnected(WifiOnStationModeConnected);
	WiFi.onStationModeDisconnected(WifiOnStationModeDisconnected);
	WiFi.onStationModeAuthModeChanged(WifiOnStationModeAuthModeChanged);
	WiFi.onStationModeGotIP(WifiOnStationModeGotIP);
	//WiFi.onStationModeDHCPTimeout(WifiOnStationModeDHCPTimeout);	// ?? !J!
	WiFi.onSoftAPModeStationConnected(WifiOnSoftAPModeStationConnected);
	WiFi.onSoftAPModeStationDisconnected(WifiOnSoftAPModeStationDisconnected);

}


bool TryConnect(struct APAuth &apInfo, uint32_t timeoutMs)
{
	msSystem.slog("wifi: trying to connect to AP: " + String(apInfo.ssid));

	WiFi.begin(apInfo.ssid, apInfo.password, 9);
	msSystem.slog("wifi: using password: " + String(apInfo.password));

	// Wait for connection
	int frame = 0;
	uint32_t startTime = millis();
	bool connected = false;
	String failReason;

	while (WiFi.status() != WL_CONNECTED) {
		for (int i = 0; i < MAX_LEDS; i++) {
			if (i < frame % MAX_LEDS)
				msSystem.msLEDs.setLED(i, 5, 5, 5, 0x10);
			else
				msSystem.msLEDs.setLED(i, 0, 0, 0, 0);
		}
		msSystem.msLEDs.updateLEDs();
		frame++;

		if (frame % 50 == 0)
			msSystem.slogln(".");

		if (WiFi.status() == WL_NO_SSID_AVAIL) {
			connected = false;
			failReason = "SSID Unavailable";
		}
		else
		if (WiFi.status() == WL_CONNECT_FAILED) {
			connected = false;
			failReason = "Connection Failed";
		}
		else
		if (millis() > startTime + timeoutMs) {
			connected = false;			
			failReason = "Connection Timed-out";
		}
		else 
			connected = true;

		if (!connected) { 
			msSystem.slogln("wifi: could not connect to: " + String(apInfo.ssid));
			msSystem.slogln("wifi: reason - " + failReason );

			return false;		// :(
		}

		delay(100);
	}

	msSystem.slogln("wifi: connected to: " + String(apInfo.ssid));

	printIPInfo();

	return true;
}



bool TrySoftAP(struct APAuth & apInfo)
{
	msSystem.slogln("wifi: configuring access point: "+ String(apInfo.ssid));
	msSystem.slogln("wifi: password: " + String(apInfo.password));

	WiFi.mode(WIFI_AP_STA);

//  WiFi.softAPConfig(IPAddress(10,20,30,40), IPAddress(10,1,1,1), IPAddress(255, 255, 255, 0));
	if (strlen(apInfo.password) == 0) {
		WiFi.softAP(apInfo.ssid);
	} else {
		WiFi.softAP(apInfo.ssid, apInfo.password);
	}

	msSystem.slogln("wifi: accesspoints: " + String(apInfo.ssid));

	printIPInfo();

	return true;
}

bool AutoConnect()
{

	setDebugCallbacks();

#ifdef SCAN_FIRST_MODE
	{
		if (msSystem.Settings.getPreferredAP(&msGlobals.ggAPConfig.apInfo.auth)) {
			msSystem.slogln("wifi: using stored configuration.");
			
			if (TryConnect
				(msGlobals.ggAPConfig.apInfo.auth, CONNECTION_TIMEOUT)) {
				msGlobals.ggModeAP = false;
				return true;
			}

		} else

		msSystem.slogln("wifi: no preferred configuration found.");

		if (SPIFFS.exists(msSystem.Settings.apListConfigPath)) {

			msSystem.slogln("wifi: start scan -");
			// WiFi.scanNetworks will return the number of networks found
			
			int numNets = WiFi.scanNetworks();

			msSystem.slogln("wifi: scan done");

			if (numNets == 0) {
				msSystem.slogln("wifi: no networks found");
			} else {
				msSystem.slogln("wifi: networks found: " + String(numNets));

				for (int i = 0; i < numNets; ++i) {
					// Print SSID and RSSI for each network found
					msSystem.slog(String("  ") + 
							  String(i + 1) + 
							  String(": ") + 
							  WiFi.SSID(i) +
							  String(" (") + 
							  WiFi.RSSI(i) + 
							  String(") ") +
							  String((WiFi.encryptionType(i) ==
								         ENC_TYPE_NONE) ? "[open]" : "[encrypted]"));
					delay(20);
				}
			}

			msSystem.slogln("wifi: attempting connect");

			msSystem.Settings.resetAPList();

			while (msSystem.Settings.getNextAP(&msGlobals.ggAPConfig.apInfo.auth)) {
				for (int i = 0; i < numNets; i++) {
					if (strcmp
						(WiFi.SSID(i).c_str(),
							msGlobals.ggAPConfig.apInfo.auth.ssid) == 0) {
						
						if (TryConnect(msGlobals.ggAPConfig.apInfo.auth, CONNECTION_TIMEOUT)) {
							
							msGlobals.ggModeAP = false;
							msGlobals.ggSoftAP.clear();
							msSystem.feedbackAnimation(msGlobals.feedbackType::OK);
							
							return true;
						}
					}
				}
			}
		}

		msSystem.slogln("wifi: none of the configured networks found.");
		msSystem.feedbackAnimation(msGlobals.feedbackType::NOT_OK);

		// return false;
	}
#endif

	msSystem.slogln("wifi: fallback to standalone access point.");
	// WiFi.disconnect(false);

	APAuthHelper softAPInfo;

	msSystem.Settings.getAPConfig(&softAPInfo.auth);
	if (TrySoftAP(softAPInfo.auth)) {
		msGlobals.ggModeAP = true;
		msGlobals.ggSoftAP = softAPInfo;

		return true;
	}

	msSystem.slogln("wifi: ERROR: autoConnect failed!");

	return false;
	
}
#endif
