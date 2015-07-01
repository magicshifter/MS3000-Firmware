struct APSettings
{
  char apSSID[32];
  char apPassword[32];
};

struct ServerSettings
{
  char hostname[];
  int port;
};

struct HWInfo
{
  int leds;
  char colorFormat[5]; // ABGR
};

class SettingsManager
{
private:
  const String apConfigPath = "settings/ap.bin";
  const String apServerConfigPath = "settings/server.bin";
  const String apListConfigPath = "settings/apList.bin";

  int apListIndex = -1;
  FSFile apListFile;

public:
  void getServerConfig(struct ServerSettings *serverSettings)
  {

  }

  void setServerConfig(struct ServerSettings *serverSettings)
  {

  }

  bool getAPConfig(struct APInfo *apInfo)
  {
    String path = apConfigPath;
    if(FS.exists((char *)path.c_str()))
    {
      FSFile file = FS.open((char *)path.c_str());
      file.read(apInfo, sizeof(*apInfo));
      file.close();
      return true;
    }
    return false;
  }

  void setAPConfig(struct APInfo *apInfo)
  {
    String path = apConfigPath;
    FSFile file = FS.open((char *)path.c_str(), FSFILE_OVERWRITE);
    file.write((uint8_t *)apInfo, sizeof(*apInfo));
    file.close();
  }

  void getLastAP(struct APInfo *apInfo)
  {

  }

  void setLastAP(struct APInfo *apInfo)
  {

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
    while (apListFile.read(&apInfoDummy, requiredBytes) == requiredBytes)
    {
      if (memcmpByte((byte *)apInfo, 0, requiredBytes))
      {
        apListFile.seek(apListIndex * requiredBytes);
        break;
      }
      apListIndex++;
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

void handlePOSTAPSettings(void)
{
  fillPixels(0, 1, 0, 0x1F);
  updatePixels();

  if (server.args() >= 1)
  {
    // call load old settings
    APInfo apInfo;
    Settings.getAPConfig(&apInfo);


    const char* input = server.arg(0).c_str();
    struct jsonparse_state jsonState;
    jsonparse_setup(&jsonState, input, strlen(input));
    byte type;
    bool error = false;
    while (type = jsonparse_next(&jsonState))
    {
      if (type == JSON_TYPE_PAIR_NAME)
      {
        char key[50], data[50];
        jsonparse_copy_value(&jsonState, key, sizeof(key));
        Serial.print("found key: ");
        Serial.println(key);
        if (!AssertParseNext(&jsonState, JSON_TYPE_PAIR))
        {
          error=true;
          break;
        }
        if (!AssertParseNext(&jsonState, JSON_TYPE_STRING)) break;
        jsonparse_copy_value(&jsonState, data, sizeof(data));

        if (strcmp(key, "ssid") == 0)
        {
          safeStrncpy(apInfo.ssid, data, sizeof(apInfo.ssid));
        }
        else if (strcmp(key, "pwd") == 0)
        {
          safeStrncpy(apInfo.password, data, sizeof(apInfo.password));
        }
      }
    }

    if (!error)
    {
      Settings.setAPConfig(&apInfo);
      server.send ( 200, "text/plain", "OK" );
    }
    else
    {
      server.send ( 500, "text/plain", "argument iunvalid!");
    }
  }
  else
  {
    server.send ( 500, "text/plain", "argument missing!");
  }
}
