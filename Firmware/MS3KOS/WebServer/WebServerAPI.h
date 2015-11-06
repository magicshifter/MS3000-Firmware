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
  const String preferedAPConfigPath = "settings/preferedap.bin";

  // used in resetAPList & getNextAP
  int apListIndex = -1;
  File apListFile;

public:
  bool getServerConfig(struct ServerConfig *config)
  {
    
    String path = apServerConfigPath;
    if(SPIFFS.exists((char *)path.c_str()))
    {
      File file = SPIFFS.open((char *)path.c_str(), "r");
      file.read((uint8_t *)config, sizeof(*config));
      file.close();
      return true;
    }
    else 
    {
      msSystem.log("File doesn't exist:");
      msSystem.logln((char * )path.c_str());
    }

    safeStrncpy(config->hostname, "magicshifter", sizeof(config->hostname));
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
    if(SPIFFS.exists((char *)path.c_str()))
    {
      File file = SPIFFS.open((char *)path.c_str(), "r");
      file.read((uint8_t *)config, sizeof(*config));
      file.close();
      return true;
    }
    else
    {
      msSystem.log("AP config missing:");
      msSystem.logln((char *)path.c_str());
    }

    safeStrncpy(config->ssid, "MS3000", sizeof(config->ssid));
    safeStrncpy(config->password, "", sizeof(config->password));
    
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

  bool getPreferedAP(struct APInfo *config)
  {
    
    String path = preferedAPConfigPath;
    if(SPIFFS.exists((char *)path.c_str()))
    {
      File file = SPIFFS.open((char *)path.c_str(), "r");
      file.read((uint8_t *)config, sizeof(*config));
      file.close();
      return true;
    }
    safeStrncpy(config->ssid, "", sizeof(config->ssid));
    safeStrncpy(config->password, "", sizeof(config->password));
    return false;
    
  }

  void setPreferedAP(struct APInfo *config)
  {
    
    String path = preferedAPConfigPath;
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
        msSystem.log("deleting wifi:");
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
      if (firstFreePos < 0 && memcmpByte((byte *)&apInfoDummy, 0, requiredBytes))
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
      apListFile = SPIFFS.open((char *)path.c_str(), "a");
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
      if(SPIFFS.exists((char *)path.c_str()))
      {
        //apListFile = SPIFFS.open((char *)path.c_str(), "r");
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
          if (!memcmpByte((byte *)apInfo, 0, requiredBytes))
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
    response += "\"api\": {";
      response += "\"versions\": [1],";
      response += "\"prefix\": \"v\"";
    response += "},";
    response += "\"init\": {";
      response += "\"settings\": [";
        response += "\"server\",";
        response += "\"ap\",";
        response += "\"wifi/list\",";
        response += "\"wifi/prefered\"";
      response += "],";
      response += "\"info\": [";
        response += "\"about\",";
        response += "\"status\"";
      response += "]";
    response += "}";
  response += "}";
  msSystem.msServer.send(200, "text/plain", response);
}


void handleGETAbout(void)
{
  msSystem.logln("handleGETAbout");

  String response = "{\"type\":\"MagicShifter3000\",\"format\":\"BGRA\",\"version\":" + String(VERSION) +
    ",\"leds\":" + String(MAX_LEDS) + ",\"id\":" + String(ESP.getChipId()) + ",\"flashid\":" + String(ESP.getFlashChipId()) + ",\"flashsize\":" + String(ESP.getFlashChipSize()) + "}";
  msSystem.msServer.send(200, "text/plain", response);
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


  readAccelData(msGlobals.accelCount);

  for (int i = 0 ; i < 3 ; i++)
  {
    msGlobals.accelG[i] = (float) msGlobals.accelCount[i] / ((1 << 12) / (2 * GSCALE)); // get actual g value, this depends on scale being set
  }

  uint32_t ip = (uint32_t)msSystem.getIP();

  String response = "{\"id\":" + String(ESP.getChipId()) + ",\"uptime\":" +
  String(millis() - msGlobals.bootTime) + ",\"adValue\":" + adValue + ",\"voltage\":" + voltage +
  "\",accelRaw\":[" + msGlobals.accelCount[0] + "," + msGlobals.accelCount[1] + "," + msGlobals.accelCount[2] + "],\"msGlobals.accelG\":[" + msGlobals.accelG[0] + "," + msGlobals.accelG[1] + ","+ msGlobals.accelG[2] + 
    "],\"ip\":\"" + (0xFF & (ip>>0)) + "."  + (0xFF & (ip>>8)) + "."  + (0xFF & (ip>>16)) + "."  + (0xFF & (ip>>24)) + "\"}";
  msSystem.msServer.send(200, "text/plain", response);
}

bool parseAPInfoFromServerArgs(APInfo &apInfo)
{
  bool success = true;
  for (int i = 0; i < msSystem.msServer.args(); i++)
  {
    msSystem.logln("argName: ");
    msSystem.logln(msSystem.msServer.argName(i));

    msSystem.logln("arg: ");
    msSystem.logln(msSystem.msServer.arg(i));

    if (strcmp(msSystem.msServer.argName(i).c_str(), "ssid") == 0)
    {
      safeStrncpy(apInfo.ssid, msSystem.msServer.arg(i).c_str(), sizeof(apInfo.ssid));
    }
    else if (strcmp(msSystem.msServer.argName(i).c_str(), "pwd") == 0)
    {
      safeStrncpy(apInfo.password, msSystem.msServer.arg(i).c_str(), sizeof(apInfo.password));
    }
    else
    {
      msSystem.logln("arg is not known!");
      success = false;
    }
  }
  return success;
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
  msSystem.msServer.send(200, "text/plain", response);
}

void handlePOSTServerSettings(void)
{
  msSystem.logln("handlePOSTServerSettings");
  if (msSystem.msServer.args() >= 2)
  {
    ServerConfig config;
    Settings.getServerConfig(&config);

    bool success = true;
    for (int i = 0; i < msSystem.msServer.args(); i++)
    {
      msSystem.logln("argName: ");
      msSystem.logln(msSystem.msServer.argName(i));

      msSystem.logln("arg: ");
      msSystem.logln(msSystem.msServer.arg(i));

      if (strcmp(msSystem.msServer.argName(i).c_str(), "host") == 0)
      {
        safeStrncpy(config.hostname, msSystem.msServer.arg(i).c_str(), sizeof(config.hostname));
      }
      else if (strcmp(msSystem.msServer.argName(i).c_str(), "port") == 0)
      {
        config.port = atoi(msSystem.msServer.arg(i).c_str());
      }
      else
      {
        success = false;
      }
    }

    if (success)
    {
      Settings.setServerConfig(&config);
      msSystem.msServer.send (200, "text/plain", "OK");
    }
    else
    {
      msSystem.msServer.send(500, "text/plain", "invalid args!");
    }
  }
  else
  {
    msSystem.msServer.send ( 500, "text/plain", "argument missing!");
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
  msSystem.msServer.send(200, "text/plain", response);
}

void handlePOSTAPSettings(void)
{
  msSystem.logln("handlePOSTAPSettings");

  if (msSystem.msServer.args() >= 2)
  {
    // load old settings
    APInfo apInfo;
    Settings.getAPConfig(&apInfo);

    if (parseAPInfoFromServerArgs(apInfo))
    {
      msSystem.logln("saving setAPConfig");
      Settings.setAPConfig(&apInfo);
      msSystem.msServer.send (200, "text/plain", "OK");
    }
    else
    {
      msSystem.msServer.send(500, "text/plain", "unknown args!");
    }
  }
  else
  {
    msSystem.msServer.send ( 500, "text/plain", "argument missing!");
  }
}


void handleGETPreferdAPSettings(void)
{
  msSystem.logln("handleGETPreferdAPSettings");

  APInfo apInfo;
  Settings.getPreferedAP(&apInfo);

  String response = "{";
  response += "\"ssid\":";
  response += "\"";
  response += apInfo.ssid;
  response += "\"";
  //response += ",";
  //response += "\"pwd\":";
  //response += "\"" + apInfo.pwd + "\"";
  response += "}";
  msSystem.msServer.send(200, "text/plain", response);
}

void handlePOSTPreferedAPSettings(void)
{
  msSystem.logln("handlePOSTPreferedAPSettings");

  if (msSystem.msServer.args() >= 2)
  {
    APInfo apInfo;
    Settings.getPreferedAP(&apInfo);

    if (parseAPInfoFromServerArgs(apInfo))
    {
      msSystem.logln("saving setAPConfig");
      Settings.setPreferedAP(&apInfo);
      msSystem.msServer.send (200, "text/plain", "OK");
    }
    else
    {
      msSystem.msServer.send(500, "text/plain", "unknown args!");
    }
  }
  else
  {
    msSystem.msServer.send ( 500, "text/plain", "argument missing!");
  }
}


///////////////////////////////////////
//////////////////////////////////////////

void handleGETAPList(void)
{
  msSystem.logln("handleGETAPList");

  APInfo apInfo;
  Settings.getPreferedAP(&apInfo);

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

  msSystem.msServer.send(200, "text/plain", response);
}

void handleGETWLANList(void)
{
  msSystem.logln("handleGETWLANList");

  if (msGlobals.apMode)
  {
    msSystem.msServer.send(200, "text/plain", "crash in AP mode...so diusabled for now");
    return;
  }

  String response = "[";

  int n = WiFi.scanNetworks();
  msSystem.logln("scan done");
  if (n == 0){
    msSystem.logln("\"no networks found\"");
  }
  else
  {
    //msSystem.log(n);
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

  msSystem.msServer.send(200, "text/plain", response);
}

void handlePOSTAPListAdd(void)
{
  msSystem.logln("handlePOSTAPListAdd");

  if (msSystem.msServer.args() >= 2)
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
      msSystem.msServer.send (200, "text/plain", "OK");
    }
    else
    {
      msSystem.msServer.send(500, "text/plain", "invalid args!");
    }
  }
  else
  {
    msSystem.msServer.send(500, "text/plain", "args missing!");
  }
}

void handlePOSTAPListDelete(void)
{
  msSystem.logln("handlePOSTAPListDelete");

  if (msSystem.msServer.args() >= 1)
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
      msSystem.msServer.send (200, "text/plain", "OK");
    }
    else
    {
      msSystem.msServer.send(500, "text/plain", "invalid args!");
    }
  }
  else
  {
    msSystem.msServer.send(500, "text/plain", "args missing!");
  }
}


