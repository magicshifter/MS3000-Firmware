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
  if(msServer.hasArg("download")) return "application/octet-stream";
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
    msServer.streamFile(file, contentType);
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
  if(!streamFile(path)) msServer.send(404, "text/plain", "FileNotFound");
}

void StartWebServer(void)
{
  while (!AutoConnect())
  {
    msSystem.logln("ARGL: WHY U no WLAN!? :( retrying...");
    delay(100);
  }
#ifdef USE_MDNS
  if (msDNS.begin("magicshifter", WiFi.localIP()))
  {
    msSystem.logln( "MDNS responder started" );
  }
#endif
  msServer.on("/restart", []() {
    msServer.send(200, "text/plain", "going down for restarts now!");
    delay(1000);
    msSystem.restart();
  } );
  msServer.on("/kill", []() {
    msServer.send(200, "text/plain", "powering down now!");
    delay(1000);
    msSystem.powerDown();
  } );
  msServer.on("/info/about", HTTP_GET, handleGETAbout);
  msServer.on("/info/status", HTTP_GET, handleGETStatus);

  msServer.on("/settings/ap", HTTP_GET, handleGETAPSettings);
  msServer.on("/settings/ap/set", handlePOSTAPSettings);

  msServer.on("/settings/msServer", HTTP_GET, handleGETServerSettings);
  msServer.on("/settings/msServer/set", handlePOSTServerSettings);

  msServer.on("/settings/wifi/prefered", HTTP_GET, handleGETPreferdAPSettings);
  msServer.on("/settings/wifi/prefered/set", handlePOSTPreferedAPSettings);

  msServer.on("/settings/wifi/list", HTTP_GET, handleGETAPList);
  msServer.on("/settings/wifi/add", handlePOSTAPListAdd);
  msServer.on("/settings/wifi/delete", handlePOSTAPListDelete);

  msServer.on("/list", HTTP_GET, handleFileList);
  msServer.on("/leds", handleLedsSet);
  msServer.on("/read", handleReadFile);

  msServer.on("/led", handleLedSet);

  msServer.on("/mode", handleSetMode);

  msServer.on("/listwlans", HTTP_GET, handleGETWLANList);

  msServer.on("/download",  []() {
    if(!msServer.hasArg("file")) {
      msServer.send(500, "text/plain", "BAD ARGS, missing file=");
      return;
    }
    String path = msServer.arg("file");

    HandleServeStaticFile(path);
  });

  msServer.on("/",  []() {
    HandleServeStaticFile("index.html");
  });
  //msServer.on("favicon.gif", HandleServeStaticFile("favicon.gif"));
  //msServer.on("manifest.appcache", HandleServeStaticFile("manifest.appcache"));

  msServer.onFileUpload(handleFileUpload);

  msServer.on ( "/format", []() {
    String message = "formatin DISABLED cos of FS change, TODO: implement!!!11";
    //FS.format();
    msServer.send ( 200, "text/plain", "formated" );
  } );

  msServer.onNotFound ( handleNotFound );
  msServer.begin();
  msSystem.logln ( "HTTP msServer started" );
}


void HandleWebServer ( void ) {
#ifdef USE_MDNS
  // CRASHES in new toolchain 
  msDNS.update();
#endif
  msServer.handleClient();
}
