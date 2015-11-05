//
// Web services 
//

#include "WebServerAPI.h"
#include "WebServerAutoConnect.h"
#include "WebServerRoutes.h"

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

String getContentType(String filename){
  if(msSystem.msServer.hasArg("download")) return "application/octet-stream";
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
  if(SPIFFS.exists((char *)(path+".gz").c_str()) || SPIFFS.exists((char *)path.c_str())){
    if(SPIFFS.exists((char *)(path+".gz").c_str()))
      path += ".gz";
    File file = SPIFFS.open((char *)path.c_str(), "r");
    msSystem.msServer.streamFile(file, contentType);
    file.close();
    return true;
  }
  else
  {
    msSystem.log("streamFile fail:");
    msSystem.logln(path.c_str());
  }
  
  return false;
}

void HandleServeStaticFile(String path)
{
  if(!streamFile(path)) msSystem.msServer.send(404, "text/plain", "FileNotFound");
}

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
  msSystem.msServer.on("/restart", []() {
    msSystem.msServer.send(200, "text/plain", "going down for restarts now!");
    delay(1000);
    msSystem.restart();
  } );
  msSystem.msServer.on("/kill", []() {
    msSystem.msServer.send(200, "text/plain", "powering down now!");
    delay(1000);
    msSystem.powerDown();
  } );
  msSystem.msServer.on("/info/about", HTTP_GET, handleGETAbout);
  msSystem.msServer.on("/info/status", HTTP_GET, handleGETStatus);

  msSystem.msServer.on("/settings/ap", HTTP_GET, handleGETAPSettings);
  msSystem.msServer.on("/settings/ap/set", handlePOSTAPSettings);

  msSystem.msServer.on("/settings/server", HTTP_GET, handleGETServerSettings);
  msSystem.msServer.on("/settings/server/set", handlePOSTServerSettings);

  msSystem.msServer.on("/settings/wifi/prefered", HTTP_GET, handleGETPreferdAPSettings);
  msSystem.msServer.on("/settings/wifi/prefered/set", handlePOSTPreferedAPSettings);

  msSystem.msServer.on("/settings/wifi/list", HTTP_GET, handleGETAPList);
  msSystem.msServer.on("/settings/wifi/add", handlePOSTAPListAdd);
  msSystem.msServer.on("/settings/wifi/delete", handlePOSTAPListDelete);

  msSystem.msServer.on("/list", HTTP_GET, handleFileList);
  msSystem.msServer.on("/leds", handleLedsSet);
  msSystem.msServer.on("/read", handleReadFile);

  msSystem.msServer.on("/led", handleLedSet);

  msSystem.msServer.on("/mode", handleSetMode);

  msSystem.msServer.on("/listwlans", HTTP_GET, handleGETWLANList);

  msSystem.msServer.on("/download",  []() {
    if(!msSystem.msServer.hasArg("file")) {
      msSystem.msServer.send(500, "text/plain", "BAD ARGS, missing file=");
      return;
    }
    String path = msSystem.msServer.arg("file");

    HandleServeStaticFile(path);
  });

  msSystem.msServer.on("/",  []() {
    HandleServeStaticFile("index.html");
  });
  //msSystem.msServer.on("favicon.gif", HandleServeStaticFile("favicon.gif"));
  //msSystem.msServer.on("manifest.appcache", HandleServeStaticFile("manifest.appcache"));

  msSystem.msServer.onFileUpload(handleFileUpload);

  msSystem.msServer.on ( "/format", []() {
    String message = "formatin DISABLED cos of FS change, TODO: implement!!!11";
    //FS.format();
    msSystem.msServer.send ( 200, "text/plain", "formated" );
  } );

  msSystem.msServer.onNotFound ( handleNotFound );
  msSystem.msServer.begin();
  msSystem.logln ( "HTTP msSystem.msServer started" );
}


void HandleWebServer ( void ) {
#ifdef USE_MDNS
  // CRASHES in new toolchain 
  msSystem.msDNS.update();
#endif
  msSystem.msServer.handleClient();
}
