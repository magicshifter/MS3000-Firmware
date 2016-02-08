#ifndef _WEBSERVERAUTOCONNECT_H
#define _WEBSERVERAUTOCONNECT_H

#include "msAPInfo.h"

void printIPInfo(){
  msSystem.slogln ( "wifi: IP address: " );
  msSystem.slogln ( String(WiFi.localIP().toString()) );
  msSystem.slogln ( "wifi: soft AP IP: " );
  msSystem.slogln ( String(WiFi.softAPIP().toString()) );

  WiFi.printDiag(Serial);
}

bool TryConnect(struct APInfo &apInfo, int timeoutMs)
{
  msSystem.slog("wifi: trying to connect to AP: ");
  msSystem.slogln(apInfo.ssid);
  WiFi.begin (apInfo.ssid, apInfo.password, 9);
  msSystem.slog("wifi: using password: ");
  msSystem.slogln(apInfo.password);

  // Wait for connection
  int frame = 0;
  long startTime = millis();

  while ( WiFi.status() != WL_CONNECTED ) {
    for (int i = 0; i < MAX_LEDS; i++)
    {
      if (i < frame % MAX_LEDS)
        msSystem.msLEDs.setLED(i, 5, 5, 5, 0x10);
      else
        msSystem.msLEDs.setLED(i, 0, 0, 0, 0);
    }
    msSystem.msLEDs.updateLEDs();
    frame++;

    if (frame%50 == 0) msSystem.slogln(".");
    
    if (WiFi.status() == WL_NO_SSID_AVAIL || WiFi.status() == WL_CONNECT_FAILED || millis() > startTime + timeoutMs)
    {
      msSystem.slog("wifi: could not connect to:");
      msSystem.slogln(apInfo.ssid);
      return false; // :(
    }

    delay(100);
  }

  msSystem.slogln ( "wifi: connected to: " );
  msSystem.slogln ( apInfo.ssid );

  printIPInfo();

  return true;
}



bool TrySoftAP(struct APInfo &apInfo)
{
  msSystem.slogln("wifi: configuring access point: ");
  msSystem.slogln(apInfo.ssid);
  msSystem.slogln("wifi: password: ");
  msSystem.slogln(apInfo.password);

  WiFi.mode(WIFI_AP_STA);
  
//  WiFi.softAPConfig(IPAddress(10,20,30,40), IPAddress(10,1,1,1), IPAddress(255, 255, 255, 0));
  if (strlen(apInfo.password) == 0)
  {
    WiFi.softAP(apInfo.ssid);
  }
  else
  {
    WiFi.softAP(apInfo.ssid, apInfo.password);
  }

  msSystem.slogln ( "wifi: accesspoints: " );
  msSystem.slogln ( apInfo.ssid );

  printIPInfo();

  return true;
}

bool AutoConnect()
{
  struct jsonparse_state jsonState;

#ifdef SCAN_FIRST_MODE
  // if (!forceAPMode)
  {
    if (Settings.getPreferredAP(&msGlobals.ggAPConfig.apInfo))
    {
      msSystem.slogln("wifi: using stored configuration.");
      if (TryConnect(msGlobals.ggAPConfig.apInfo, CONNECTION_TIMEOUT))
      {
        msGlobals.ggModeAP = false;
        return true;
      }
    }
    else
      msSystem.slogln("wifi: no preferred configuration found.");


    if (SPIFFS.exists(Settings.apListConfigPath)) { 

      msSystem.slog("wifi: start scan -");
      // WiFi.scanNetworks will return the number of networks found
      int n = WiFi.scanNetworks();
      msSystem.slogln("done");
      if (n == 0) {
        msSystem.slogln("wifi: no networks found");
      }
      else
      {
        msSystem.slogln("wifi: networks found: ");
        msSystem.slog(String(n));

        for (int i = 0; i < n; ++i)
        {
          // Print SSID and RSSI for each network found
          msSystem.slog(String(i + 1));
          msSystem.slog(": ");
          msSystem.slog(WiFi.SSID(i));
          msSystem.slog(" (");
          msSystem.slog(String(WiFi.RSSI(i)));
          msSystem.slog(")");
          msSystem.slog((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
          delay(20);
        }
      }

      msSystem.slogln("");

      Settings.resetAPList();

      while (Settings.getNextAP(&msGlobals.ggAPConfig.apInfo))
      {
        for (int i = 0; i < n; i++)
        {
          if (strcmp(WiFi.SSID(i).c_str(), msGlobals.ggAPConfig.apInfo.ssid) == 0)
          {
            if (TryConnect(msGlobals.ggAPConfig.apInfo, CONNECTION_TIMEOUT))
            {
              msGlobals.ggModeAP = false;
              msSystem.feedbackAnimation(msGlobals.feedbackType::OK);
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

  Settings.getAPConfig(&msGlobals.ggAPConfig.apInfo);
  if (TrySoftAP(msGlobals.ggAPConfig.apInfo))
  {
    msGlobals.ggModeAP = true;
    return true;
  }

  msSystem.slogln("wifi: ERROR: autoConnect failed!");
  return false;
}

#endif
