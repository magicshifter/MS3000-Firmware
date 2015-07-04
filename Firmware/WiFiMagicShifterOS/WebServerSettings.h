#define JSON_TYPE_ARRAY_END ']'
#define JSON_TYPE_OBJECT_END '}'
#define JSON_TYPE_SEPERATOR ','


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

void ReportJsonLocation(struct jsonparse_state *jsonStatePtr)
{
  Serial.print("at position: ");
  Serial.print(jsonStatePtr->pos);
  Serial.print(" of text: ");
  Serial.println(jsonStatePtr->json);
}

bool AssertParseNext(struct jsonparse_state *jsonStatePtr, byte expectedType)
{
  byte type;

  if ((type = jsonparse_next(jsonStatePtr) != expectedType))
  {
    Serial.print("AssertParseNext failed! Expected: ");
    Serial.print(expectedType);
    Serial.print(" but found: ");
    Serial.println(type);
    ReportJsonLocation(jsonStatePtr);
    return false;
  }
  return true;
}

int AssertParseNextMultiple(struct jsonparse_state *jsonStatePtr, byte typeA, byte typeB)
{
  byte type = jsonparse_next(jsonStatePtr);

  if (type == typeA) return 0;
  if (type == typeB) return 1;

  Serial.print("AssertParseNext failed! Expected one of: ");

  Serial.print(typeA);
  Serial.print(" or ");
  Serial.print(typeB);

  Serial.print(" but found: ");
  Serial.println(type);
  ReportJsonLocation(jsonStatePtr);
  return -1;
}

int TryParseKeyValue(struct jsonparse_state *jsonStatePtr, const char *key, char *data, int size)
{
  if (jsonparse_strcmp_value(jsonStatePtr, key) == 0)
  {
    // 0 means syntax error
    if (!AssertParseNext(jsonStatePtr, JSON_TYPE_PAIR)) return 0;
    if (!AssertParseNext(jsonStatePtr, JSON_TYPE_STRING)) return 0;
    jsonparse_copy_value(jsonStatePtr, data, size);

    return 2; // 2 means correct syntax and key found
  }
  return 1; // 1 means correct syntax but not found
}

bool ParseAPInfo(struct APInfo *apInfo, struct jsonparse_state *jsonState)
{
  byte type;

  if (!AssertParseNext(jsonState, JSON_TYPE_OBJECT)) return false;

  while ((type = jsonparse_next(jsonState)) == JSON_TYPE_PAIR_NAME || type == JSON_TYPE_SEPERATOR)
  {
    if (type == JSON_TYPE_PAIR_NAME)
    {
      int parseResult;

      if (!(parseResult = TryParseKeyValue(jsonState, "ssid", apInfo->ssid, MAX_AP_LEN))) return false;
      if (parseResult == 2) continue;

      if (!(parseResult = TryParseKeyValue(jsonState, "pwd", apInfo->password, MAX_AP_LEN))) return false;
      if (parseResult == 2) continue;

      // TODO: improve skipping of unnecessary fields
      char key[20];
      jsonparse_copy_value(jsonState, key, 20);
      Serial.print("ignoring unknown key: ");
      Serial.println(key);
      ReportJsonLocation(jsonState);
      if (!AssertParseNext(jsonState, JSON_TYPE_PAIR)) return false;
      if (!AssertParseNext(jsonState, JSON_TYPE_STRING)) return false;
    }
  }

  if (type != JSON_TYPE_OBJECT_END)
  {
    Serial.print("Expected end of object '}' but got: ");
    Serial.println(type);
    ReportJsonLocation(jsonState);
    return false;
  }
  return true;
}

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

bool TrySoftAP(struct APInfo &apInfo, int timeoutMs)
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
  #define CONNECTION_TIMEOUT 40000
  struct jsonparse_state jsonState;
  struct APInfo apInfo;

  if (!forceAPMode)
  {
    jsonparse_setup(&jsonState, jsonLastAP, strlen(jsonLastAP));

    if (ParseAPInfo(&apInfo, &jsonState))
    {
      Serial.println("stored last ssid found!");
      if (TryConnect(apInfo, CONNECTION_TIMEOUT))
      {
        return true; // no need to change anything
      }
    }
    else
    {
      Serial.println("stored last ssid has syntax error or is missing!");
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

    jsonparse_setup(&jsonState, jsonAPList, strlen(jsonAPList));

    do {
      if (!AssertParseNext(&jsonState, JSON_TYPE_OBJECT)) break;
      if (!AssertParseNext(&jsonState, JSON_TYPE_PAIR_NAME)) break;
      if (!AssertParseNext(&jsonState, JSON_TYPE_PAIR)) break;
      if (!AssertParseNext(&jsonState, JSON_TYPE_ARRAY)) break;

      bool result;
      while (ParseAPInfo(&apInfo, &jsonState))
      {
        for (int i = 0; i < n; i++)
        {
          if (strcmp(WiFi.SSID(i), apInfo.ssid) == 0)
          {
            if (TryConnect(apInfo, CONNECTION_TIMEOUT))
            {
              // TODO: store it for netx
              return true;
            }
          }
        }

        int nextTypeIndex;
        if ((nextTypeIndex = AssertParseNextMultiple(&jsonState, JSON_TYPE_ARRAY_END, JSON_TYPE_SEPERATOR)) < 0)
        {
          Serial.println("Invalid list of stored WiFi APs!");
          break;
        }
        if (nextTypeIndex == 0)
        {
          Serial.println("None of the stored WiFi APs was found!");
          break;
        }
      }
    } while(0);
  }

  Serial.println("fallback to standalone access point...");
  jsonparse_setup(&jsonState, jsonSoftAP, strlen(jsonSoftAP));

  if (ParseAPInfo(&apInfo, &jsonState))
  {
    Serial.println("stored AP config found!");
    if (TrySoftAP(apInfo, CONNECTION_TIMEOUT))
    {
      return true;
    }
  }
  else
  {
    Serial.println("stored AP config has syntax error or is missing!");
  }


  Serial.println("ERROR: AutoConnect failed!");
  return false;
}
