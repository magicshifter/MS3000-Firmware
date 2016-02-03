#ifndef _WEBSERVERAPI_H
#define _WEBSERVERAPI_H

#define MAX_AP_LEN 48
struct APInfo
{
    char ssid[MAX_AP_LEN];
    char password[MAX_AP_LEN];

  public:
    APInfo()
    {
      clear();
    }

    void clear(void)
    {
      memset(ssid, 0, sizeof(ssid));
      memset(password, 0, sizeof(password));
    }
};


struct ServerConfig
{
  char hostname[48];
  int port;

public:
  ServerConfig()
  {
    clear();
  }

  void clear(void)
  {
    memset(hostname, 0, sizeof(hostname));
    port = 80;
  }
};

struct APConfig
{
  struct APInfo  apInfo;
  bool forceAPMode;
};

class SettingsManager
{
private:
  const String apConfigPath = "settings/ap.bin";
  const String apServerConfigPath = "settings/server1.bin";
  const String apListConfigPath = "settings/aplist1.bin";
  const String preferredAPConfigPath = "settings/preferredap.bin";

  // used in resetAPList & getNextAP
  int apListIndex = -1;
  File apListFile;

public:
  bool getServerConfig(struct ServerConfig *config)
  {
    
    String path = apServerConfigPath;
    if (SPIFFS.exists((char *)path.c_str()))
    {
      File file = SPIFFS.open((char *)path.c_str(), "r");
      file.read((uint8_t *)config, sizeof(*config));
      file.close();
      return true;
    }
    else 
    {
      msSystem.log("no server config file?::");
      msSystem.logln((char * )path.c_str());
    }

    l_safeStrncpy(config->hostname, "magicshifter", sizeof(config->hostname));
    config->port = 80;
    
    return false;
  }

  void setServerConfig(struct ServerConfig *config)
  {
    
    String path = apServerConfigPath;
    File file = SPIFFS.open((char *)path.c_str(), "w");
    file.write((uint8_t *)config, sizeof(*config));
    
    file.close();
  
  }

  bool getAPConfig(struct APInfo *config)
  {
    
    String path = apConfigPath;
    if (SPIFFS.exists((char *)path.c_str()))
    {
      File file = SPIFFS.open((char *)path.c_str(), "r");
      file.read((uint8_t *)config, sizeof(*config));
      file.close();
      return true;
    }
    else
    {
      msSystem.logln("AP config missing:");
      msSystem.logln((char *)path.c_str());
    }

    
#ifdef AP_NAME_OVERRIDE
    l_safeStrncpy(config->ssid, AP_NAME_OVERRIDE, sizeof(config->ssid));
    l_safeStrncpy(config->password, "", sizeof(config->password));
#endif
    
    return false;
  }

  void setAPConfig(struct APInfo *config)
  {
    
    String path = apConfigPath;
    File file = SPIFFS.open((char *)path.c_str(), "w");
    file.write((uint8_t *)config, sizeof(*config));
    file.close();

    msSystem.logln("saved:");
    msSystem.logln(config->ssid);
    
  }

  bool getPreferredAP(struct APInfo *config)
  {
    
    String path = preferredAPConfigPath;
    if (SPIFFS.exists((char *)path.c_str()))
    {
      File file = SPIFFS.open((char *)path.c_str(), "r");
      file.read((uint8_t *)config, sizeof(*config));
      file.close();
      return true;
    }
    l_safeStrncpy(config->ssid, "", sizeof(config->ssid));
    l_safeStrncpy(config->password, "", sizeof(config->password));
    return false;
    
  }

  void setPreferredAP(struct APInfo *config)
  {
    
    String path = preferredAPConfigPath;
    File file = SPIFFS.open((char *)path.c_str(), "w");
    file.write((uint8_t *)config, sizeof(*config));
    file.close();
    
  }

  void deleteAP(char *ssid)
  {
    
    String path = apListConfigPath;
    // bug in FS WRITE define!!!!
    //FSFile apListFile = FS.open((char *)path.c_str(), (SPIFFS_RDONLY | SPIFFS_WRONLY | SPIFFS_CREAT));
    File apListFile = SPIFFS.open((char *)path.c_str(), "a+");

    APInfo apInfoDummy;
    const int requiredBytes = sizeof(apInfoDummy);
    int apListIndex = 0;

    int lastPos = apListFile.position();

    while (apListFile.read((uint8_t *)&apInfoDummy, requiredBytes) == requiredBytes)
    {
      if (strcmp(apInfoDummy.ssid, ssid) == 0)
      {
        msSystem.logln("deleting wifi:");
        msSystem.logln(ssid);

        apInfoDummy.clear();
        int calcPos = apListIndex * requiredBytes;
        apListFile.seek(calcPos, SeekSet);
        apListFile.write((uint8_t *)&apInfoDummy, requiredBytes);
        break;
      }
      apListIndex++;
      lastPos = apListFile.position();
    }
    apListFile.close();
    
  }

