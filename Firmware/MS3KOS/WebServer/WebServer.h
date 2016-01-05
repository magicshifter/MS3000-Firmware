//
// Web services 
//


#include "WebServerAPI.h"
#include "WebServerAutoConnect.h"
#include "WebServerFiles.h"

class MagicShifterWebServer {

private:
// TODO: sensible syslog config
#ifdef CONFIG_USE_DEBUG_AP
  const char *jsonLastAP =
  //"{\"ssid\":\"PACIFIC\", \"pwd\":\"AllesR0ger\"}";
  //"{\"ssid\":\"wizard23\", \"pwd\":\"lolinternets\"}";
  "{\"ssid\":\"wizme\", \"pwd\":\"lolinternets\"}";

  const char *jsonAPList = "{\"list\": [\
  {\"ssid\":\"metalab\", \"pwd\":\"\"},\
  {\"ssid\":\"wizard23\", \"pwd\":\"lolinternets\"},\
  {\"ssid\":\"wizme\", \"pwd\":\"lolinternets\"}\
  ]}";
  const char *jsonSoftAP = "{\"ssid\":\"MagicShifter3000\", \"pwd\":\"\"}";
#endif


static String getContentType(String filename){
  if (msSystem.msESPWebServer.hasArg("download")) return "application/octet-stream";
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

static bool streamFile(String path){
  
  if (path.endsWith("/")) {
    path += "index.htm";
  }
  String contentType = getContentType(path);
  if (SPIFFS.exists((char *)(path+".gz").c_str()) || SPIFFS.exists((char *)path.c_str())){
    if (SPIFFS.exists((char *)(path+".gz").c_str())) {
      path += ".gz";
    }
    File file = SPIFFS.open((char *)path.c_str(), "r");
    msSystem.msESPWebServer.streamFile(file, contentType);
    file.close();
    return true;
  }
  else
  {
    msSystem.logln("streamFile fail:");
    msSystem.logln(path.c_str());
  }
  
  return false;
}

static void HandleServeStaticFile(String path)
{
  if (!streamFile(path)) {
    msSystem.msESPWebServer.send(404, "text/plain", "FileNotFound");
  }
}


public:

void StartWebServer(void)
{
  while (!AutoConnect())
  {
    msSystem.logln("ARGL: WHY U no WLAN!? :( retrying...");
    delay(100);
  }
#ifdef USE_MDNS
  if (msSystem.msDNS.begin("magicshifter", WiFi.localIP()))
  {
    msSystem.logln( "MDNS responder started" );
  }
#endif
  msSystem.msESPWebServer.on("/restart", []() {
    msSystem.msESPWebServer.send(200, "text/plain", "going down for restarts now!");
    delay(1000);
    msSystem.restart();
  } );
  msSystem.msESPWebServer.on("/kill", []() {
    msSystem.msESPWebServer.send(200, "text/plain", "powering down now!");
    delay(1000);
    msSystem.powerDown();
  } );

  msSystem.msESPWebServer.on("/info", HTTP_GET, handleGETInfo);

  msSystem.msESPWebServer.on("/info/about", HTTP_GET, handleGETAbout);
  msSystem.msESPWebServer.on("/info/status", HTTP_GET, handleGETStatus);

  msSystem.msESPWebServer.on("/settings", HTTP_GET, handleGetSettings);

  msSystem.msESPWebServer.on("/settings/ap", HTTP_GET, handleGETAPSettings);
  msSystem.msESPWebServer.on("/settings/ap/set", handlePOSTAPSettings);

  msSystem.msESPWebServer.on("/settings/server", HTTP_GET, handleGETServerSettings);
  msSystem.msESPWebServer.on("/settings/server/set", handlePOSTServerSettings);

  msSystem.msESPWebServer.on("/settings/wifi/preferred", HTTP_GET, handleGETPreferredAPSettings);
  msSystem.msESPWebServer.on("/settings/wifi/preferred/set", handlePOSTPreferredAPSettings);

  msSystem.msESPWebServer.on("/settings/wifi/list", HTTP_GET, handleGETAPList);
  msSystem.msESPWebServer.on("/settings/wifi/add", handlePOSTAPListAdd);
  msSystem.msESPWebServer.on("/settings/wifi/delete", handlePOSTAPListDelete);

  msSystem.msESPWebServer.on("/list", HTTP_GET, handleFileList);
  msSystem.msESPWebServer.on("/leds", handleLedsSet);
  msSystem.msESPWebServer.on("/read", handleReadFile);

  msSystem.msESPWebServer.on("/led", handleLedSet);

  msSystem.msESPWebServer.on("/mode", handleSetMode);

  msSystem.msESPWebServer.on("/time", HTTP_GET, handleGETTime);
  msSystem.msESPWebServer.on("/time", HTTP_POST, handlePOSTTime);

  msSystem.msESPWebServer.on("/listwlans", HTTP_GET, handleGETWLANList);

  msSystem.msESPWebServer.on("/download",  []() {
    if (!msSystem.msESPWebServer.hasArg("file")) {
      msSystem.msESPWebServer.send(500, "text/plain", "BAD ARGS, missing file=");
      return;
    }
    String path = msSystem.msESPWebServer.arg("file");

    HandleServeStaticFile(path);
  });

  msSystem.msESPWebServer.on("/",  []() {
    HandleServeStaticFile("index.html");
  });
  //msSystem.msESPWebServer.on("favicon.gif", HandleServeStaticFile("favicon.gif"));
  //msSystem.msESPWebServer.on("manifest.appcache", HandleServeStaticFile("manifest.appcache"));

  msSystem.msESPWebServer.onFileUpload(handleFileUpload);

  msSystem.msESPWebServer.on ( "/format", []() {
    String message = "formatin DISABLED cos of FS change, TODO: implement!!!11";
    //FS.format();
    msSystem.msESPWebServer.send ( 200, "text/plain", "formated" );
  } );

  msSystem.msESPWebServer.onNotFound ( handleNotFound );
  msSystem.msESPWebServer.begin();
  msSystem.logln ( "HTTP msSystem.msESPWebServer started" );
}


void HandleWebServer ( void ) {
#ifdef USE_MDNS
  // CRASHES in new toolchain 
  msSystem.msDNS.update();
#endif
  msSystem.msESPWebServer.handleClient();
}

};