void handleSetMode(void)
{
  msSystem.logln("handleSetMode");
  if (msSystem.msServer.args() == 1)
  {
    ServerConfig config;
    Settings.getServerConfig(&config);

    bool success = true;
    for (int i = 0; i < msSystem.msServer.args(); i++)
    {
      msSystem.logln("argName: ");
      msSystem.logln(msSystem.msServer.argName(i));

      msSystem.logln("arg: ");
      msSystem.logln(msSystem.msServer.arg(i));

      msGlobals.shifterMode = atoi(msSystem.msServer.arg(i).c_str());
    }

    if (success)
    {
      String response = "{";
      response += "\"mode\":";
      response += "\"";
      response += msGlobals.shifterMode;
      response += "\"";
      response += "}";
      msSystem.msServer.send(200, "text/plain", response);
    }
    else
    {
      msSystem.msServer.send(500, "text/plain", "invalid args!");
    }
  }
  else
  {
    msSystem.msServer.send ( 500, "text/plain", "argument missing!");
  }
}


void handleLedSet()
{
  byte ledData[MAX_LEDS*5];

  String message = "LedSet\n\n";

  if (msSystem.msServer.args() == 1)
  {
    const char* input = msSystem.msServer.arg(0).c_str();
    unsigned int inputLen =  (int)msSystem.msServer.arg(0).length();
    msSystem.log("inputLen: ");
    msSystem.logln(String(inputLen));

    if (inputLen > sizeof(ledData))
      inputLen = sizeof(ledData);

    msSystem.logln(String(inputLen));

    unsigned int dataLen = base64_decode((char *)ledData, input, inputLen);

    for (int i = 0; i < dataLen; i += 5)
    {
      //setPixel(ledData[i], ledData[i+1], ledData[i+2], ledData[i+3], ledData[i+4]);
      byte idx = ledData[i];
      msSystem.log("idx: ");
      msSystem.logln(String((int)idx));
      if (idx >= MAX_LEDS) continue;
      msSystem.log("data+1: ");
      msSystem.logln(String((int)ledData[i+1]));

      msGlobals.web_rgb_buffer[idx*4] = ledData[i+1];
      msGlobals.web_rgb_buffer[idx*4+1] = ledData[i+2];
      msGlobals.web_rgb_buffer[idx*4+2] = ledData[i+3];
      msGlobals.web_rgb_buffer[idx*4+3] = ledData[i+4];
    }
  }
  else
  {
    message += "argument missing or too many arguments!";
  }
  msSystem.msServer.send ( 200, "text/plain",message );
}


void handleLedsSet()
{
  msSystem.logln("handleLedsSet");
  
  String message = "LedsSet\n\n";

  if (msSystem.msServer.args() >= 1)
  {
    const char* input = msSystem.msServer.arg(0).c_str();
    int inputLen = BASE64_ENC_LEN(RGB_BUFFER_SIZE);

    base64_decode((char *)msGlobals.web_rgb_buffer, input, inputLen);

    message += "done";
  }
  else
  {
    message += "argument missing!";
  }
  msSystem.msServer.send ( 200, "text/plain",message );
}

#endif

