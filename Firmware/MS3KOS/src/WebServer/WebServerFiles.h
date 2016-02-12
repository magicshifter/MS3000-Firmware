#ifndef _WEBSERVER_ROUTES_H
#define _WEBSERVER_ROUTES_H



static String getContentType(String filename)
{
	if (msSystem.msESPServer.hasArg("download"))
		return "application/octet-stream";
	else if (filename.endsWith(".htm"))
		return "text/html";
	else if (filename.endsWith(".html"))
		return "text/html";
	else if (filename.endsWith(".css"))
		return "text/css";
	else if (filename.endsWith(".js"))
		return "application/javascript";
	else if (filename.endsWith(".png"))
		return "image/png";
	else if (filename.endsWith(".gif"))
		return "image/gif";
	else if (filename.endsWith(".jpg"))
		return "image/jpeg";
	else if (filename.endsWith(".ico"))
		return "image/x-icon";
	else if (filename.endsWith(".xml"))
		return "text/xml";
	else if (filename.endsWith(".pdf"))
		return "application/x-pdf";
	else if (filename.endsWith(".zip"))
		return "application/x-zip";
	else if (filename.endsWith(".gz"))
		return "application/x-gzip";
	return "text/plain";
}

static bool streamFile(String path)
{

	if (path.endsWith("/")) {
		path += "index.htm";
	}
	String contentType = getContentType(path);
	if (SPIFFS.exists((char *) (path + ".gz").c_str())
		|| SPIFFS.exists((char *) path.c_str())) {
		if (SPIFFS.exists((char *) (path + ".gz").c_str())) {
			path += ".gz";
		}
		File file = SPIFFS.open((char *) path.c_str(), "r");
		msSystem.msESPServer.streamFile(file, contentType);
		file.close();
		return true;
	} else {
		msSystem.slogln("streamFile fail:");
		msSystem.slogln(path.c_str());
	}

	return false;
}



void handleNotFound()
{

	char fName[MAX_FILENAME_LENGTH];
	l_safeStrncpy(fName, msSystem.msESPServer.uri().c_str(),
				  MAX_FILENAME_LENGTH);

	msSystem.slog("URI to retrieve: ");
	msSystem.slogln(msSystem.msESPServer.uri());
	msSystem.slog("fName to retrieve: ");
	msSystem.slogln(String(fName + 1));

	String gzFName = String(fName + 1) + ".gz";

	// if we don't have a route for the request, but do have a file, serve the file
	if (SPIFFS.exists(fName + 1) || SPIFFS.exists(gzFName.c_str())) {
		streamFile(String(fName + 1));
	} else if (msSystem.msESPServer.uri() != "/upload") {
		String message = "MS Command Not Found\n\n";
		message += "URI: ";
		message += msSystem.msESPServer.uri();
		message += "\nMethod: ";
		message +=
			(msSystem.msESPServer.method() == HTTP_GET) ? "GET" : "POST";
		message += "\nArguments: ";
		message += msSystem.msESPServer.args();
		message += "\n";

		for (uint8_t i = 0; i < msSystem.msESPServer.args(); i++) {
			message +=
				" " + msSystem.msESPServer.argName(i) + ": " +
				msSystem.msESPServer.arg(i) + "\n";
		}

		msSystem.msESPServer.send(404, "text/plain", message);
	} else {
		String message = "<html>\
      <head>\
        <meta http-equiv='refresh' content='5'/>\
        <title><a href=\"/\">MagicShifter3000</a> upload done</title>\
      </head>\
      <body>\
        <h1>Upload of " + String(msGlobals.ggUploadFileName) + " done</h1><a href=\"/list?dir=\">list</a><br><a href=\"\">index.html</a></body></html>";

		msSystem.msESPServer.sendHeader("Access-Control-Allow-Origin",
										"*");
		msSystem.msESPServer.send(200, "text/html", message);

	}
}


void handleReadFile()
{
	msSystem.msLEDs.fillLEDs(0, 0, 1, 0x1F);
	msSystem.msLEDs.updateLEDs();

	msSystem.slogln("Free heap: ");
	msSystem.slogln(String(ESP.getFreeHeap()));

	String message = "ReadFile:\n";


	if (msSystem.msESPServer.args() >= 1) {
		String args = msSystem.msESPServer.arg(0);

		strcpy(msGlobals.ggUploadFileName, args.c_str());
		msSystem.msEEPROMs.saveString(msGlobals.ggUploadFileName,
									  MAX_FILENAME_LENGTH);


		message += "file name: \"" + args /* String(filename) */  + "\"\n";

		//message += "file name strcpied: \"" +  msGlobals.ggUploadFileName + "\"\n";


		byte buffer[105];
		File file = SPIFFS.open(msGlobals.ggUploadFileName, "r");

		if (file) {
			int fileLen = file.available();
			msSystem.slogln("file available: ");
			msSystem.slogln(String(fileLen));

			if (fileLen > 0) {
				if (fileLen > 100)
					fileLen = 100;
				msSystem.slogln("reading: ");
				msSystem.slogln(String(file.read(buffer, fileLen)));

				msSystem.slogln("buffer: ");

				for (int i = 0; i < fileLen; i++) {
					msSystem.slogln(String((char) buffer[i]));
					message += (char) buffer[i];
				}
			}

			file.close();
		} else {
			msSystem.slogln("file not found");
			message += " file not found!";
		}
	} else {
		msSystem.slogln("arg missing");
		message += "argument missing!";
	}

	msSystem.msESPServer.send(200, "text/html", message);
}


