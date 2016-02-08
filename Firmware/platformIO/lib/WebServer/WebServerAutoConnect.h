#ifndef _WEBSERVERAUTOCONNECT_H
#define _WEBSERVERAUTOCONNECT_H

#include "msAPInfo.h"

void printIPInfo(){
  msSystem.slogln ( "IP address: " );
  msSystem.slogln ( String(WiFi.localIP().toString()) );
  msSystem.slogln ( "SoftAP IP: " );
  msSystem.slogln ( String(WiFi.softAPIP().toString()) );

  WiFi.printDiag(Serial);
}

bool TryConnect(struct APInfo &apInfo, int timeoutMs)
{
  msSystem.slog("trying to connect to AP: ");
  msSystem.slogln(apInfo.ssid);
  WiFi.begin (apInfo.ssid, apInfo.password, 9);
  msSystem.slog("using password: ");
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
      msSystem.slogln ( "" );
      msSystem.slogln ("Could NOT connect to:");
      msSystem.slogln(apInfo.ssid);
      return false; // :(
    }

    delay(20);
  }

  msSystem.slogln ( "" );
  msSystem.slogln ( "Connected to: " );
  msSystem.slogln ( apInfo.ssid );

  printIPInfo();

  return true;
}



bool TrySoftAP(struct APInfo &apInfo)
{
  msSystem.slogln("Configuring access point: ");
  msSystem.slogln(apInfo.ssid);
  msSystem.slogln(" password: ");
  msSystem.slogln(apInfo.password);

  WiFi.mode(WIFI_AP);
  
//  WiFi.softAPConfig(IPAddress(10,20,30,40), IPAddress(10,1,1,1), IPAddress(255, 255, 255, 0));
  if (strlen(apInfo.password) == 0)
  {
    WiFi.softAP(apInfo.ssid);
  }
  else
  {
    WiFi.softAP(apInfo.ssid, apInfo.password);
  }

  msSystem.slogln ( "accesspoints: " );
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
    if (Settings.getPreferredAP(&msGlobals.ggAPInfo))
    {
      msSystem.slogln("stored preferred wifi found.");
      if (TryConnect(msGlobals.ggAPInfo, CONNECTION_TIMEOUT))
      {
        msGlobals.ggModeAP = false;
        return true;
      }
    }

    msSystem.slog("wifi scan start:");
    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    msSystem.slogln("good");
    if (n == 0) {
      msSystem.slogln("no networks found");
    }
    else
    {
      msSystem.slog(String(n));
      msSystem.slogln(" networks found");
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

    while (Settings.getNextAP(&apInfo))
    {
      for (int i = 0; i < n; i++)
      {
        if (strcmp(WiFi.SSID(i).c_str(), apInfo.ssid) == 0)
        {
          if (TryConnect(apInfo, CONNECTION_TIMEOUT))
          {
            msGlobals.ggModeAP = false;
            return true;
          }
        }
      }
    }

    msSystem.slogln("None of the stored WiFi APs was found!");
    false;
  }
#endif


  msSystem.slogln("fallback to standalone access point...");
  // WiFi.disconnect(false);

  Settings.getAPConfig(&msGlobals.ggAPInfo);
  if (TrySoftAP(msGlobals.ggAPInfo))
  {
    msGlobals.ggModeAP = true;
    return true;
  }

  msSystem.slogln("ERROR: AutoConnect failed!");
  return false;
}

#endif