  void addAP(struct APInfo *apInfo)
  {
    
    String path = apListConfigPath;
    File apListFile = SPIFFS.open((char *)path.c_str(), "r");
    const int requiredBytes = sizeof(*apInfo);
    APInfo apInfoDummy;
    int apListIndex = 0;
    int firstFreePos = -1;

    while (apListFile.read((uint8_t *)&apInfoDummy, requiredBytes) == requiredBytes)
    {
      if (firstFreePos < 0 && msSystem.msEEPROMs.memcmpByte((byte *)&apInfoDummy, 0, requiredBytes))
      {
        firstFreePos = apListIndex * requiredBytes;
      }
      else if (strcmp(apInfoDummy.ssid, apInfo->ssid) == 0)
      {
        firstFreePos = apListIndex * requiredBytes;
        break;
      }
      apListIndex++;
    }
    apListFile.close();

    if (firstFreePos >= 0)
    {
      msSystem.logln("found hole!");
      msSystem.logln(String(firstFreePos));
      apListFile = SPIFFS.open((char *)path.c_str(), "a+");
      apListFile.seek(firstFreePos, SeekSet);
    }
    else
    {
      msSystem.logln("appendiong at end");
      apListFile = SPIFFS.open((char *)path.c_str(), "A");
    }
    apListFile.write((uint8_t *)apInfo, requiredBytes);
    apListFile.close();
    
  }

  void resetAPList()
  {
    
      apListIndex = -1;
      apListFile.close();
      
  }

  bool getNextAP(struct APInfo *apInfo)
  {
    
    if (apListIndex < 0)
    {
      String path = apListConfigPath;
      if (SPIFFS.exists((char *)path.c_str()))
      {
        apListFile = SPIFFS.open((char *)path.c_str(), "r");
        apListIndex = 0;
      }
    }

    if (apListIndex >= 0)
    {
      const int requiredBytes = sizeof(*apInfo);
      do
      {
        if (apListFile.read((uint8_t *)apInfo, requiredBytes) == requiredBytes)
        {
          apListIndex++;
          if (!msSystem.msEEPROMs.memcmpByte((byte *)apInfo, 0, requiredBytes))
            return true;
        }
        else
        {
          return false;
        }
      } while(true);
    }
    else
    {
      return false;
    }
    

    ///hack
    return false;
  }
};
SettingsManager Settings;


void handleGETInfo(void)
{
  msSystem.logln("handleGETInfo");

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
  msSystem.logln("handleGETAbout");

  String response = "{";
    response += "\"type\":\"MagicShifter3000\",";
    response += "\"format\":\"BGRA\",";
    response += "\"version\":" + String(VERSION) + ",";
    response += "\"leds\":" + String(MAX_LEDS) + ",";
    response += "\"id\":" + String(ESP.getChipId()) + ",";
    response += "\"flashid\":" + String(ESP.getFlashChipId()) + ",";
    response += "\"flashsize\":" + String(ESP.getFlashChipSize());
  response += "}";
  msSystem.msESPServer.send(200, "text/plain", response);
}

