#ifndef _WEBSERVER_ROUTES_H
#define _WEBSERVER_ROUTES_H



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



void handleNotFound() {

  char fName[MAX_FILENAME_LENGTH];
  msSystem.msEEPROMs.safeStrncpy(fName, msSystem.msESPServer.uri().c_str(), MAX_FILENAME_LENGTH);

  msSystem.log("URI to retrieve: ");
  msSystem.logln(msSystem.msESPServer.uri());
  msSystem.log("fName to retrieve: ");
  msSystem.logln(String(fName + 1));

  String gzFName = String(fName + 1) + ".gz";

  // if we don't have a route for the request, but do have a file, serve the file
  if (SPIFFS.exists(fName + 1) || SPIFFS.exists(gzFName.c_str())) {
    streamFile(String(fName + 1));
  }
else
  if (msSystem.msESPServer.uri() != "/upload")
  {
    String message = "MS Command Not Found\n\n";
    message += "URI: ";
    message += msSystem.msESPServer.uri();
    message += "\nMethod: ";
    message += ( msSystem.msESPServer.method() == HTTP_GET ) ? "GET" : "POST";
    message += "\nArguments: ";
    message += msSystem.msESPServer.args();
    message += "\n";

    for ( uint8_t i = 0; i < msSystem.msESPServer.args(); i++ ) {
      message += " " + msSystem.msESPServer.argName ( i ) + ": " + msSystem.msESPServer.arg ( i ) + "\n";
    }

    msSystem.msESPServer.send ( 404, "text/plain", message );
  }
  else
  {
    String message = "<html>\
      <head>\
        <meta http-equiv='refresh' content='5'/>\
        <title><a href=\"/\">MagicShifter3000</a> upload done</title>\
      </head>\
      <body>\
        <h1>Upload of " + String(msGlobals.ggUploadFileName) + " done</h1><a href=\"/list?dir=\">list</a><br><a href=\"\">index.html</a></body></html>";

    msSystem.msESPServer.sendHeader("Access-Control-Allow-Origin", "*");
    msSystem.msESPServer.send ( 200, "text/html", message );

  }
}


void handleReadFile()
{
  msSystem.msLEDs.fillPixels(0, 0, 1, 0x1F);
  msSystem.msLEDs.updatePixels();

  msSystem.logln("Free heap: ");
  msSystem.logln(String(ESP.getFreeHeap()));

  String message = "ReadFile:\n";


  if (msSystem.msESPServer.args() >= 1)
  {
    String args = msSystem.msESPServer.arg(0);

    strcpy(msGlobals.ggUploadFileName, args.c_str());
    msSystem.msEEPROMs.saveString(msGlobals.ggUploadFileName, MAX_FILENAME_LENGTH);


    message += "file name: \"" + args /* String(filename)*/ + "\"\n";

    //message += "file name strcpied: \"" +  msGlobals.ggUploadFileName + "\"\n";


    byte buffer[105];
    File file = SPIFFS.open(msGlobals.ggUploadFileName, "r");

    if (file)
    {
      int fileLen = file.available();
      msSystem.logln("file available: ");
      msSystem.logln(String(fileLen));

      if (fileLen > 0)
      {
        if (fileLen > 100) fileLen = 100 ;
        msSystem.logln("reading: ");
        msSystem.logln(String(file.read(buffer, fileLen)));

        msSystem.logln("buffer: ");

        for (int i = 0; i < fileLen; i++) {
          msSystem.logln(String((char)buffer[i]));
          message += (char)buffer[i];
        }
      }

      file.close();
    }
    else
    {
      msSystem.logln("file not found");
      message += " file not found!";
    }
  }
  else
  {
    msSystem.logln("arg missing");
    message += "argument missing!";
  }

  msSystem.msESPServer.send ( 200, "text/html", message );
}


void handleDeleteFile()
{
  msSystem.msLEDs.fillPixels(0, 0, 1, 0x1F);
  msSystem.msLEDs.updatePixels();

  String message = "handleDeleteFile\n";
  if (msSystem.msESPServer.args() >= 1)
  {
    String args = msSystem.msESPServer.arg(0);
    message += "Trying to delete file: \"" + args + "\"\n";

    if (SPIFFS.exists(args)) {
      SPIFFS.remove(args);
      msSystem.logln("file deleted");
      message += "file deleted!";
    }
    else
    {
      msSystem.logln("file not found");
      message += "file not found!";
    }
  }
  else
  {
    msSystem.logln("arg missing");
    message += "argument missing!";
  }

  msSystem.msESPServer.send ( 200, "text/html", message );
}





