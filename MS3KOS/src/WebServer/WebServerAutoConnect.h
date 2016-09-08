#ifndef _WEBSERVERAUTOCONNECT_H
#define _WEBSERVERAUTOCONNECT_H

void printIPInfo()
{
	msSystem.slogln("wifi: IP address: ");
	msSystem.slogln(String(WiFi.localIP().toString()));
	msSystem.slogln("wifi: soft AP IP: ");
	msSystem.slogln(String(WiFi.softAPIP().toString()));

	WiFi.printDiag(Serial);
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
            msSystem.slogln("WiFi connected");
            msSystem.slogln("IP address: ");
            msSystem.slogln(WiFi.localIP().toString());
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
    WiFi.onEvent(WiFiEvent); // deprecated

// non-deprecated, but doesn't seem to work..
#if 0
	WiFi.onStationModeConnected(WifiOnStationModeConnected);
	WiFi.onStationModeDisconnected(WifiOnStationModeDisconnected);
	WiFi.onStationModeAuthModeChanged(WifiOnStationModeAuthModeChanged);
	WiFi.onStationModeGotIP(WifiOnStationModeGotIP);
	WiFi.onStationModeDHCPTimeout(WifiOnStationModeDHCPTimeout);
	WiFi.onSoftAPModeStationConnected(WifiOnSoftAPModeStationConnected);
	WiFi.onSoftAPModeStationDisconnected(WifiOnSoftAPModeStationDisconnected);
#endif

}


bool TryConnect(struct APAuth &apInfo, int timeoutMs)
{
	msSystem.slog("wifi: trying to connect to AP: ");
	msSystem.slogln(apInfo.ssid);

	WiFi.begin(apInfo.ssid, apInfo.password, 9);
	msSystem.slog("wifi: using password: ");
	msSystem.slogln(apInfo.password);

	// Wait for connection
	int frame = 0;
	long startTime = millis();

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

		if (WiFi.status() == WL_NO_SSID_AVAIL
			|| WiFi.status() == WL_CONNECT_FAILED
			|| millis() > startTime + timeoutMs) {
			msSystem.slog("wifi: could not connect to:");
		msSystem.slogln(apInfo.ssid);
			return false;		// :(
		}

		delay(100);
	}

	msSystem.slogln("wifi: connected to: ");
	msSystem.slogln(apInfo.ssid);

	printIPInfo();

	return true;
}



bool TrySoftAP(struct APAuth & apInfo)
{
	msSystem.slogln("wifi: configuring access point: ");
	msSystem.slogln(apInfo.ssid);
	msSystem.slogln("wifi: password: ");
	msSystem.slogln(apInfo.password);

	WiFi.mode(WIFI_AP_STA);

//  WiFi.softAPConfig(IPAddress(10,20,30,40), IPAddress(10,1,1,1), IPAddress(255, 255, 255, 0));
	if (strlen(apInfo.password) == 0) {
		WiFi.softAP(apInfo.ssid);
	} else {
		WiFi.softAP(apInfo.ssid, apInfo.password);
	}

	msSystem.slogln("wifi: accesspoints: ");
	msSystem.slogln(apInfo.ssid);

	printIPInfo();

	return true;
}

bool AutoConnect()
{

	setDebugCallbacks();

#ifdef SCAN_FIRST_MODE
	// if (!forceAPMode)
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

		msSystem.slog("wifi: start scan -");
			// WiFi.scanNetworks will return the number of networks found
		int n = WiFi.scanNetworks();
		msSystem.slogln("done");
		if (n == 0) {
			msSystem.slogln("wifi: no networks found");
		} else {
			msSystem.slogln("wifi: networks found: ");
			msSystem.slog(String(n));

			for (int i = 0; i < n; ++i) {
					// Print SSID and RSSI for each network found
				msSystem.slog(String(i + 1));
				msSystem.slog(": ");
				msSystem.slog(WiFi.SSID(i));
				msSystem.slog(" (");
				msSystem.slog(String(WiFi.RSSI(i)));
				msSystem.slog(")");
				msSystem.
				slog((WiFi.encryptionType(i) ==
					ENC_TYPE_NONE) ? " " : "*");
				delay(20);
			}
		}

		msSystem.slogln("");

		msSystem.Settings.resetAPList();

		while (msSystem.Settings.getNextAP(&msGlobals.ggAPConfig.apInfo.auth)) {
			for (int i = 0; i < n; i++) {
				if (strcmp
					(WiFi.SSID(i).c_str(),
						msGlobals.ggAPConfig.apInfo.auth.ssid) == 0) {
					if (TryConnect
						(msGlobals.ggAPConfig.apInfo.auth,
							CONNECTION_TIMEOUT)) {
						msGlobals.ggModeAP = false;
					msGlobals.ggSoftAP.clear();

					msSystem.feedbackAnimation(msGlobals.
						feedbackType::OK);

					return true;
				}
			}
		}
	}
}

msSystem.slogln("wifi: none of the configured networks found.");
msSystem.feedbackAnimation(msGlobals.feedbackType::NOT_OK);
false;
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
