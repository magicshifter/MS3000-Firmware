#ifndef _WEBSERVERAUTOCONNECT_H
#define _WEBSERVERAUTOCONNECT_H

bool TryConnect(struct APInfo &apInfo, int timeoutMs)
{
  msSystem.log("trying to connect to AP: ");
  msSystem.logln(apInfo.ssid);
  WiFi.begin (apInfo.ssid, apInfo.password );

  // Wait for connection
  int frame = 0;
  long startTime = millis();

  while ( WiFi.status() != WL_CONNECTED ) {
    for (int i = 0; i < MAX_LEDS; i++)
    {
      if (i < frame % MAX_LEDS)
        msSystem.msLEDs.setPixels(i, 5, 5, 5, 0x10);
      else
        msSystem.msLEDs.setPixels(i, 0, 0, 0, 0);
    }
    msSystem.msLEDs.updatePixels();
    frame++;
    if (frame%50 == 0) msSystem.logln(".");
    if (WiFi.status() == WL_NO_SSID_AVAIL || WiFi.status() == WL_CONNECT_FAILED || millis() > startTime + timeoutMs)
    {
      msSystem.logln ( "" );
      msSystem.logln ("Could NOT connect to:");
      msSystem.logln(apInfo.ssid);
      return false; // :(
    }

    // crashes!
    msSystem.loop();
    
    delay(20);
  }

  msSystem.logln ( "" );
  msSystem.logln ( "Connected to: " );
  msSystem.logln ( apInfo.ssid );
  msSystem.logln ( "IP address: " );
  msSystem.logln ( String(WiFi.localIP()) );

  return true;
}



bool TrySoftAP(struct APInfo &apInfo)
{
  msSystem.logln("Configuring access point: ");
  msSystem.logln(apInfo.ssid);
  msSystem.logln(" password: ");
  msSystem.logln(apInfo.password);

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

  msSystem.logln ( "Established acces point: " );
  msSystem.logln ( apInfo.ssid );
  msSystem.logln("SoftAP IP: ");
  msSystem.logln(String(WiFi.softAPIP()));

  return true;
}

bool AutoConnect()
{
  struct jsonparse_state jsonState;
  struct APInfo apInfo;

#ifndef FORCE_AP_MODE
  // if (!forceAPMode)
  {
    if (Settings.getPreferredAP(&apInfo))
    {
      msSystem.logln("stored preferred wifi found.");
      if (TryConnect(apInfo, CONNECTION_TIMEOUT))
      {
        msGlobals.ggModeAP = false;
        return true;
      }
    }

    msSystem.logln("WiFi AP scan starting...");
    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    msSystem.logln("scan done");
    if (n == 0) {
      msSystem.logln("no networks found");
    }
    else
    {
      msSystem.logln(String(n));
      msSystem.logln(" networks found");
      for (int i = 0; i < n; ++i)
      {
        // Print SSID and RSSI for each network found
        msSystem.logln(String(i + 1));
        msSystem.logln(": ");
        msSystem.logln(WiFi.SSID(i));
        msSystem.logln(" (");
        msSystem.logln(String(WiFi.RSSI(i)));
        msSystem.logln(")");
        msSystem.logln((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
        delay(20);
      }
    }
    msSystem.logln("");

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

    msSystem.logln("None of the stored WiFi APs was found!");
    false;
  }
#endif

  msSystem.logln("fallback to standalone access point...");

  Settings.getAPConfig(&apInfo);
  if (TrySoftAP(apInfo))
  {
    msGlobals.ggModeAP = true;
    return true;
  }

  msSystem.logln("ERROR: AutoConnect failed!");
  return false;
}

#endif