void handleFileList() {
  if (!msSystem.msESPServer.hasArg("dir")) {
    msSystem.msESPServer.send(500, "text/plain", "BAD ARGS, missing dir=");
    return;
  }
  String path = msSystem.msESPServer.arg("dir");

// msSystem.log("dir path::::"); msSystem.logln((char *)path.c_str());

  //File entry;
  Dir dir = SPIFFS.openDir((char *)path.c_str());
  path = String();
 
/*
  if (!dir.isDirectory()){
    dir.close();
    msSystem.msESPServer.send(500, "text/plain", "NOT DIR");
    return;
  }
  */
  //dir.rewindDirectory();

  String output = "<html>\
    <head>\
      <meta http-equiv='refresh' content='5'/>\
      <title>MagicShifter3000 directory list</title>\
    </head>\
    <body>\
      <h1>/" + path + "</h1>";

  while(true){
    if (!dir.next())
      break;

/*
    if (!FS.exists(entry.name())){
      os_printf("Entry[%s] Not Exists!\n", entry.name());
      entry.remove();
      entry.close();
      continue;
    }
*/
    String name = dir.fileName();

    output += "<p><a href=\"download?file=" + name + "\">" +
      name + "</a> " +
    //  (entry.isDirectory()?"dir":"file") + " : " + entry.size() +
      " <a href=\"delete?file=" + name + "\">delete file</a></p>";
    //entry.close();
  }
  //dir.close();


    output += "</body>\
</html>";
	msSystem.msESPServer.send ( 200, "text/html", output );
}


/*
void handleFileListJson() {
  if (!msSystem.msESPServer.hasArg("dir")) {
    msSystem.msESPServer.send(500, "text/plain", "BAD ARGS, missing dir=");
    return;
  }
  String path = msSystem.msESPServer.arg("dir");

  File entry;
  File dir = FS.open((char *)path.c_str());
  path = String();

  if (!dir.isDirectory()){
    dir.close();
    msSystem.msESPServer.send(500, "text/plain", "NOT DIR");
    return;
  }
  dir.rewindDirectory();

  String output = "[";
  while(true){
    entry = dir.openNextFile();
    if (!entry)
      break;

    if (!FS.exists(entry.name())){
      os_printf("Entry[%s] Not Exists!\n", entry.name());
      entry.remove();
      entry.close();
      continue;
    }

    if (output != "[")
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
  msSystem.msESPServer.send(200, "text/json", output);
}
*/

void handleFileUpload(){

  msSystem.logln("handle upload!");

  //if (msSystem.msESPServer.uri() != "/upload") return;
  HTTPUpload& upload = msSystem.msESPServer.upload();

  if (upload.status == UPLOAD_FILE_START)
  {
    msSystem.msEEPROMs.safeStrncpy(msGlobals.ggUploadFileName, (char *)upload.filename.c_str(), MAX_FILENAME_LENGTH);//.c_str();
    msSystem.logln("upload started: ");
    msSystem.logln(msGlobals.ggUploadFileName);

    msSystem.log("upload open.. ");

    if (SPIFFS.exists(msGlobals.ggUploadFileName)) {
      msSystem.logln("Removing previous copy..");
      SPIFFS.remove(msGlobals.ggUploadFileName);
      
    }

    msGlobals.ggUploadFile = SPIFFS.open(msGlobals.ggUploadFileName, "w+");
    msSystem.log("ggUploadFile opened:");
    msSystem.logln(msGlobals.ggUploadFileName);

    if (!msGlobals.ggUploadFile) {
      msSystem.logln("ERROR: COULD NOT open file!!!");
    }
    else
      msSystem.logln("Opened file for writing...");

    //if (SD.exists((char *)upload.filename.c_str())) SD.remove((char *)upload.filename.c_str());
    //msGlobals.ggUploadFile = SD.open(upload.filename.c_str(), FILE_WRITE);
    //DBG_OUTPUT_PORT.print("Upload: START, filename: "); DBG_OUTPUT_PORT.println(upload.filename);

  }
  else if (upload.status == UPLOAD_FILE_WRITE)
  {

    bool result;
    if (msGlobals.ggUploadFile)
    {
      result = msGlobals.ggUploadFile.write(upload.buf, upload.currentSize);
      if (!result) 
      {
        msSystem.logln("ERROR: could not write!");
      }
    }
    msSystem.log("Upload: WRITE, Bytes: ");
    msSystem.logln(String(upload.currentSize));
  }
  else if (upload.status == UPLOAD_FILE_END)
  {
    msGlobals.ggShouldAutoLoad = 1;

    msSystem.msEEPROMs.saveString(msGlobals.ggUploadFileName, MAX_FILENAME_LENGTH);
    
    msGlobals.ggUploadFile.close();

    msSystem.logln("Upload: END, Size: ");
    msSystem.logln(String(upload.totalSize));
  }
}

#endif
//_WEBSERVER_ROUTES_H
