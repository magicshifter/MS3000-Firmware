#include "Config.h"

#include "MagicShifterGlobals.h"

extern MagicShifterGlobals msGlobals;

bool TryConnect(struct APInfo &apInfo, int timeoutMs)
{
  // msSystem.log("trying to connect to AP: ");
  // msSystem.logln(apInfo.ssid);
  WiFi.begin (apInfo.ssid, apInfo.password );

  // Wait for connection
  int frame = 0;
  long startTime = millis();

  while ( WiFi.status() != WL_CONNECTED ) {
    for (int i = 0; i < LEDS; i++)
    {
      if (i < frame % LEDS)
        setPixel(i, 5, 5, 5, 0x10);
      else
        setPixel(i, 0, 0, 0, 0);
    }
    updatePixels();
    frame++;
    if (frame%50 == 0) {
      // msSystem.log(".");
    }
    if (WiFi.status() == WL_NO_SSID_AVAIL || WiFi.status() == WL_CONNECT_FAILED || millis() > startTime + timeoutMs)
    {
      // msSystem.logln ( "" );
      // msSystem.log ("Could NOT connect to:");
      // msSystem.logln(apInfo.ssid);
      return false; // :(
    }

    // crashes!
    msSystem.loop();
    
    delay(20);
  }

  // msSystem.logln ( "" );
  // msSystem.log ( "Connected to: " );
  // msSystem.logln ( apInfo.ssid );
  // msSystem.log ( "IP address: " );
  // msSystem.logln ( WiFi.localIP() );

  return true;
}



bool TrySoftAP(struct APInfo &apInfo)
{
  // msSystem.log("Configuring access point: ");
  // msSystem.log(apInfo.ssid);
  // msSystem.log(" password: ");
  // msSystem.logln(apInfo.password);

//  WiFi.softAPConfig(IPAddress(10,20,30,40), IPAddress(10,1,1,1), IPAddress(255, 255, 255, 0));
  if (strlen(apInfo.password) == 0)
  {
    WiFi.softAP(apInfo.ssid);
  }
  else
  {
    WiFi.softAP(apInfo.ssid, apInfo.password);
  }

  // msSystem.log ( "Established acces point: " );
  // msSystem.logln ( apInfo.ssid );
  // msSystem.log("SoftAP IP: ");
  // msSystem.logln(WiFi.softAPIP());

  return true;
}

bool AutoConnect()
{
  struct jsonparse_state jsonState;
  struct APInfo apInfo;

  if (!forceAPMode)
  {
    if (Settings.getPreferedAP(&apInfo))
    {
      // msSystem.logln("stored prefered wifi found.");
      if (TryConnect(apInfo, CONNECTION_TIMEOUT))
      {
        msGlobals.apMode = false;
        return true;
      }
    }

    // msSystem.logln("WiFi AP scan starting...");
    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    // msSystem.logln("scan done");
    if (n == 0) {
      // msSystem.logln("no networks found");
    }
    else
    {
      // msSystem.log(n);
      // msSystem.logln(" networks found");
      for (int i = 0; i < n; ++i)
      {
        // Print SSID and RSSI for each network found
        // msSystem.log(i + 1);
        // msSystem.log(": ");
        // msSystem.log(WiFi.SSID(i));
        // msSystem.log(" (");
        // msSystem.log(WiFi.RSSI(i));
        // msSystem.log(")");
        // msSystem.logln((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
        delay(20);
      }
    }
    // msSystem.logln("");

    Settings.resetAPList();


    while (Settings.getNextAP(&apInfo))
    {
      for (int i = 0; i < n; i++)
      {
        if (strcmp(WiFi.SSID(i), apInfo.ssid) == 0)
        {
          if (TryConnect(apInfo, CONNECTION_TIMEOUT))
          {
            msGlobals.apMode = false;
            return true;
          }
        }
      }
    }

    // msSystem.logln("None of the stored WiFi APs was found!");
    false;
  }

  // msSystem.logln("fallback to standalone access point...");

  Settings.getAPConfig(&apInfo);
  if (TrySoftAP(apInfo))
  {
    msGlobals.apMode = true;
    return true;
  }

  // msSystem.logln("ERROR: AutoConnect failed!");
  return false;
}
