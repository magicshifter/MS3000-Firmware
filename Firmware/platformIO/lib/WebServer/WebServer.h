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
  if (msSystem.msESPServer.hasArg("download")) return "application/octet-stream";
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
    msSystem.msESPServer.streamFile(file, contentType);
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
    msSystem.msESPServer.send(404, "text/plain", "FileNotFound");
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
  msSystem.msESPServer.on("/restart", []() {
    msSystem.msESPServer.send(200, "text/plain", "going down for restarts now!");
    delay(1000);
    msSystem.restart();
  } );
  msSystem.msESPServer.on("/kill", []() {
    msSystem.msESPServer.send(200, "text/plain", "powering down now!");
    delay(1000);
    msSystem.powerDown();
  } );

  msSystem.msESPServer.on("/info", HTTP_GET, handleGETInfo);

  msSystem.msESPServer.on("/info/about", HTTP_GET, handleGETAbout);
  msSystem.msESPServer.on("/info/status", HTTP_GET, handleGETStatus);

  msSystem.msESPServer.on("/settings", HTTP_GET, handleGetSettings);

  msSystem.msESPServer.on("/settings/ap", HTTP_GET, handleGETAPSettings);
  msSystem.msESPServer.on("/settings/ap/set", handlePOSTAPSettings);

  msSystem.msESPServer.on("/settings/server", HTTP_GET, handleGETServerSettings);
  msSystem.msESPServer.on("/settings/server/set", handlePOSTServerSettings);

  msSystem.msESPServer.on("/settings/wifi/preferred", HTTP_GET, handleGETPreferredAPSettings);
  msSystem.msESPServer.on("/settings/wifi/preferred/set", handlePOSTPreferredAPSettings);

  msSystem.msESPServer.on("/settings/wifi/list", HTTP_GET, handleGETAPList);
  msSystem.msESPServer.on("/settings/wifi/add", handlePOSTAPListAdd);
  msSystem.msESPServer.on("/settings/wifi/delete", handlePOSTAPListDelete);

  msSystem.msESPServer.on("/list", HTTP_GET, handleFileList);
  msSystem.msESPServer.on("/leds", handleLedsSet);
  msSystem.msESPServer.on("/read", handleReadFile);

  msSystem.msESPServer.on("/led", handleLedSet);

  msSystem.msESPServer.on("/mode", handleSetMode);

  msSystem.msESPServer.on("/time", HTTP_GET, handleGETTime);
  msSystem.msESPServer.on("/time", HTTP_POST, handlePOSTTime);

  msSystem.msESPServer.on("/listwlans", HTTP_GET, handleGETWLANList);

  msSystem.msESPServer.on("/download",  []() {
    if (!msSystem.msESPServer.hasArg("file")) {
      msSystem.msESPServer.send(500, "text/plain", "BAD ARGS, missing file=");
      return;
    }
    String path = msSystem.msESPServer.arg("file");

    HandleServeStaticFile(path);
  });

  msSystem.msESPServer.on("/",  []() {
    HandleServeStaticFile("index.html");
  });
  //msSystem.msESPServer.on("favicon.gif", HandleServeStaticFile("favicon.gif"));
  //msSystem.msESPServer.on("manifest.appcache", HandleServeStaticFile("manifest.appcache"));

//  msSystem.msESPServer.onFileUpload(handleFileUpload);
  msSystem.msESPServer.on("/upload", HTTP_POST, [](){ msSystem.msESPServer.send(200, "text/plain", ""); }, handleFileUpload);


  msSystem.msESPServer.on ( "/format", []() {
    String message = "formatin DISABLED cos of FS change, TODO: implement!!!11";
    //FS.format();
    msSystem.msESPServer.send ( 200, "text/plain", "formated" );
  } );

  msSystem.msESPServer.onNotFound ( handleNotFound );
  msSystem.msESPServer.begin();
  msSystem.logln ( "HTTP msSystem.msESPServer started" );
}


void HandleWebServer ( void ) {
#ifdef USE_MDNS
  // CRASHES in new toolchain 
  msSystem.msDNS.update();
#endif
  msSystem.msESPServer.handleClient();
}

};