void handleDeleteFile()
{
	msSystem.msLEDs.fillLEDs(0, 0, 1, 0x1F);
	msSystem.msLEDs.updateLEDs();

	String message = "handleDeleteFile\n";
	if (msSystem.msESPServer.args() >= 1) {
		String args = msSystem.msESPServer.arg(0);
		message += "Trying to delete file: \"" + args + "\"\n";

		if (SPIFFS.exists(args)) {
			SPIFFS.remove(args);
			msSystem.slogln("file deleted");
			message += "file deleted!";
		} else {
			msSystem.slogln("file not found");
			message += "file not found!";
		}
	} else {
		msSystem.slogln("arg missing");
		message += "argument missing!";
	}

	msSystem.msESPServer.send(200, "text/html", message);
}





void handleFileList()
{
	if (!msSystem.msESPServer.hasArg("dir")) {
		msSystem.msESPServer.send(500, "text/plain",
								  "BAD ARGS, missing dir=");
		return;
	}
	String path = msSystem.msESPServer.arg("dir");

// msSystem.slog("dir path::::"); msSystem.slogln((char *)path.c_str());

	//File entry;
	Dir dir = SPIFFS.openDir((char *) path.c_str());
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

	while (true) {
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
	msSystem.msESPServer.send(200, "text/html", output);
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


#if 0
void dumpSPIFFS_info()
{
	FSInfo linfo;
	SPIFFS.info(&linfo);
	msSystem.slog("linfo.maxOpenFiles =");
	msSystem.slogln(linfo.maxOpenFiles);
	msSystem.slog("linfo.blockSize =");
	msSystem.slogln(linfo.blockSize);
	msSystem.slog("linfo.pageSize =");
	msSystem.slogln(linfo.pageSize);
	msSystem.slog("linfo.maxOpenFiles =");
	msSystem.slogln(linfo.maxOpenFds);
	msSystem.slog("linfo.maxPathLength =");
	msSystem.slogln(linfo.maxPathLength);
	msSystem.slog("linfo.totalBytes =");
	msSystem.slogln(linfo.totalBytes);
	msSystem.slog("linfo.usedBytes = ");
	msSystem.slogln(linfo.usedBytes);
}
#endif


void handleFileUpload()
{

	msSystem.slogln("handle upload!");

	//if (msSystem.msESPServer.uri() != "/upload") return;
	HTTPUpload & upload = msSystem.msESPServer.upload();

	if (upload.status == UPLOAD_FILE_START) {
		l_safeStrncpy(msGlobals.ggUploadFileName, (char *) upload.filename.c_str(), MAX_FILENAME_LENGTH);	//.c_str();
		msSystem.slogln("upload started: ");
		msSystem.slogln(msGlobals.ggUploadFileName);

		msSystem.slog("upload open.. ");

		if (SPIFFS.exists(msGlobals.ggUploadFileName)) {
			msSystem.slog("Removing previous copy:");
			msSystem.
				slogln(String(SPIFFS.remove(msGlobals.ggUploadFileName)));
		}
		// !J! re-use file..
		if (msGlobals.ggUploadFile)
			msGlobals.ggUploadFile.close();


		msSystem.slog("ggUploadFile opened:");
		msSystem.slogln(msGlobals.ggUploadFileName);

		// // !J! hak try 3 times:
		int cnt = 3;
		while (--cnt >= 0) {
			msSystem.slog("try:");
			msSystem.slogln(String(cnt));

			msGlobals.ggUploadFile =
				SPIFFS.open(msGlobals.ggUploadFileName, "w");
			if (msGlobals.ggUploadFile)
				break;
		}

		if (!msGlobals.ggUploadFile) {
			msSystem.slogln("ERROR: COULD NOT open file!!!");
		} else
			msSystem.slogln("Opened file for writing...");

		//if (SD.exists((char *)upload.filename.c_str())) SD.remove((char *)upload.filename.c_str());
		//msGlobals.ggUploadFile = SD.open(upload.filename.c_str(), FILE_WRITE);
		//DBG_OUTPUT_PORT.print("Upload: START, filename: "); DBG_OUTPUT_PORT.println(upload.filename);

	} else if (upload.status == UPLOAD_FILE_WRITE) {

		bool result;
		if (msGlobals.ggUploadFile) {
			result =
				msGlobals.ggUploadFile.write(upload.buf,
											 upload.currentSize);
			if (!result) {
				msSystem.slogln("ERROR: could not write!");
			}
		}
		msSystem.slog("Upload: WRITE, Bytes: ");
		msSystem.slogln(String(upload.currentSize));
	} else if (upload.status == UPLOAD_FILE_END) {
		msGlobals.ggShouldAutoLoad = 1;

		msSystem.msEEPROMs.saveString(msGlobals.ggUploadFileName,
									  MAX_FILENAME_LENGTH);

		msGlobals.ggUploadFile.close();

		msSystem.slogln("Upload: END, Size: ");
		msSystem.slogln(String(upload.totalSize));
		msGlobals.ggFault = FAULT_NEW_FILEUPLOAD;
	}
}

#endif
//_WEBSERVER_ROUTES_H
