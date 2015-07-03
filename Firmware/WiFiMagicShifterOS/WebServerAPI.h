struct ServerConfig
{
  char hostname[48];
  int port;
};

struct HWInfo
{
  int leds;
  char colorFormat[5]; // ABGR
};

#define POSTHANDLER_START() \
          if (server.args() >= 1)\
          {\
            const char* input = server.arg(0).c_str();\
            char json[100];\
            url_decode(json, input, sizeof(json));

#define JSONPARSE_START(json) \
            struct jsonparse_state jsonState;\
            jsonparse_setup(&jsonState, json, strlen(json));\
            byte type;\
            bool error = false;\
            while (type = jsonparse_next(&jsonState))\
            {\
              if (type == JSON_TYPE_PAIR_NAME)\
              {\
                char key[30], data[50];\
                jsonparse_copy_value(&jsonState, key, sizeof(key));\
                //Serial.print("found key: ");\
                //Serial.println(key);\
                if (!AssertParseNext(&jsonState, JSON_TYPE_PAIR))\
                {\
                  error=true;\
                  break;\
                }\
                jsonparse_next(&jsonState); \ //dummy parse data of pair TODO: check for int, bool, or string)
                //if (!AssertParseNext(&jsonState, JSON_TYPE_STRING)) break;\
                jsonparse_copy_value(&jsonState, data, sizeof(data));

#define JSONPARSE_END()   } }

#define POSTHANDLER_END() \
            if (!error)\
            {\
              server.send ( 200, "text/plain", "OK" );\
            }\
            else\
            {\
              server.send ( 500, "text/plain", "argument invalid!");\
            }\
          }\
          else\
          {\
            server.send ( 500, "text/plain", "argument missing!");\
          }

class SettingsManager
{
private:
  const String apConfigPath = "settings/ap.bin";
  const String apServerConfigPath = "settings/server.bin";
  const String apListConfigPath = "settings/aplist.bin";
  const String preferedAPConfigPath = "settings/preferedap.bin";

  // used in resetAPList & getNextAP
  int apListIndex = -1;
  FSFile apListFile;

public:
  bool getServerConfig(struct ServerConfig *config)
  {
    String path = apServerConfigPath;
    if(FS.exists((char *)path.c_str()))
    {
      FSFile file = FS.open((char *)path.c_str());
      file.read(config, sizeof(*config));
      file.close();
      return true;
    }
    safeStrncpy(config->hostname, "magicshifter", sizeof(config->hostname));
    config->port = 80;
    return false;
  }

  void setServerConfig(struct ServerConfig *config)
  {
    String path = apServerConfigPath;
    FSFile file = FS.open((char *)path.c_str(), FSFILE_OVERWRITE);
    file.write((uint8_t *)config, sizeof(*config));
    file.close();
  }

  bool getAPConfig(struct APInfo *config)
  {
    String path = apConfigPath;
    if(FS.exists((char *)path.c_str()))
    {
      FSFile file = FS.open((char *)path.c_str());
      file.read(config, sizeof(*config));
      file.close();
      return true;
    }
    safeStrncpy(config->ssid, "MS3000", sizeof(config->ssid));
    safeStrncpy(config->password, "", sizeof(config->password));
    return false;
  }

  void setAPConfig(struct APInfo *config)
  {
    String path = apConfigPath;
    FSFile file = FS.open((char *)path.c_str(), FSFILE_OVERWRITE);
    file.write((uint8_t *)config, sizeof(*config));
    file.close();
  }

  bool getPreferedAP(struct APInfo *config)
  {
    String path = preferedAPConfigPath;
    if(FS.exists((char *)path.c_str()))
    {
      FSFile file = FS.open((char *)path.c_str());
      file.read(config, sizeof(*config));
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
    FSFile file = FS.open((char *)path.c_str(), FSFILE_OVERWRITE);
    file.write((uint8_t *)config, sizeof(*config));
    file.close();
  }

  void deleteAP(char *ssid)
  {
    String path = apListConfigPath;
    FSFile apListFile = FS.open((char *)path.c_str(), FSFILE_WRITE);
    APInfo apInfoDummy;
    const int requiredBytes = sizeof(apInfoDummy);
    int apListIndex = 0;
    while (apListFile.read(&apInfoDummy, requiredBytes) == requiredBytes)
    {
      if (strcmp(apInfoDummy.ssid, ssid) == 0)
      {
        memset(&apInfoDummy, 0, requiredBytes);
        apListFile.seek(apListIndex * requiredBytes);
        apListFile.write((uint8_t *)&apInfoDummy, requiredBytes);
        break;
      }
      apListIndex++;
    }
    apListFile.close();
  }

  void addAP(struct APInfo *apInfo)
  {
    String path = apListConfigPath;
    FSFile apListFile = FS.open((char *)path.c_str(), FSFILE_WRITE);
    const int requiredBytes = sizeof(*apInfo);
    APInfo apInfoDummy;
    int apListIndex = 0;
    int firstFreePos = -1;

    while (apListFile.read(&apInfoDummy, requiredBytes) == requiredBytes)
    {
      if (firstFreePos < 0 && memcmpByte((byte *)apInfo, 0, requiredBytes))
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
    if (firstFreePos >= 0)
    {
      apListFile.seek(firstFreePos);
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
      if(FS.exists((char *)path.c_str()))
      {
        apListFile = FS.open((char *)path.c_str());
        apListIndex = 0;
      }
    }

    if (apListIndex >= 0)
    {
      const int requiredBytes = sizeof(*apInfo);
      do
      {
        if (apListFile.read(apInfo, requiredBytes) == requiredBytes)
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
  }
};

SettingsManager Settings;

void handleGETServerSettings(void)
{
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
  server.send(200, "text/plain", response);
}

void handlePOSTServerSettings(void)
{
  POSTHANDLER_START()
    // load old settings
    ServerConfig config;
    Settings.getServerConfig(&config);

    JSONPARSE_START(input)
      if (strcmp(key, "host") == 0)
      {
        safeStrncpy(config.hostname, data, sizeof(config.hostname));
      }
      else if (strcmp(key, "port") == 0)
      {
        config.port = atoi(data);
      }
    JSONPARSE_END()

    if (!error)
    {
      // save modified settings
      Settings.setServerConfig(&config);
    }
  POSTHANDLER_END()
}

void handleGETAPSettings(void)
{
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
  server.send(200, "text/plain", response);
}

void handlePOSTAPSettings(void)
{
  POSTHANDLER_START()
    // load old settings
    APInfo apInfo;
    Settings.getAPConfig(&apInfo);

    JSONPARSE_START(input)
      if (strcmp(key, "ssid") == 0)
      {
        safeStrncpy(apInfo.ssid, data, sizeof(apInfo.ssid));
      }
      else if (strcmp(key, "pwd") == 0)
      {
        safeStrncpy(apInfo.password, data, sizeof(apInfo.password));
      }
    JSONPARSE_END()

    if (!error)
    {
      Settings.setAPConfig(&apInfo);
    }
  POSTHANDLER_END()
}


void handleGETPreferdAPSettings(void)
{
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
  server.send(200, "text/plain", response);
}

void handlePOSTPreferedAPSettings(void)
{
  POSTHANDLER_START()
    // load old settings
    APInfo apInfo;
    Settings.getPreferedAP(&apInfo);

    JSONPARSE_START(input)
      if (strcmp(key, "ssid") == 0)
      {
        safeStrncpy(apInfo.ssid, data, sizeof(apInfo.ssid));
      }
      else if (strcmp(key, "pwd") == 0)
      {
        safeStrncpy(apInfo.password, data, sizeof(apInfo.password));
      }
    JSONPARSE_END()

    if (!error)
    {
      Settings.setPreferedAP(&apInfo);
    }
  POSTHANDLER_END()
}


///////////////////////////////////////
//////////////////////////////////////////

void handleGETAPList(void)
{
  APInfo apInfo;
  Settings.getPreferedAP(&apInfo);

  String response = "[";

  Settings.resetAPList();
  bool firstAP = false;
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

  server.send(200, "text/plain", response);
}
void handlePOSTAPListAdd(void)
{
  POSTHANDLER_START()
    // load old settings
    APInfo apInfo;
    strcpy(apInfo.ssid, "");
    strcpy(apInfo.password, "");

    JSONPARSE_START(input)
      if (strcmp(key, "ssid") == 0)
      {
        safeStrncpy(apInfo.ssid, data, sizeof(apInfo.ssid));
      }
      else if (strcmp(key, "pwd") == 0)
      {
        safeStrncpy(apInfo.password, data, sizeof(apInfo.password));
      }
    JSONPARSE_END()

    if (!error)
    {
      if (!strcmp(apInfo.ssid, "") == 0)
      {
        Settings.addAP(&apInfo);
      }
    }
  POSTHANDLER_END()
}
void handlePOSTAPSListDelete(void)
{
  POSTHANDLER_START()
    // load old settings
    APInfo apInfo;
    strcpy(apInfo.ssid, "");
    strcpy(apInfo.password, "");

    JSONPARSE_START(input)
      if (strcmp(key, "ssid") == 0)
      {
        safeStrncpy(apInfo.ssid, data, sizeof(apInfo.ssid));
      }
    JSONPARSE_END()

    if (!error)
    {
      if (!strcmp(apInfo.ssid, "") == 0)
      {
        Settings.deleteAP(apInfo.ssid);
      }
    }
  POSTHANDLER_END()
}
