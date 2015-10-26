#define FILENAME_LENGTH 100

#include "msGlobals.h"
extern MidiShifterGlobals msGlobals;

// #include "MagicShakeMode.h"
// extern MagicShakeMode msMagicShakeMode;


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
        <title><a href=\"/\">MagicShifter3000</a> upload done</title>\
      </head>\
      <body>\
        <h1>Upload of " + String(msGlobals.uploadFileName) + " done</h1><a href=\"/list?dir=\">list</a><br><a href=\"\">index.html</a></body></html>";

    server.send ( 200, "text/html", message );

  }
}

void handleReadFile()
{
  fillPixels(0, 0, 1, 0x1F);
  updatePixels();

  msSystem.log("Free heap: ");
  msSystem.logln(String(ESP.getFreeHeap()));

  String message = "ReadFile:\n";


  if (server.args() >= 1)
  {
    String args = server.arg(0);

    strcpy(msGlobals.uploadFileName, args.c_str());
    saveString(msGlobals.uploadFileName, FILENAME_LENGTH);


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

  server.send ( 200, "text/html", message );
}





void handleFileList() {
  if(!server.hasArg("dir")) {
    server.send(500, "text/plain", "BAD ARGS, missing dir=");
    return;
  }
  String path = server.arg("dir");

  //File entry;
  Dir dir = SPIFFS.openDir((char *)path.c_str());
  path = String();
 
/*
  if(!dir.isDirectory()){
    dir.close();
    server.send(500, "text/plain", "NOT DIR");
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
    if(!FS.exists(entry.name())){
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
	server.send ( 200, "text/html", output );
}


/*
void handleFileListJson() {
  if(!server.hasArg("dir")) {
    server.send(500, "text/plain", "BAD ARGS, missing dir=");
    return;
  }
  String path = server.arg("dir");

  File entry;
  File dir = FS.open((char *)path.c_str());
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
*/

void handleFileUpload(){
  msSystem.logln("handle upload!");
  //if(server.uri() != "/upload") return;
  HTTPUpload& upload = server.upload();
  if(upload.status == UPLOAD_FILE_START)
  {
    strcpy(msGlobals.uploadFileName, upload.filename.c_str());//.c_str();
    msSystem.log("upload started: ");
    msSystem.logln(msGlobals.uploadFileName);

    msGlobals.uploadFile = SPIFFS.open(msGlobals.uploadFileName, "w");

    if (!msGlobals.uploadFile) {
      msSystem.logln("ERROR: COULD NOT open file!!!");
    }

    //if(SD.exists((char *)upload.filename.c_str())) SD.remove((char *)upload.filename.c_str());
    //msGlobals.uploadFile = SD.open(upload.filename.c_str(), FILE_WRITE);
    //DBG_OUTPUT_PORT.print("Upload: START, filename: "); DBG_OUTPUT_PORT.println(upload.filename);

  }
  else if(upload.status == UPLOAD_FILE_WRITE)
  {

    bool result;
    if(msGlobals.uploadFile)
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
  else if(upload.status == UPLOAD_FILE_END)
  {
    msGlobals.setActiveFile = 1;

    saveString(msGlobals.uploadFileName, FILENAME_LENGTH);
    if(msGlobals.uploadFile)
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
