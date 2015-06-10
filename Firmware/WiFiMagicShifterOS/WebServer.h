
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Base64.h>

//#include <ArduinoJson.h>
#include "APA102.h"

#define USE_MDNS


#define BASE64_ENC_LEN(n) (((n) + 2 - (((n) + 2) % 3)) / 3 * 4);

/*
const char *ssid = "fnord";
const char *password = "fnord235";
*/
/*
const char *ssid = "metalab";
const char *password = "";
*/

const char *ssid = "wizard23";
//const char *ssid = "wizme";
const char *password = "lolinternets";

// const char* ssid = "MyPlace";
// const char* password = "gieb8nua4uns";


MDNSResponder mdns;
ESP8266WebServer server ( 80 );

// make it larger to be on the save side when base64 decoding
byte web_rgb_buffer[RGB_BUFFER_SIZE + 4];


const char *uploadname = "shroom_png.magicBitmap";
FSFile uploadFile;

void handleNotFound() {
  if (server.uri() != "/upload")
  {
    String message = "MS Command Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";

    for ( uint8_t i = 0; i < server.args(); i++ ) {
      message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
    }

    server.send ( 404, "text/plain", message );
  }
  else
  {
    String message = "<html>\
      <head>\
        <meta http-equiv='refresh' content='5'/>\
        <title>ESP8266 Demo</title>\
        <style>\
          body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
        </style>\
      </head>\
      <body>\
        <h1>Upload of " + String(uploadname) + " done</h1><a href=\"/list?dir=\">go back</a></body></html>";

        server.send ( 200, "text/html", message );

  }
}



// http://192.168.43.251/leds?set=[[0,4,4],[0,0,4],[4],[4,4,0,31],[0,4,4,31],[0,4,4,31],[0,0,4,31],[4,4,4,31],[4,4,0,31],[0,4,4,31],[0,4,4,31],[0,0,4,31],[4,4,4,31],[4,4,0,31],[0,4,4,31],[0,4,4,31]]
// http://192.168.43.251/leds?set=[[0,4,4],[0,0,4],[4]]

//http://192.168.43.251/leds?set=[[0,4,4],[1,1,0],[4],[8,1,0],[1,1,0],[4],[8,1,0],[1,1,0],[4],[8,1,0],[1],[1],[1],[1],[1],[1]]


void handleLedSet()
{
  fillPixels(0, 1, 0, 0x1F);
  updatePixels();
  byte ledData[200];

  String message = "LedSet\n\n";

  if (server.args() >= 1)
  {
    const char* input = server.arg(0).c_str();
    int inputLen = BASE64_ENC_LEN(RGB_BUFFER_SIZE);

    base64_decode((char *)web_rgb_buffer, input, inputLen);

    message += "done";
  }
  else
  {
    message += "argument missing!";
  }
  server.send ( 200, "text/plain",message );
}


void handleReadFile()
{
  fillPixels(0, 0, 1, 0x1F);
  updatePixels();

  Serial.print("Free heap: ");
  Serial.println(ESP.getFreeHeap());

  String message = "ReadFile:\n";


  if (server.args() >= 1)
  {
    String args = server.arg(0);
    const char* filename = args.c_str();

    message += "file: \"" + server.arg(0) /* String(filename)*/ + "\"\n";

    message += "file cast: \"" +  String(filename) + "\"\n";

    message += "file: \"" + args /* String(filename)*/ + "\"\n";


    byte buffer[105];
    FSFile file = FS.open(filename, FSFILE_READ);

    if (file)
    {
      int fileLen = file.available();
      Serial.print("file available: ");
      Serial.println(fileLen);

      if (fileLen > 0)
      {
        if (fileLen > 100) fileLen = 100 ;
        Serial.println("reading: ");
        Serial.println(file.read(buffer, fileLen));

        Serial.println("buffer: ");

        for (int i = 0; i < fileLen; i++) {
          Serial.print((char)buffer[i]);
          message += (char)buffer[i];
        }
      }

      file.close();
    }
    else
    {
      Serial.println("file not found");
      message += " file not found!";
    }
  }
  else
  {
    Serial.println("arg missing");
    message += "argument missing!";
  }

  server.send ( 200, "text/html", message );
}





