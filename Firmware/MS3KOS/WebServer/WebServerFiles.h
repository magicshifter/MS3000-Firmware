#ifndef _WEBSERVER_ROUTES_H
#define _WEBSERVER_ROUTES_H

void handleNotFound() {
  if (msSystem.msESPWebServer.uri() != "/upload")
  {
    String message = "MS Command Not Found\n\n";
    message += "URI: ";
    message += msSystem.msESPWebServer.uri();
    message += "\nMethod: ";
    message += ( msSystem.msESPWebServer.method() == HTTP_GET ) ? "GET" : "POST";
    message += "\nArguments: ";
    message += msSystem.msESPWebServer.args();
    message += "\n";

    for ( uint8_t i = 0; i < msSystem.msESPWebServer.args(); i++ ) {
      message += " " + msSystem.msESPWebServer.argName ( i ) + ": " + msSystem.msESPWebServer.arg ( i ) + "\n";
    }

    msSystem.msESPWebServer.send ( 404, "text/plain", message );
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

    msSystem.msESPWebServer.send ( 200, "text/html", message );

  }
}

void handleReadFile()
{
  msSystem.msLEDs.fillPixels(0, 0, 1, 0x1F);
  msSystem.msLEDs.updatePixels();

  msSystem.logln("Free heap: ");
  msSystem.logln(String(ESP.getFreeHeap()));

  String message = "ReadFile:\n";


  if (msSystem.msESPWebServer.args() >= 1)
  {
    String args = msSystem.msESPWebServer.arg(0);

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

  msSystem.msESPWebServer.send ( 200, "text/html", message );
}





void handleFileList() {
  if (!msSystem.msESPWebServer.hasArg("dir")) {
    msSystem.msESPWebServer.send(500, "text/plain", "BAD ARGS, missing dir=");
    return;
  }
  String path = msSystem.msESPWebServer.arg("dir");

  //File entry;
  Dir dir = SPIFFS.openDir((char *)path.c_str());
  path = String();
 
/*
  if (!dir.isDirectory()){
    dir.close();
    msSystem.msESPWebServer.send(500, "text/plain", "NOT DIR");
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
      " <a href=\"read?file=" + name + "\">show</a></p>";
    //entry.close();
  }
  //dir.close();


    output += "</body>\
</html>";
	msSystem.msESPWebServer.send ( 200, "text/html", output );
}


/*
void handleFileListJson() {
  if (!msSystem.msESPWebServer.hasArg("dir")) {
    msSystem.msESPWebServer.send(500, "text/plain", "BAD ARGS, missing dir=");
    return;
  }
  String path = msSystem.msESPWebServer.arg("dir");

  File entry;
  File dir = FS.open((char *)path.c_str());
  path = String();

  if (!dir.isDirectory()){
    dir.close();
    msSystem.msESPWebServer.send(500, "text/plain", "NOT DIR");
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
  msSystem.msESPWebServer.send(200, "text/json", output);
}
*/

void handleFileUpload(){
  msSystem.logln("handle upload!");
  //if (msSystem.msESPWebServer.uri() != "/upload") return;
  HTTPUpload& upload = msSystem.msESPWebServer.upload();
  if (upload.status == UPLOAD_FILE_START)
  {
    strcpy(msGlobals.ggUploadFileName, upload.filename.c_str());//.c_str();
    msSystem.logln("upload started: ");
    msSystem.logln(msGlobals.ggUploadFileName);

    msGlobals.ggUploadFile = SPIFFS.open(msGlobals.ggUploadFileName, "w");

    if (!msGlobals.ggUploadFile) {
      msSystem.logln("ERROR: COULD NOT open file!!!");
    }

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
    msSystem.logln("Upload: WRITE, Bytes: ");
    msSystem.logln(String(upload.currentSize));
  }
  else if (upload.status == UPLOAD_FILE_END)
  {
    msGlobals.ggAFileSet = 1;

    msSystem.msEEPROMs.saveString(msGlobals.ggUploadFileName, MAX_FILENAME_LENGTH);
    if (msGlobals.ggUploadFile)
    {
      //bool result;
      //result =
      msGlobals.ggUploadFile.close();
      //if (!result) msSystem.logln("ERROR: could not close!");
    }
    msSystem.logln("Upload: END, Size: ");
    msSystem.logln(String(upload.totalSize));
  }
}

#endif
//_WEBSERVER_ROUTES_H