void handleGETStatus(void)
{
  msSystem.logln("handleGETStatus");

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

  float voltage = ((float)(r1 + r2 + r3) * adValue) / (r1 * ad1V);


  msSystem.msSensor.readAccelData(msGlobals.ggAccelCount);

  for (int i = 0 ; i < 3 ; i++)
  {
    msGlobals.ggAccel[i] = (float) msGlobals.ggAccelCount[i] / ((1 << 12) / (2 * GSCALE)); // get actual g value, this depends on scale being set
  }

  uint32_t ip = (uint32_t)msSystem.getIP();


  String response = "{";
    response += "\"id\":" + String(ESP.getChipId()) + ",";
    response += "\"uptime\":" + String(millis() - msGlobals.ggBootTime) + ",";
    response += "\"adValue\":" + String(adValue) + ",";
    response += "\"voltage\":" + String(voltage) + ",";
    response += "\"ggAccelTime\":" + String(msGlobals.ggAccelTime) + ",";
    response += "\"ggLFrameTime\":" + String(msGlobals.ggLFrameTime) + ",";
    response += "\"ggLastFrameMicros\":" + String(msGlobals.ggLastFrameMicros) + ",";
    response += "\"ggCurrentMicros\":" + String(msGlobals.ggCurrentMicros) + ",";
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
    response += String(0xFF & (ip>>0)) + ".";
    response += String(0xFF & (ip>>8)) + ".";
    response += String(0xFF & (ip>>16)) + ".";
    response += String(0xFF & (ip>>24)) + "\"";
  response += "}";
  msSystem.msESPServer.send(200, "text/plain", response);
}

bool parseAPInfoFromServerArgs(APInfo &apInfo)
{
  bool success = true;
  for (int i = 0; i < msSystem.msESPServer.args(); i++)
  {
    msSystem.logln("argName: ");
    msSystem.logln(msSystem.msESPServer.argName(i));

    msSystem.logln("arg: ");
    msSystem.logln(msSystem.msESPServer.arg(i));

    if (strcmp(msSystem.msESPServer.argName(i).c_str(), "ssid") == 0)
    {
      l_safeStrncpy(apInfo.ssid, msSystem.msESPServer.arg(i).c_str(), sizeof(apInfo.ssid));
    }
    else if (strcmp(msSystem.msESPServer.argName(i).c_str(), "pwd") == 0)
    {
      l_safeStrncpy(apInfo.password, msSystem.msESPServer.arg(i).c_str(), sizeof(apInfo.password));
    }
    else
    {
      msSystem.logln("arg is not known!");
      success = false;
    }
  }
  return success;
}

void handleGetSettings(void)
{
  msSystem.logln("handleGetSettings");

  ServerConfig config;
  Settings.getServerConfig(&config);

  String response = "<!DOCTYPE html><html><head></head><body>";;
    response += "<h3>Urls</h3>";
    response += "<ul>";
      response += "<li><a href=\"/settings\">this page (/settings)</a></li>";
      response += "<li><a href=\"/settings/server\">server settings (/settings/server)</a></li>";
      response += "<li><a href=\"/settings/ap\">accesspoint settings (/settings/ap)</a></li>";

      response += "<li><a href=\"/info\">the schema page (/info)</a></li>";

      response += "<li><a href=\"/info/about\">about info page (/info/about)</a></li>";
      response += "<li><a href=\"/info/status\">status output page (/info/status)</a></li>";

      response += "<li><a href=\"/settings/wifi/list\">list of saved wifis (/settings/wifi/list)</a></li>";
      response += "<li><a href=\"/settings/wifi/preferred\">preferred wifi (/settings/wifi/preferred)</a></li>";

    response += "</ul>";
  response += "</body></html>";
  msSystem.msESPServer.send(200, "text/html", response);
}

void handleGETServerSettings(void)
{
  msSystem.logln("handleGETServerSettings");

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
  msSystem.logln("handlePOSTServerSettings");
  if (msSystem.msESPServer.args() >= 2)
  {
    ServerConfig config;
    Settings.getServerConfig(&config);

    bool success = true;
    for (int i = 0; i < msSystem.msESPServer.args(); i++)
    {
      msSystem.logln("argName: ");
      msSystem.logln(msSystem.msESPServer.argName(i));

      msSystem.logln("arg: ");
      msSystem.logln(msSystem.msESPServer.arg(i));

      if (strcmp(msSystem.msESPServer.argName(i).c_str(), "host") == 0)
      {
        l_safeStrncpy(config.hostname, msSystem.msESPServer.arg(i).c_str(), sizeof(config.hostname));
      }
      else if (strcmp(msSystem.msESPServer.argName(i).c_str(), "port") == 0)
      {
        config.port = atoi(msSystem.msESPServer.arg(i).c_str());
      }
      else
      {
        success = false;
      }
    }

    if (success)
    {
      Settings.setServerConfig(&config);
      msSystem.msESPServer.send (200, "text/plain", "OK");
    }
    else
    {
      msSystem.msESPServer.send(500, "text/plain", "invalid args!");
    }
  }
  else
  {
    msSystem.msESPServer.send ( 500, "text/plain", "argument missing!");
  }
}

