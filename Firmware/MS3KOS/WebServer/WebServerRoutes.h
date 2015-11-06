#ifndef _WEBSERVER_ROUTES_H
#define _WEBSERVER_ROUTES_H

void handleNotFound() {
  if (msSystem.msServer.uri() != "/upload")
  {
    String message = "MS Command Not Found\n\n";
    message += "URI: ";
    message += msSystem.msServer.uri();
    message += "\nMethod: ";
    message += ( msSystem.msServer.method() == HTTP_GET ) ? "GET" : "POST";
    message += "\nArguments: ";
    message += msSystem.msServer.args();
    message += "\n";

    for ( uint8_t i = 0; i < msSystem.msServer.args(); i++ ) {
      message += " " + msSystem.msServer.argName ( i ) + ": " + msSystem.msServer.arg ( i ) + "\n";
    }

    msSystem.msServer.send ( 404, "text/plain", message );
  }
  else
  {
    String message = "<html>\
      <head>\
        <meta http-equiv='refresh' content='5'/>\
        <title><a href=\"/\">MagicShifter3000</a> upload done</title>\
      </head>\
      <body>\
        <h1>Upload of " + String(msGlobals.uploadFileName) + " done</h1><a href=\"/list?dir=\">list</a><br><a href=\"\">index.html</a></body></html>";

    msSystem.msServer.send ( 200, "text/html", message );

  }
}

void handleReadFile()
{
  fillPixels(0, 0, 1, 0x1F);
  updatePixels();

  msSystem.log("Free heap: ");
  msSystem.logln(String(ESP.getFreeHeap()));

  String message = "ReadFile:\n";


  if (msSystem.msServer.args() >= 1)
  {
    String args = msSystem.msServer.arg(0);

    strcpy(msGlobals.uploadFileName, args.c_str());
    saveString(msGlobals.uploadFileName, MAX_FILENAME_LENGTH);


    message += "file name: \"" + args /* String(filename)*/ + "\"\n";

    //message += "file name strcpied: \"" +  msGlobals.uploadFileName + "\"\n";


    byte buffer[105];
    File file = SPIFFS.open(msGlobals.uploadFileName, "r");

    if (file)
    {
      int fileLen = file.available();
      msSystem.log("file available: ");
      msSystem.logln(String(fileLen));

      if (fileLen > 0)
      {
        if (fileLen > 100) fileLen = 100 ;
        msSystem.logln("reading: ");
        msSystem.logln(String(file.read(buffer, fileLen)));

        msSystem.logln("buffer: ");

        for (int i = 0; i < fileLen; i++) {
          msSystem.log((char)buffer[i]);
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

  msSystem.msServer.send ( 200, "text/html", message );
}





void handleFileList() {
  if (!msSystem.msServer.hasArg("dir")) {
    msSystem.msServer.send(500, "text/plain", "BAD ARGS, missing dir=");
    return;
  }
  String path = msSystem.msServer.arg("dir");

  //File entry;
  Dir dir = SPIFFS.openDir((char *)path.c_str());
  path = String();
 
/*
  if (!dir.isDirectory()){
    dir.close();
    msSystem.msServer.send(500, "text/plain", "NOT DIR");
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
	msSystem.msServer.send ( 200, "text/html", output );
}


/*
void handleFileListJson() {
  if (!msSystem.msServer.hasArg("dir")) {
    msSystem.msServer.send(500, "text/plain", "BAD ARGS, missing dir=");
    return;
  }
  String path = msSystem.msServer.arg("dir");

  File entry;
  File dir = FS.open((char *)path.c_str());
  path = String();

  if (!dir.isDirectory()){
    dir.close();
    msSystem.msServer.send(500, "text/plain", "NOT DIR");
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
  msSystem.msServer.send(200, "text/json", output);
}
*/

void handleFileUpload(){
  msSystem.logln("handle upload!");
  //if (msSystem.msServer.uri() != "/upload") return;
  HTTPUpload& upload = msSystem.msServer.upload();
  if (upload.status == UPLOAD_FILE_START)
  {
    strcpy(msGlobals.uploadFileName, upload.filename.c_str());//.c_str();
    msSystem.log("upload started: ");
    msSystem.logln(msGlobals.uploadFileName);

    msGlobals.uploadFile = SPIFFS.open(msGlobals.uploadFileName, "w");

    if (!msGlobals.uploadFile) {
      msSystem.logln("ERROR: COULD NOT open file!!!");
    }

    //if (SD.exists((char *)upload.filename.c_str())) SD.remove((char *)upload.filename.c_str());
    //msGlobals.uploadFile = SD.open(upload.filename.c_str(), FILE_WRITE);
    //DBG_OUTPUT_PORT.print("Upload: START, filename: "); DBG_OUTPUT_PORT.println(upload.filename);

  }
  else if (upload.status == UPLOAD_FILE_WRITE)
  {

    bool result;
    if (msGlobals.uploadFile)
    {
      result = msGlobals.uploadFile.write(upload.buf, upload.currentSize);
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
    msGlobals.setActiveFile = 1;

    saveString(msGlobals.uploadFileName, MAX_FILENAME_LENGTH);
    if (msGlobals.uploadFile)
    {
      //bool result;
      //result =
      msGlobals.uploadFile.close();
      //if (!result) msSystem.logln("ERROR: could not close!");
    }
    msSystem.log("Upload: END, Size: ");
    msSystem.logln(String(upload.totalSize));
  }
}

#endif
//_WEBSERVER_ROUTES_H
