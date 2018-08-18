void handleGETInterfacesModes(void)
{
	msSystem.slogln("handleGETInterfacesModes");

	String response = "{";
	response += "\"Magic POV\": 0,";
	response += "\"Light\": 1,";
	response += "\"Compass\": 2,";
	response += "\"Config\": 3,";
	response += "\"Beat\": 4,";
	response += "}";
	msSystem.msESPServer.send(200, "text/plain", response);
}

// http://esp8266.github.io/Arduino/versions/2.0.0/doc/filesystem.html#directory-object-dir
void handleFileListJson()
{
    if (!msSystem.msESPServer.hasArg("dir")) {
        msSystem.msESPServer.send(500, "text/plain",
                                  "BAD ARGS, missing dir=");
        return;
    }
    String path = msSystem.msESPServer.arg("dir");

    Dir dir = SPIFFS.openDir((char *) path.c_str());
    path = String();

    String output = "{path:'" + path + "', files:[";

    bool first = 1;
    while (true) {
        if (!dir.next())
            break;

        if (!first) {
            output += ", ";
        }
        first = 0;

        String name = dir.fileName();
        File f = dir.openFile("r");
        Serial.println(f.size());
        int size = f.size();
        f.close();
        output += "{name:'"+ name + "',size:" + size + "}";
    }

    output += "]}";
    msSystem.msESPServer.send(200, "text/json", output);
}