void handleGETAPSettings(void)
{
  msSystem.logln("handleGETAPSettings");

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
  msSystem.logln("handlePOSTAPSettings");

  if (msSystem.msESPServer.args() >= 2)
  {
    // load old settings
    APInfo apInfo;
    Settings.getAPConfig(&apInfo);

    if (parseAPInfoFromServerArgs(apInfo))
    {
      msSystem.logln("saving setAPConfig");
      Settings.setAPConfig(&apInfo);
      msSystem.msESPServer.send (200, "text/plain", "OK");
    }
    else
    {
      msSystem.msESPServer.send(500, "text/plain", "unknown args!");
    }
  }
  else
  {
    msSystem.msESPServer.send ( 500, "text/plain", "argument missing!");
  }
}


void handleGETPreferredAPSettings(void)
{
  msSystem.logln("handleGETPreferredAPSettings");

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
  msSystem.logln("handlePOSTPreferredAPSettings");

  if (msSystem.msESPServer.args() >= 2)
  {
    APInfo apInfo;
    Settings.getPreferredAP(&apInfo);

    if (parseAPInfoFromServerArgs(apInfo))
    {
      msSystem.logln("saving setAPConfig");
      Settings.setPreferredAP(&apInfo);
      msSystem.msESPServer.send (200, "text/plain", "OK");
    }
    else
    {
      msSystem.msESPServer.send(500, "text/plain", "unknown args!");
    }
  }
  else
  {
    msSystem.msESPServer.send ( 500, "text/plain", "argument missing!");
  }
}


///////////////////////////////////////
//////////////////////////////////////////

