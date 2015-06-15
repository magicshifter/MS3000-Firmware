

#define JSON_TYPE_ARRAY_END ']'
#define JSON_TYPE_OBJECT_END '}'
#define JSON_TYPE_SEPERATOR ','

const char *jsonLastAP =
//"{\"ssid\":\"PACIFIC\", \"pwd\":\"AllesR0ger\"}";
"{\"ssid\":\"wizard23\", \"pwd\":\"lolinternets\"}";

const char *jsonAPList =  "{\"list\": [{\"ssid\":\"wizard23\", \"pwd\":\"lolinternets\"}, {\"ssid\":\"PACIFIC\", \"pwd\":\"AllesR0ger\"}]}";


#define MAX_AP_LEN 48

struct APInfo
{
    char ssid[MAX_AP_LEN];
    char password[MAX_AP_LEN];
};

bool AssertParseNext(struct jsonparse_state *jsonStatePtr, byte expectedType)
{
  byte type;

  if ((type = jsonparse_next(jsonStatePtr) != expectedType))
  {
    Serial.print("AssertParseNext failed! Expected: ");
    Serial.print(expectedType);
    Serial.print(" but found: ");
    Serial.print(type);
    Serial.print(" at position: ");
    Serial.println(jsonStatePtr->pos);
    Serial.print("in: ");
    Serial.println(jsonStatePtr->json);
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
  Serial.print(type);
  Serial.print(" at position: ");
  Serial.println(jsonStatePtr->pos);
  Serial.print("in: ");
  Serial.println(jsonStatePtr->json);
  return -1;
}

bool ParseAPInfo(struct APInfo *apInfo, struct jsonparse_state *jsonState)
{
  byte type;

  if (!AssertParseNext(jsonState, JSON_TYPE_OBJECT)) return false;

  while ((type = jsonparse_next(jsonState)) == JSON_TYPE_PAIR_NAME ||
    type == JSON_TYPE_SEPERATOR)
  {
    Serial.print((char)type); Serial.print(type); Serial.println("//parsed");

    if (type == JSON_TYPE_PAIR_NAME)
    {
      if (jsonparse_strcmp_value(jsonState, "ssid") == 0)
      {
        if (jsonparse_next(jsonState) != JSON_TYPE_PAIR) { Serial.print(jsonparse_get_type(jsonState)); Serial.println("-xb"); return false; }
        if (jsonparse_next(jsonState) != JSON_TYPE_STRING) { Serial.print(jsonparse_get_type(jsonState)); Serial.println("-xc"); return false; }
        jsonparse_copy_value(jsonState, apInfo->ssid, MAX_AP_LEN);
      }
      else if (jsonparse_strcmp_value(jsonState, "pwd") == 0)
      {
        if (jsonparse_next(jsonState) != JSON_TYPE_PAIR) { Serial.print(jsonparse_get_type(jsonState)); Serial.println("-xd"); return false; }
        if (jsonparse_next(jsonState) != JSON_TYPE_STRING) { Serial.print(jsonparse_get_type(jsonState)); Serial.println("-xe"); return false; }
        jsonparse_copy_value(jsonState, apInfo->password, MAX_AP_LEN);
      }
      else
      {
        jsonparse_copy_value(jsonState, apInfo->ssid, MAX_AP_LEN);
        Serial.println(apInfo->ssid);
        Serial.print((char)type); Serial.print(type); Serial.println("//uknwn");
      }
    }
    else
    {
      Serial.print((char)type); Serial.print(type); Serial.println("//ignored");
    }
  }
  Serial.print((char)type); Serial.print(type); Serial.println("//exparsed");

  if (type != JSON_TYPE_OBJECT_END) {
    Serial.print(type); Serial.print("/"); Serial.print(jsonparse_get_type(jsonState)); Serial.println("-xxf"); return false; }
  return true;
}

bool TryConnect(struct APInfo &apInfo, int timeoutMs)
{
  Serial.print("trying to connect to AP: ");
  Serial.println(apInfo.ssid);
  //Serial.print("pwd: ");
  //Serial.println(apInfo.password);
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
      Serial.print ( "Could NOT connect to:" );
      Serial.println ( ssid );
      return false; // :(
    }
    delay(10);
  }

  Serial.println ( "" );
  Serial.print ( "Connected to: " );
  Serial.println ( apInfo.ssid );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP() );

  return true;
}

bool AutoConnect()
{
  #define CONNECTION_TIMEOUT 30000
  struct jsonparse_state jsonState;
  jsonparse_setup(&jsonState, jsonLastAP, strlen(jsonLastAP));

  struct APInfo apInfo;
  if (ParseAPInfo(&apInfo, &jsonState))
  {
    Serial.println("stored last ssid found!");
    if (TryConnect(apInfo, CONNECTION_TIMEOUT))
    {
      return true; // no need to change anything
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


  jsonparse_setup(&jsonState, jsonAPList, strlen(jsonAPList));

  if (jsonparse_next(&jsonState) != JSON_TYPE_OBJECT) { Serial.print(jsonparse_get_type(&jsonState)); Serial.println("-a"); return false; }
  if (jsonparse_next(&jsonState) != JSON_TYPE_PAIR_NAME) { Serial.print(jsonparse_get_type(&jsonState)); Serial.println("-b"); return false; }
  if (jsonparse_next(&jsonState) != JSON_TYPE_PAIR) { Serial.print(jsonparse_get_type(&jsonState)); Serial.println("-c"); return false; }
  if (jsonparse_next(&jsonState) != JSON_TYPE_ARRAY) {Serial.print(jsonparse_get_type(&jsonState)); Serial.println("-dd"); return false; }

  bool result;
  while (ParseAPInfo(&apInfo, &jsonState))
  {
    for (int i = 0; i < n; i++)
    {
      Serial.println("comparing: ");
      Serial.println(WiFi.SSID(i));
      Serial.println(apInfo.ssid);
      Serial.println(strcmp(WiFi.SSID(i), apInfo.ssid));

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
    //if ((nextTypeIndex = AssertParseNextMultiple(&jsonState, [']', ','], 2)) < 0)
    {
      Serial.println("Invalid list of stored WiFi APs!");
      return false;
    }
    if (nextTypeIndex == 0)
    {
      Serial.println("None of the stored WiFi APs was found!");
      break;
    }
  }

  Serial.println("ERROR: AutoConnect failed!");
  return false;
}
