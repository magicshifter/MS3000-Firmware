//
// Web services 
//

#include "WebServerAPI.h"
#include "WebServerAutoConnect.h"
#include "WebServerFiles.h"

class MagicShifterWebServer {

  private:
	static void HandleServeStaticFile(String path) {
		if (!streamFile(path)) {
			msSystem.msESPServer.send(404, "text/plain", "FileNotFound");
		}
  } public:

  	void stop() {
  		
  	}

	void start(void) {
		while (!AutoConnect()) {
			msSystem.slogln("webserver: wifi connection failed, retrying...");
			delay(100);
		}
#ifdef USE_MDNS
		if (msSystem.msDNS.begin("magicshifter", WiFi.localIP())) {
			msSystem.slogln("MDNS responder started");
		}
#endif
		msSystem.msESPServer.on("/restart",[]() {
								msSystem.msESPServer.send(200,
														  "text/plain",
														  "going down for restarts now!");
								delay(1000); msSystem.restart();}
		);
		msSystem.msESPServer.on("/kill",[]() {
								msSystem.msESPServer.send(200,
														  "text/plain",
														  "powering down now!");
								delay(1000); msSystem.powerDown();}
		);

		msSystem.msESPServer.on("/info", HTTP_GET, handleGETInfo);

		msSystem.msESPServer.on("/info/about", HTTP_GET, handleGETAbout);
		msSystem.msESPServer.on("/info/status", HTTP_GET, handleGETStatus);

		msSystem.msESPServer.on("/settings", HTTP_GET, handleGetSettings);

		msSystem.msESPServer.on("/settings/ap", HTTP_GET,
								handleGETAPSettings);
		msSystem.msESPServer.on("/settings/ap", HTTP_POST,
								handlePOSTAPSettings);

		msSystem.msESPServer.on("/settings/syslog", HTTP_GET,
								handleGETSysLogHostSettings);
		msSystem.msESPServer.on("/settings/syslog", HTTP_POST,
								handlePOSTSysLogSettings);

		msSystem.msESPServer.on("/settings/server", HTTP_GET,
								handleGETServerSettings);
		msSystem.msESPServer.on("/settings/server/set",
								handlePOSTServerSettings);

		msSystem.msESPServer.on("/settings/ui", handleGetUISettings);
		msSystem.msESPServer.on("/settings/ui/set", handleSetUISettings);

		msSystem.msESPServer.on("/settings/wifi/preferred", HTTP_GET,
								handleGETPreferredAPSettings);
		msSystem.msESPServer.on("/settings/wifi/preferred/set",
								handlePOSTPreferredAPSettings);

		msSystem.msESPServer.on("/settings/wifi/list", HTTP_GET,
								handleGETAPList);
		msSystem.msESPServer.on("/settings/wifi/add", handlePOSTAPListAdd);
		msSystem.msESPServer.on("/settings/wifi/delete",
								handlePOSTAPListDelete);

		msSystem.msESPServer.on("/list", HTTP_GET, handleFileList);
		msSystem.msESPServer.on("/leds", handleLedsSet);
		msSystem.msESPServer.on("/read", handleReadFile);
		msSystem.msESPServer.on("/delete", handleDeleteFile);

		msSystem.msESPServer.on("/led", handleLedSet);

		msSystem.msESPServer.on("/mode", handleSetMode);

		msSystem.msESPServer.on("/time", HTTP_GET, handleGETTime);
		msSystem.msESPServer.on("/time", HTTP_POST, handlePOSTTime);

		msSystem.msESPServer.on("/listwlans", HTTP_GET, handleGETWLANList);

		msSystem.msESPServer.on("/download",[]() {
								if (!msSystem.msESPServer.hasArg("file")) {
								msSystem.msESPServer.send(500,
														  "text/plain",
														  "BAD ARGS, missing file=");
								return;}
								String path =
								msSystem.msESPServer.arg("file");
								HandleServeStaticFile(path);}
		);

		msSystem.msESPServer.on("/",[]() {
								HandleServeStaticFile("index.html");}
		);
		// msSystem.msESPServer.on("favicon.gif", HandleServeStaticFile("favicon.gif"));
		//msSystem.msESPServer.on("manifest.appcache", HandleServeStaticFile("manifest.appcache"));

//  msSystem.msESPServer.onFileUpload(handleFileUpload);
		msSystem.msESPServer.on("/upload", HTTP_POST,[]() {
								msSystem.msESPServer.send(200,
														  "text/plain",
														  "");
								}
								, handleFileUpload);


		msSystem.msESPServer.on("/format",[]() {
								//String message = "formatin DISABLED cos of FS change, TODO: implement!!!11";
								SPIFFS.format();
								msSystem.msESPServer.send(200,
														  "text/plain",
														  "formated");}
		);

		msSystem.msESPServer.onNotFound(handleNotFound);
		msSystem.msESPServer.begin();
		msSystem.slogln("HTTP msSystem.msESPServer started");
	}


	void step(void) {
		msSystem.msESPServer.handleClient();
	}

};
