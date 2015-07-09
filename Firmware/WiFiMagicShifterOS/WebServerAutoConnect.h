#include "Config.h"

bool TryConnect(struct APInfo &apInfo, int timeoutMs)
{
  Serial.print("trying to connect to AP: ");
  Serial.println(apInfo.ssid);
  WiFi.begin (apInfo.ssid, apInfo.password );

  // Wait for connection
  int frame = 0;
  long startTime = millis();

  while ( WiFi.status() != WL_CONNECTED ) {
    for (int i = 0; i < LEDS; i++)
    {
      if (i < frame % LEDS)
        setPixel(i, 1, 1, 1, 0x1);
      else
        setPixel(i, 0, 0, 0, 0);
    }
    updatePixels();
    frame++;
    if (frame%50 == 0) Serial.print(".");
    if (WiFi.status() == WL_NO_SSID_AVAIL || WiFi.status() == WL_CONNECT_FAILED || millis() > startTime + timeoutMs)
    {
      Serial.println ( "" );
      Serial.print ("Could NOT connect to:");
      Serial.println(apInfo.ssid);
      return false; // :(
    }
    delay(20);
  }

  Serial.println ( "" );
  Serial.print ( "Connected to: " );
  Serial.println ( apInfo.ssid );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP() );

  return true;
}

bool TrySoftAP(struct APInfo &apInfo)
{
  Serial.print("Configuring access point: ");
  Serial.print(apInfo.ssid);
  Serial.print(" password: ");
  Serial.println(apInfo.password);

//  WiFi.softAPConfig(IPAddress(10,20,30,40), IPAddress(10,1,1,1), IPAddress(255, 255, 255, 0));
  if (strlen(apInfo.password) == 0)
  {
    WiFi.softAP(apInfo.ssid);
  }
  else
  {
    WiFi.softAP(apInfo.ssid, apInfo.password);
  }

  Serial.print ( "Established acces point: " );
  Serial.println ( apInfo.ssid );
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());

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
      Serial.println("stored prefered wifi found.");
      if (TryConnect(apInfo, CONNECTION_TIMEOUT))
      {
        return true;
      }
    }

    Serial.println("WiFi AP scan starting...");
    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0)
      Serial.println("no networks found");
    else
    {
      Serial.print(n);
      Serial.println(" networks found");
      for (int i = 0; i < n; ++i)
      {
        // Print SSID and RSSI for each network found
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(WiFi.SSID(i));
        Serial.print(" (");
        Serial.print(WiFi.RSSI(i));
        Serial.print(")");
        Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
        delay(20);
      }
    }
    Serial.println("");

    Settings.resetAPList();


    while (Settings.getNextAP(&apInfo))
    {
      for (int i = 0; i < n; i++)
      {
        if (strcmp(WiFi.SSID(i), apInfo.ssid) == 0)
        {
          if (TryConnect(apInfo, CONNECTION_TIMEOUT))
          {
            return true;
          }
        }
      }
    }

    Serial.println("None of the stored WiFi APs was found!");
    false;
  }

  Serial.println("fallback to standalone access point...");

  Settings.getAPConfig(&apInfo);
  if (TrySoftAP(apInfo))
  {
    return true;
  }

  Serial.println("ERROR: AutoConnect failed!");
  return false;
}
