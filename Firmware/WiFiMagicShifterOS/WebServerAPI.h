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

  int apListIndex;

  void getServerConfig(struct ServerSettings *serverSettings)
  {

  }

  void setServerConfig(struct ServerSettings *serverSettings)
  {

  }

  void getAPConfig(struct APInfo *apInfo)
  {

  }

  void setAPConfig(struct APInfo *apInfo)
  {

  }

  void getLastAP(struct APInfo *apInfo)
  {

  }

  void setLastAP(struct APInfo *apInfo)
  {

  }

  void deleteAP(char *ssid)
  {

  }

  void addAP(struct APInfo *apInfo)
  {

  }

  void resetAPList()
  {
      apListIndex = 0;
  }

  bool getNextAP(struct APInfo *apInfo)
  {
    if(path.endsWith("/")) path += "index.htm";
    String contentType = getContentType(path);
    if(FS.exists((char *)(path+".gz").c_str()) || FS.exists((char *)path.c_str())){
      if(FS.exists((char *)(path+".gz").c_str()))
        path += ".gz";
      FSFile file = FS.open((char *)path.c_str());
      server.streamFile(file, contentType);
      file.close();
    FSFile apListFile =
  }
};


void handlePOSTSettings()
{
  byte type;
  bool error = false;

  fillPixels(0, 1, 0, 0x1F);
  updatePixels();
  String message = "handlePOSTSettings\n";

  if (server.args() >= 1)
  {
    const char* input = server.arg(0).c_str();
    struct jsonparse_state jsonState;
    jsonparse_setup(&jsonState, input, strlen(input));

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

        }
        else if (strcmp(key, "pwd") == 0)
        {

        }
      }
    }
  }
  else
  {
    message += "argument missing!";
  }
  server.send ( 200, "text/plain",message );
}
