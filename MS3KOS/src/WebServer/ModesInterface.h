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