void handleFileList() {
  if(!server.hasArg("dir")) {
    server.send(500, "text/plain", "BAD ARGS, missing dir=");
    return;
  }
  String path = server.arg("dir");

  FSFile entry;
  FSFile dir = FS.open((char *)path.c_str());
  path = String();
  if(!dir.isDirectory()){
    dir.close();
    server.send(500, "text/plain", "NOT DIR");
    return;
  }
  dir.rewindDirectory();

  String output = "<html>\
    <head>\
      <meta http-equiv='refresh' content='5'/>\
      <title>ESP8266 Demo</title>\
      <style>\
        body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
      </style>\
    </head>\
    <body>\
      <h1>Hello from ESP8266!</h1>";

  while(true){
    entry = dir.openNextFile();
    if (!entry)
      break;

    if(!FS.exists(entry.name())){
      os_printf("Entry[%s] Not Exists!\n", entry.name());
      entry.remove();
      entry.close();
      continue;
    }

    output += "<p><a href=\"read?file=" + String(entry.name()) + "\">" + String(entry.name()) + " : " + (entry.isDirectory()?"dir":"file") + " : " + entry.size() + "</a></p>";
    entry.close();
  }
  dir.close();


    output += "</body>\
</html>";
	server.send ( 200, "text/html", output );
}



void handleFileListJson() {
  if(!server.hasArg("dir")) {
    server.send(500, "text/plain", "BAD ARGS, missing dir=");
    return;
  }
  String path = server.arg("dir");

  FSFile entry;
  FSFile dir = FS.open((char *)path.c_str());
  path = String();
  if(!dir.isDirectory()){
    dir.close();
    server.send(500, "text/plain", "NOT DIR");
    return;
  }
  dir.rewindDirectory();

  String output = "[";
  while(true){
    entry = dir.openNextFile();
    if (!entry)
      break;

    if(!FS.exists(entry.name())){
      os_printf("Entry[%s] Not Exists!\n", entry.name());
      entry.remove();
      entry.close();
      continue;
    }

    if(output != "[")
      output += ',';
    output += "{\"type\":\"";
    output += (entry.isDirectory())?"dir":"file";
    output += "\",\"name\":\"";
    output += String(entry.name());
    output += "\",\"size\":\"";
    output += entry.size();
    output += "\"}";
    entry.close();
  }
  dir.close();

  output += "]";
  server.send(200, "text/json", output);
}

void handleFileUpload(){
  Serial.println("handle upload!");
  //if(server.uri() != "/upload") return;
  HTTPUpload& upload = server.upload();
  if(upload.status == UPLOAD_FILE_START)
  {
    uploadname = upload.filename.c_str();
    Serial.print("upload started: ");
    Serial.println(upload.filename.c_str());
    uploadFile = FS.open(upload.filename.c_str(), FSFILE_OVERWRITE);

    if (!uploadFile)
      Serial.println("ERROR: COULD NOT open file!!!");

    //if(SD.exists((char *)upload.filename.c_str())) SD.remove((char *)upload.filename.c_str());
    //uploadFile = SD.open(upload.filename.c_str(), FILE_WRITE);
    //DBG_OUTPUT_PORT.print("Upload: START, filename: "); DBG_OUTPUT_PORT.println(upload.filename);

  }
  else if(upload.status == UPLOAD_FILE_WRITE)
  {

    bool result;
    if(uploadFile)
    {
      result = uploadFile.write(upload.buf, upload.currentSize);
      if (!result) Serial.println("ERROR: could not write!");
    }
    Serial.print("Upload: WRITE, Bytes: ");
    Serial.println(upload.currentSize);
  }
  else if(upload.status == UPLOAD_FILE_END)
  {
    if(uploadFile)
    {
      //bool result;
      //result =
      uploadFile.close();
      //if (!result) Serial.println("ERROR: could not close!");
    }
    Serial.print("Upload: END, Size: ");
    Serial.println(upload.totalSize);
  }
}

void StartWebServer ( void ) {
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
    if (frame==0) Serial.println(".");
    delay ( 100 );
    Serial.print ( "." );
  }
  Serial.println ( "" );
  Serial.print ( "Connected to " );
  Serial.println ( ssid );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP() );

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