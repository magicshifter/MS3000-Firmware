
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Base64.h>

#include "Config.h"
#include "APA102.h"

#define USE_MDNS

MDNSResponder mdns;
ESP8266WebServer server (80);


void loadString(char * str, int len)
{
  for (int i = 0; i < len; i++)
  {

    str[i] = EEPROM.read(i);
    Serial.print("readin: ");
    Serial.print(str[i]);
    Serial.print(" : ");
    Serial.println((int)str[i]);
  }
  str[len-1] = '\0';
}

void saveString(char * str, int len)
{
  for (int i = 0; i < len; i++)
  {
    EEPROM.write(i, str[i]);
    Serial.print("writtin: ");
    Serial.print(str[i]);
    Serial.print(" back: ");
    Serial.println((int)EEPROM.read(i));
  }
  EEPROM.commit();

  for (int i = 0; i < len; i++)
  {
    Serial.print("wri: ");
    Serial.print(str[i]);
    Serial.print(" back: ");
    Serial.println((int)EEPROM.read(i));
  }
}

#include "WebServerSettings.h"
#include "WebServerRoutes.h"

String getContentType(String filename){
  if(server.hasArg("download")) return "application/octet-stream";
  else if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool streamFile(String path){
  if(path.endsWith("/")) path += "index.htm";
  String contentType = getContentType(path);
  if(FS.exists((char *)(path+".gz").c_str()) || FS.exists((char *)path.c_str())){
    if(FS.exists((char *)(path+".gz").c_str()))
      path += ".gz";
    FSFile file = FS.open((char *)path.c_str());
    server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

void HandleServeStaticFile(String path)
{
  if(!streamFile(path)) server.send(404, "text/plain", "FileNotFound");
}

void StartWebServer(void)
{

// TODO: refactor into settings, add mode :)
  EEPROM.begin(512);

  loadString(uploadname, FILENAME_LENGTH);

  if (!FS.exists(uploadname))
  {
    Serial.print("could not find: ");
    Serial.println(uploadname);
    strcpy(uploadname, "default.magicBitmap");
  }
  Serial.print("using POV file: ");
  Serial.println(uploadname);

/*
  Serial.print("connecting to AP: ");
  Serial.println(ssid);
  WiFi.begin ( ssid, password );


  // Wait for connection
  int frame = 0;
  while ( WiFi.status() != WL_CONNECTED ) {
    for (int i = 0; i < LEDS; i++)
    {
      if (i < frame)
        setPixel(i, 1, 1, 1, 0x1);
      else
        setPixel(i, 0, 0, 0, 0);
    }
    updatePixels();
    frame = (frame + 1) % LEDS;
    if (frame%24 == 0) Serial.print(".");
    delay ( 20 );
  }
  Serial.println ( "" );
  Serial.print ( "Connected to " );
  Serial.println ( ssid );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP() );
*/
while (!AutoConnect())
{
  Serial.println("ARGL: WHY U no WLAN!? :( retrying...");
  delay(100);
}
#ifdef USE_MDNS
  if ( mdns.begin ( "magicshifter", WiFi.localIP() ) ) {
    Serial.println ( "MDNS responder started" );
  }
#endif

  //server.on ( "/", handleRoot );
  //server.on ( "/ex", handleRootEx );
  server.on("/list", HTTP_GET, handleFileList);
  server.on ("/leds", handleLedSet );
  server.on("/read", handleReadFile);

  server.on("/",  []() {
    HandleServeStaticFile("index.html");
  });

  //server.on("favicon.gif", HandleServeStaticFile("favicon.gif"));
  //server.on("manifest.appcache", HandleServeStaticFile("manifest.appcache"));

  server.onFileUpload(handleFileUpload);

  server.on ( "/inline", []() {
    server.send ( 200, "text/plain", "this works as well" );
  } );

  server.on ( "/format", []() {
    String message = "formatin...";
    FS.format();
    server.send ( 200, "text/plain", "formated" );
  } );

  server.onNotFound ( handleNotFound );
  server.begin();
  Serial.println ( "HTTP server started" );
}


void HandleWebServer ( void ) {
#ifdef USE_MDNS
  mdns.update();
#endif
  server.handleClient();
}