void handleGETAPList(void)
{
  msSystem.logln("handleGETAPList");

  APInfo apInfo;
  Settings.getPreferredAP(&apInfo);

  String response = "[";

  Settings.resetAPList();
  bool firstAP = true;
  while (Settings.getNextAP(&apInfo))
  {
    if (!firstAP)
    {
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
  msSystem.logln("handleGETWLANList");

  // if (msGlobals.ggModeAP)
  // {
  //   msSystem.msESPServer.send(200, "text/plain", "crash in AP mode...so diusabled for now");
  //   return;
  // }

  String response = "[";

  int n = WiFi.scanNetworks();
  msSystem.logln("scan done");
  if (n == 0)
  {
    msSystem.logln("\"no networks found\"");
  }
  else
  {
    //msSystem.logln(n);
    msSystem.logln(" networks found");
    bool firstAP = true;
    for (int i = 0; i < n; ++i)
    {
      if (!firstAP)
      {
        response += ",";
      }
      firstAP = false;

      response += "{\"ssid\":";
      response += "\"";
      response += WiFi.SSID(i);
      response += "\"";
      response += ",";
      response += "\"rssi\":";
      response +=  WiFi.RSSI(i);
      response += ",";
      response += "\"free\":";
      response += (WiFi.encryptionType(i) == ENC_TYPE_NONE)?"true":"false";
      response += "}";
    }
  }


  response += "]";
  Settings.resetAPList();

  msSystem.msESPServer.send(200, "text/plain", response);
}

void handlePOSTAPListAdd(void)
{
  msSystem.logln("handlePOSTAPListAdd");

  if (msSystem.msESPServer.args() >= 2)
  {
    APInfo apInfo;
    memset(apInfo.ssid, 0, sizeof(apInfo.ssid));
    memset(apInfo.password, 0, sizeof(apInfo.password));

    if (parseAPInfoFromServerArgs(apInfo))
    {
      if (!strcmp(apInfo.ssid, "") == 0)
      {
        msSystem.logln("adding wifi");
        Settings.addAP(&apInfo);
      }
      msSystem.msESPServer.send (200, "text/plain", "OK");
    }
    else
    {
      msSystem.msESPServer.send(500, "text/plain", "invalid args!");
    }
  }
  else
  {
    msSystem.msESPServer.send(500, "text/plain", "args missing!");
  }
}

void handlePOSTAPListDelete(void)
{
  msSystem.logln("handlePOSTAPListDelete");

  if (msSystem.msESPServer.args() >= 1)
  {
    APInfo apInfo;
    strcpy(apInfo.ssid, "");
    strcpy(apInfo.password, "");

    if (parseAPInfoFromServerArgs(apInfo))
    {
      if (!strcmp(apInfo.ssid, "") == 0)
      {
        msSystem.logln("deleting wifi");
        Settings.deleteAP(apInfo.ssid);
      }
      msSystem.msESPServer.send (200, "text/plain", "OK");
    }
    else
    {
      msSystem.msESPServer.send(500, "text/plain", "invalid args!");
    }
  }
  else
  {
    msSystem.msESPServer.send(500, "text/plain", "args missing!");
  }
}


void handleSetMode(void)
{
  msSystem.logln("handleSetMode");
  if (msSystem.msESPServer.args() == 1)
  {
    ServerConfig config;
    Settings.getServerConfig(&config);

    bool success = true;
    for (int i = 0; i < msSystem.msESPServer.args(); i++)
    {
      msSystem.logln("argName: ");
      msSystem.logln(msSystem.msESPServer.argName(i));

      msSystem.logln("arg: ");
      msSystem.logln(msSystem.msESPServer.arg(i));

      msGlobals.ggCurrentMode = atoi(msSystem.msESPServer.arg(i).c_str());
    }

    if (success)
    {
      String response = "{";
      response += "\"mode\":";
      response += "\"";
      response += msGlobals.ggCurrentMode;
      response += "\"";
      response += "}";
      msSystem.msESPServer.send(200, "text/plain", response);
    }
    else
    {
      msSystem.msESPServer.send(500, "text/plain", "invalid args!");
    }
  }
  else
  {
    msSystem.msESPServer.send ( 500, "text/plain", "argument missing!");
  }
}

void respondREQTime()
{
  int currentTime = msGlobals.ggTime + (millis() - msGlobals.ggTimePostedAt);

  String response = "{";
    response += "\"time\":" + String(currentTime) + ",";
    response += "\"postedAt\":" + String(msGlobals.ggTimePostedAt);
  response += "}";

  msSystem.msESPServer.send ( 200, "text/plain", response);
}

void handleGETTime()
{
  msSystem.logln("handleGETTime");

  respondREQTime();
}

void handlePOSTTime()
{
  msSystem.logln("handlePOSTTime");

  if (msSystem.msESPServer.argName(0) == "t")
  {
    msGlobals.ggTime = atoi(msSystem.msESPServer.arg(0).c_str());
    msGlobals.ggTimePostedAt = millis();
  }

  respondREQTime();
}

void handleLedSet()
{
  byte ledData[MAX_LEDS*5];

  String message = "LedSet\n\n";

  if (msSystem.msESPServer.args() == 1)
  {
    const char* input = msSystem.msESPServer.arg(0).c_str();
    unsigned int inputLen =  (int)msSystem.msESPServer.arg(0).length();
    msSystem.logln("inputLen: ");
    msSystem.logln(String(inputLen));

    if (inputLen > sizeof(ledData))
      inputLen = sizeof(ledData);

    msSystem.logln(String(inputLen));

    unsigned int dataLen = base64_decode((char *)ledData, input, inputLen);

    for (int i = 0; i < dataLen; i += 5)
    {
      //msSystem.msLEDs.setPixels(ledData[i], ledData[i+1], ledData[i+2], ledData[i+3], ledData[i+4]);
      byte idx = ledData[i];
      msSystem.logln("idx: ");
      msSystem.logln(String((int)idx));
      if (idx >= MAX_LEDS) continue;
      msSystem.logln("data+1: ");
      msSystem.logln(String((int)ledData[i+1]));

      msGlobals.ggRGBLEDBuf[idx*4] = ledData[i+1];
      msGlobals.ggRGBLEDBuf[idx*4+1] = ledData[i+2];
      msGlobals.ggRGBLEDBuf[idx*4+2] = ledData[i+3];
      msGlobals.ggRGBLEDBuf[idx*4+3] = ledData[i+4];
    }
  }
  else
  {
    message += "argument missing or too many arguments!";
  }
  msSystem.msESPServer.send ( 200, "text/plain",message );
}


void handleLedsSet()
{
  msSystem.logln("handleLedsSet");
  
  String message = "LedsSet\n\n";

  if (msSystem.msESPServer.args() >= 1)
  {
    const char* input = msSystem.msESPServer.arg(0).c_str();
    int inputLen = BASE64_ENC_LEN(RGB_BUFFER_SIZE);

    base64_decode((char *)msGlobals.ggRGBLEDBuf, input, inputLen);

    message += "done";
  }
  else
  {
    message += "argument missing!";
  }
  msSystem.msESPServer.send ( 200, "text/plain",message );
}

#endif

