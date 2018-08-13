#ifndef _WEBSERVERAPI_H
#define _WEBSERVERAPI_H

#include <pb.h>
#include <pb_decode.h>
#include <pb_encode.h>

const char b64_alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "abcdefghijklmnopqrstuvwxyz" "0123456789+/";

/* 'Private' declarations */
inline void a3_to_a4(unsigned char *a4, unsigned char *a3);
inline void a4_to_a3(unsigned char *a3, unsigned char *a4);
inline unsigned char b64_lookup(char c);

int base64_encode(char *output, const char *input, int inputLen)
{
	int i = 0, j = 0;
	int encLen = 0;
	unsigned char a3[3];
	unsigned char a4[4];

	while (inputLen--) {
		a3[i++] = *(input++);
		if (i == 3) {
			a3_to_a4(a4, a3);

			for (i = 0; i < 4; i++) {
				output[encLen++] = b64_alphabet[a4[i]];
			}

			i = 0;
		}
	}

	if (i) {
		for (j = i; j < 3; j++) {
			a3[j] = '\0';
		}

		a3_to_a4(a4, a3);

		for (j = 0; j < i + 1; j++) {
			output[encLen++] = b64_alphabet[a4[j]];
		}

		while ((i++ < 3)) {
			output[encLen++] = '=';
		}
	}
	output[encLen] = '\0';
	return encLen;
}

int base64_decode(char *output, const char *input, int inputLen)
{
	int i = 0, j = 0;
	int decLen = 0;
	unsigned char a3[3];
	unsigned char a4[4];


	while (inputLen--) {
		if (*input == '=') {
			break;
		}

		a4[i++] = *(input++);
		if (i == 4) {
			for (i = 0; i < 4; i++) {
				a4[i] = b64_lookup(a4[i]);
			}

			a4_to_a3(a3, a4);

			for (i = 0; i < 3; i++) {
				output[decLen++] = a3[i];
			}
			i = 0;
		}
	}

	if (i) {
		for (j = i; j < 4; j++) {
			a4[j] = '\0';
		}

		for (j = 0; j < 4; j++) {
			a4[j] = b64_lookup(a4[j]);
		}

		a4_to_a3(a3, a4);

		for (j = 0; j < i - 1; j++) {
			output[decLen++] = a3[j];
		}
	}
	output[decLen] = '\0';
	return decLen;
}

int base64_enc_len(int plainLen)
{
	int n = plainLen;
	return (n + 2 - ((n + 2) % 3)) / 3 * 4;
}

int base64_dec_len(char *input, int inputLen)
{
	int i = 0;
	int numEq = 0;
	for (i = inputLen - 1; input[i] == '='; i--) {
		numEq++;
	}

	return ((6 * inputLen) / 8) - numEq;
}

inline void a3_to_a4(unsigned char *a4, unsigned char *a3)
{
	a4[0] = (a3[0] & 0xfc) >> 2;
	a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
	a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);
	a4[3] = (a3[2] & 0x3f);
}

inline void a4_to_a3(unsigned char *a3, unsigned char *a4)
{
	a3[0] = (a4[0] << 2) + ((a4[1] & 0x30) >> 4);
	a3[1] = ((a4[1] & 0xf) << 4) + ((a4[2] & 0x3c) >> 2);
	a3[2] = ((a4[2] & 0x3) << 6) + a4[3];
}

inline unsigned char b64_lookup(char c)
{
	if (c >= 'A' && c <= 'Z')
		return c - 'A';
	if (c >= 'a' && c <= 'z')
		return c - 71;
	if (c >= '0' && c <= '9')
		return c + 4;
	if (c == '+')
		return 62;
	if (c == '/')
		return 63;
	return -1;
}
void handleGETInfo(void)
{
	msSystem.slogln("handleGETInfo");

	String response = "{";
	response += "\"api\":{";
	response += "\"versions\":[1],";
	response += "\"prefix\":\"v\"";
	response += "},";
	response += "\"init\": {";
	response += "\"settings\":[";
	response += "\"server\",";
	response += "\"ap\",";
	response += "\"wifi/list\",";
	response += "\"wifi/preferred\"";
	response += "],";
	response += "\"info\":[";
	response += "\"about\",";
	response += "\"status\"";
	response += "]";
	response += "}";
	response += "}";
	msSystem.msESPServer.send(200, "text/plain", response);
}


void handleGETAbout(void)
{
	msSystem.slogln("handleGETAbout");

	String response = "{";
	response += "\"type\":\"MagicShifter3000\",";
	response += "\"format\":\"BGRA\",";
	response += "\"version\":" + String(MS3KOS_VERSION) + ",";
	response += "\"leds\":" + String(MAX_LEDS) + ",";
	response += "\"id\":" + String(ESP.getChipId()) + ",";
	response += "\"flashid\":" + String(ESP.getFlashChipId()) + ",";
	response += "\"flashsize\":" + String(ESP.getFlashChipSize());
	response += "}";
	msSystem.msESPServer.send(200, "text/plain", response);
}

void handleGETStatus(void)
{
	msSystem.slogln("handleGETStatus");

	int adValue = msGlobals.ggLastADValue;
	float voltage = msSystem.getBatteryVoltage();

	uint32_t ip = (uint32_t) msSystem.getIP();

	String response = "{";
	response += "\"id\":" + String(ESP.getChipId()) + ",";
	response +=
		"\"uptime\":" + String(millis() - msGlobals.ggBootTime) + ",";
	response += "\"adValue\":" + String(adValue) + ",";
	response += "\"voltage\":" + String(voltage) + ",";
	response += "\"ggAccelTime\":" + String(msGlobals.ggAccelTime) + ",";
	response += "\"ggLFrameTime\":" + String(msGlobals.ggLFrameTime) + ",";
	response +=
		"\"ggLastFrameMicros\":" + String(msGlobals.ggLastFrameMicros) +
		",";
	response +=
		"\"ggCurrentMicros\":" + String(msGlobals.ggCurrentMicros) + ",";
	response += "\"accelRaw\":[";
	response += String(msGlobals.ggAccelCount[XAXIS]) + ",";
	response += String(msGlobals.ggAccelCount[YAXIS]) + ",";
	response += String(msGlobals.ggAccelCount[ZAXIS]);
	response += "],";
	response += "\"msGlobals.ggAccel\":[";
	response += String(msGlobals.ggAccel[XAXIS]) + ",";
	response += String(msGlobals.ggAccel[YAXIS]) + ",";
	response += String(msGlobals.ggAccel[ZAXIS]);
	response += "],";
	response += "\"ip\":\"";
	response += String(0xFF & (ip >> 0)) + ".";
	response += String(0xFF & (ip >> 8)) + ".";
	response += String(0xFF & (ip >> 16)) + ".";
	response += String(0xFF & (ip >> 24)) + "\"";
	response += "}";
	msSystem.msESPServer.send(200, "text/plain", response);
}


/**
ms3000.info
ms3000.about
ms3000.status
ms3000.config.ap.info //our ssid and pwd
ms3000.config.ap.current
ms3000.config.ap.list
ms3000.config.ap.preferred
ms3000.config.ap.server.name
ms3000.config.syslog // host, port ?
ms3000.config.mode
ms3000.mode.rainbow.speed
ms3000.mode.brightlight.color
ms3000.mode.sensi.sensitivity
ms3000.mode.magicpov.image.name
ms3000.mode.magicpov.image.rawRGB
ms3000.mode.magicpov.image.brightness
ms3000.mode.arpi.mode
ms3000.mode.arpi.midi.channel.out
ms3000.mode.arpi.midi.channel.in
ms3000.mode.arpi.midi.channel.keys
ms3000.mode.arpi.midi.channel.out
ms3000.mode.lightning.submode
ms3000.mode.lightning.mode.description
ms3000.mode.lightning.color


e.g.

int arpi_mode = _storage.GetInt("ms3000.mode.arpi.submode")
int lightning_submode = _storage.GetInt("ms3000.mode.lightning.submode")
RGBType lightning_color_ = _storage.GetColor("ms3000.mode.lightning.color")

_storage.PutInt("ms3000.mode.arpi.submode", 2);
_storage.PutInt("ms3000.mode.lightning.submode", 5);

**/

bool parseAPInfoFromServerArgs(APAuth & apInfo)
{
	bool success = true;
	for (int i = 0; i < msSystem.msESPServer.args(); i++) {
		msSystem.slogln("argName: ");
		msSystem.slogln(msSystem.msESPServer.argName(i));

		msSystem.slogln("arg: ");
		msSystem.slogln(msSystem.msESPServer.arg(i));

		if (strcmp(msSystem.msESPServer.argName(i).c_str(), "ssid") == 0) {
			l_safeStrncpy(apInfo.ssid, msSystem.msESPServer.arg(i).c_str(),
						  sizeof(apInfo.ssid));
		} else if (strcmp(msSystem.msESPServer.argName(i).c_str(), "pwd")
				   == 0) {
			l_safeStrncpy(apInfo.password,
						  msSystem.msESPServer.arg(i).c_str(),
						  sizeof(apInfo.password));
		} else {
			msSystem.slogln("arg is not known!");
			success = false;
		}
	}
	return success;
}

bool parseSysLogInfoFromServerArgs(ServerConfig & sysLogHost)
{
	bool success = true;
	for (int i = 0; i < msSystem.msESPServer.args(); i++) {
		msSystem.slogln("argName: ");
		msSystem.slogln(msSystem.msESPServer.argName(i));

		msSystem.slogln("arg: ");
		msSystem.slogln(msSystem.msESPServer.arg(i));

		if (strcmp(msSystem.msESPServer.argName(i).c_str(), "host") == 0) {
			l_safeStrncpy(sysLogHost.hostname,
						  msSystem.msESPServer.arg(i).c_str(),
						  sizeof(sysLogHost.hostname));
		} else if (strcmp(msSystem.msESPServer.argName(i).c_str(), "port")
				   == 0) {
			sysLogHost.port = atoi(msSystem.msESPServer.arg(i).c_str());
		} else {
			msSystem.slogln("arg is not known!");
			success = false;
		}
	}
	return success;
}


void handleGetSettings(void)
{
	msSystem.slogln("handleGetSettings");

	ServerConfig config;
	msSystem.Settings.getServerConfig(&config);

	String response = "<!DOCTYPE html><html><head></head><body>";;
	response += "<h3>Urls</h3>";
	response += "<ul>";
	response += "<li><a href=\"/settings\">this page (/settings)</a></li>";
	response +=
		"<li><a href=\"/settings/server\">server settings (/settings/server)</a></li>";
	response +=
		"<li><a href=\"/settings/ap\">accesspoint settings (/settings/ap)</a></li>";

	response += "<li><a href=\"/info\">the schema page (/info)</a></li>";

	response +=
		"<li><a href=\"/info/about\">about info page (/info/about)</a></li>";
	response +=
		"<li><a href=\"/info/status\">status output page (/info/status)</a></li>";

	response +=
		"<li><a href=\"/settings/wifi/list\">list of saved wifis (/settings/wifi/list)</a></li>";
	response +=
		"<li><a href=\"/settings/wifi/preferred\">preferred wifi (/settings/wifi/preferred)</a></li>";

	response += "</ul>";
	response += "</body></html>";
	msSystem.msESPServer.send(200, "text/html", response);
}

void handleGETServerSettings(void)
{
	msSystem.slogln("handleGETServerSettings");

	ServerConfig config;
	msSystem.Settings.getServerConfig(&config);

	String response = "{";
	response += "\"host\":";
	response += "\"";
	response += config.hostname;
	response += "\"";
	response += ",";
	response += "\"port\":";
	response += config.port;
	response += "}";
	msSystem.msESPServer.send(200, "text/plain", response);
}

void handlePOSTServerSettings(void)
{
	msSystem.slogln("handlePOSTServerSettings");
	if (msSystem.msESPServer.args() >= 2) {
		ServerConfig config;
		msSystem.Settings.getServerConfig(&config);

		bool success = true;
		for (int i = 0; i < msSystem.msESPServer.args(); i++) {
			msSystem.slogln("argName: ");
			msSystem.slogln(msSystem.msESPServer.argName(i));

			msSystem.slogln("arg: ");
			msSystem.slogln(msSystem.msESPServer.arg(i));

			if (strcmp(msSystem.msESPServer.argName(i).c_str(), "host") ==
				0) {
				l_safeStrncpy(config.hostname,
							  msSystem.msESPServer.arg(i).c_str(),
							  sizeof(config.hostname));
			} else
				if (strcmp(msSystem.msESPServer.argName(i).c_str(), "port")
					== 0) {
				config.port = atoi(msSystem.msESPServer.arg(i).c_str());
			} else {
				success = false;
			}
		}

		if (success) {
			msSystem.Settings.setServerConfig(&config);
			msSystem.msESPServer.send(200, "text/plain", "OK");
		} else {
			msSystem.msESPServer.send(500, "text/plain", "invalid args!");
		}
	} else {
		msSystem.msESPServer.send(500, "text/plain", "argument missing!");
	}
}

void handleGETAPSettings(void)
{
	msSystem.slogln("handleGETAPSettings");

	APAuth apInfo;
	msSystem.Settings.getAPConfig(&apInfo);

	String response = "{";
	response += "\"ssid\":";
	response += "\"";
	response += apInfo.ssid;
	response += "\"";
	//response += ",";
	//response += "\"pwd\":";
	//response += "\"" + apInfo.pwd + "\"";
	response += "}";
	msSystem.msESPServer.send(200, "text/plain", response);
}


#define PROTOBUF_LEN 1024


bool ms3kModeLightNameFunc(pb_istream_t *stream, const pb_field_t *field, void **arg)
{

 uint8_t buf[PROTOBUF_LEN] = {0};
    size_t len = stream->bytes_left;

	printf("ms3kModeLightNameFunc: len: %d", len );

    if (len > sizeof(buf) - 1 || !pb_read(stream, buf, len))
        return false;
    
    printf("ms3kModeLightNameFunc: STRING: %s\n", (char*)buf);
    return true;
}



void handleGETProtoBufferBase64(void)
{
	// MS3KG lMS3KG = MS3KG_init_zero;

	int encoderStatus;
	uint8_t pbufOutput[256];
	char base64Output[1024];
	int base64length;

	msSystem.slogln("handleGETProtoBufferBase64");

	pb_ostream_t stream = pb_ostream_from_buffer(pbufOutput, sizeof(pbufOutput));
	encoderStatus = pb_encode(&stream, MS3KG_fields, &msGlobals.protocolBuffer);

	if (!encoderStatus)
	{
		printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
	}

	msSystem.hexDump(stream.bytes_written, pbufOutput, "bufOutput::");

	base64length = base64_encode(base64Output, (char *)pbufOutput, stream.bytes_written);

printf("base64length: %d\n", base64length);
printf("encoderStatus: %d\n", encoderStatus);
printf("stream.bytes_written: %d\n", stream.bytes_written);

	String response = "";

	response += (char *)base64Output;

	msSystem.msESPServer.sendHeader("Access-Control-Allow-Origin", "*");
	msSystem.msESPServer.send(200, "text/plain", response);

}

void handlePOSTProtocolBufferBase64(void)
{
	msSystem.slogln("handlePOSTProtocolBufferBase64");

	if (msSystem.msESPServer.args() == 0) {
		msSystem.msESPServer.send(500, "text/plain", "argument missing!");
	}
	else
	if (msSystem.msESPServer.args() >= 1) {
		// const char *encodedInputStr = msSystem.msESPServer.arg(0).c_str();

		bool decodeStatus;
		unsigned int decodedDataLen = 0;


		char decoderInput[PROTOBUF_LEN];
		uint8_t decoderOutput[PROTOBUF_LEN];

//void l_safeStrncpy(char *dest, const char *source, int n)

		l_safeStrncpy(decoderInput,  msSystem.msESPServer.arg(0).c_str(), PROTOBUF_LEN);


		printf("handlePPBB64: arg name is %s!\n",  msSystem.msESPServer.argName(0).c_str());
		// printf("handlePPBB64: encodedInputStr is %s!\n", encodedInputStr);
		printf("handlePPBB64: encodedInputStr (2) is %s!\n", msSystem.msESPServer.arg(0).c_str());
		printf("handlePPBB64: decoderInput is %s!\n", decoderInput);



		unsigned int encodedInputStrLen = (int) msSystem.msESPServer.arg(0).length();
		// encodedInputStrLen = strlen(decoderInput);
		if (encodedInputStrLen >  sizeof(decoderOutput))
			encodedInputStrLen = sizeof(decoderOutput);

printf("handlePPBB64: xx\n");
		decodedDataLen = base64_decode((char *)decoderOutput, decoderInput, encodedInputStrLen);

        /* Create a stream that reads from the decoderOutput. */
		pb_istream_t stream = pb_istream_from_buffer(decoderOutput, decodedDataLen);
printf("handlePPBB64: xxx\n");



        /* Now we are ready to decode the message. */
		decodeStatus = pb_decode(&stream, MS3KG_fields, &msGlobals.protocolBuffer); // TODO: !J! msGlobals.protocolBuffer??
printf("handlePPBB64: xxxx\n");

		// printf("handlePPBB64: encodedInputStrLen is %d!\n", encodedInputStrLen);
		printf("handlePPBB64: decodedDataLen is %d!\n", decodedDataLen);
		printf("handlePPBB64: decodeStatus is %d!\n", decodeStatus);

        /* Check for errors... */
		if (!decodeStatus)
		{
			printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
		}

        /* Print the data contained in the message. */
		// msGlobals.protocolBuffer.modes.light.name.funcs.decode = ms3kModeLightNameFunc;
printf("handlePPBB64: xxxxx\n");

		printf("handlePPBB64: submode is %d!\n", (int)msGlobals.protocolBuffer.modes.light.subMode);
		// printf("handlePPBB64: submode name is %d!\n", strlen(msGlobals.protocolBuffer.modes.light.name));
		printf("handlePPBB64: networkName is %s!\n", msGlobals.protocolBuffer.networkName);
		printf("handlePPBB64: beatMode is %d!\n", msGlobals.protocolBuffer.modes.beat.beatMode);
		printf("handlePPBB64: sensitivity is %d!\n", msGlobals.protocolBuffer.modes.beat.sensitivity);
		printf("handlePPBB64: R is %d!\n", msGlobals.protocolBuffer.modes.light.color.R);
		printf("handlePPBB64: G is %d!\n", msGlobals.protocolBuffer.modes.light.color.G);
		printf("handlePPBB64: B is %d!\n", msGlobals.protocolBuffer.modes.light.color.B);
		printf("handlePPBB64: networkName is %s!\n", msGlobals.protocolBuffer.networkName);

		msSystem.msESPServer.send(200, "text/plain", "OK");

	} else {
		msSystem.msESPServer.send(500, "text/plain", "unknown args!");
	} 
}



void handlePOSTAPSettings(void)
{
	msSystem.slogln("handlePOSTAPSettings");

	if (msSystem.msESPServer.args() >= 2) {
		// load old settings
		APAuth apInfo;
		msSystem.Settings.getAPConfig(&apInfo);

		if (parseAPInfoFromServerArgs(apInfo)) {
			msSystem.slogln("saving setAPConfig");
			msSystem.Settings.setAPConfig(&apInfo);
			msSystem.msESPServer.send(200, "text/plain", "OK");
		} else {
			msSystem.msESPServer.send(500, "text/plain", "unknown args!");
		}
	} else {
		msSystem.msESPServer.send(500, "text/plain", "argument missing!");
	}
}



void handleGETSysLogHostSettings(void)
{
	msSystem.slogln("handleGETSysLogHostSettings");

	ServerConfig sysLogInfo;
	msSystem.Settings.getSyslogConfig(&sysLogInfo);

	String response = "{";
	response += "\"syslog_host\":";
	response += "\"";
	response += sysLogInfo.hostname;
	response += "\"";
	response += "}";
	msSystem.msESPServer.send(200, "text/plain", response);
}

void handlePOSTSysLogSettings(void)
{
	msSystem.slogln("handlePOSTSysLogSettings");

	if (msSystem.msESPServer.args() >= 2) {
		// load old settings
		ServerConfig sysLogInfo;
		msSystem.Settings.getSyslogConfig(&sysLogInfo);

		if (parseSysLogInfoFromServerArgs(sysLogInfo)) {
			msSystem.slogln("saving setSysLogConfig");
			msSystem.Settings.setSyslogConfig(&sysLogInfo);
			msSystem.msESPServer.send(200, "text/plain", "OK");
		} else {
			msSystem.msESPServer.send(500, "text/plain", "unknown args!");
		}
	} else {
		msSystem.msESPServer.send(500, "text/plain", "argument missing!");
	}
}



void handleGETPreferredAPSettings(void)
{
	msSystem.slogln("handleGETPreferredAPSettings");

	APAuth apInfo;
	msSystem.Settings.getPreferredAP(&apInfo);

	String response = "{";
	response += "\"ssid\":";
	response += "\"";
	response += apInfo.ssid;
	response += "\"";
	//response += ",";
	//response += "\"pwd\":";
	//response += "\"" + apInfo.pwd + "\"";
	response += "}";
	msSystem.msESPServer.send(200, "text/plain", response);
}

void handlePOSTPreferredAPSettings(void)
{
	msSystem.slogln("handlePOSTPreferredAPSettings");

	if (msSystem.msESPServer.args() >= 2) {
		APAuth apInfo;
		msSystem.Settings.getPreferredAP(&apInfo);

		if (parseAPInfoFromServerArgs(apInfo)) {
			msSystem.slogln("saving setPreferredAP");
			msSystem.Settings.setPreferredAP(&apInfo);
			msSystem.msESPServer.send(200, "text/plain", "OK");
		} else {
			msSystem.msESPServer.send(500, "text/plain", "unknown args!");
		}
	} else {
		msSystem.msESPServer.send(500, "text/plain", "argument missing!");
	}
}


///////////////////////////////////////
//////////////////////////////////////////

void handleGETAPList(void)
{
	msSystem.slogln("handleGETAPList");

	APAuth apInfo;
	msSystem.Settings.getPreferredAP(&apInfo);

	String response = "[";

	msSystem.Settings.resetAPList();
	bool firstAP = true;
	while (msSystem.Settings.getNextAP(&apInfo)) {
		if (!firstAP) {
			response += ",";
		}
		firstAP = false;

		response += "{\"ssid\":";
		response += "\"";
		response += apInfo.ssid;
		response += "\"";
		//response += ",";
		//response += "\"pwd\":";
		//response += "\"" + apInfo.pwd + "\"";
		response += "}";
	}
	response += "]";
	msSystem.Settings.resetAPList();

	msSystem.msESPServer.send(200, "text/plain", response);
}

void handleGETWLANList(void)
{
	msSystem.slogln("handleGETWLANList");

	String response = "[";

	int n = WiFi.scanNetworks();
	msSystem.slogln("scan done");
	if (n == 0) {
		msSystem.slogln("\"no networks found\"");
	} else {
		//msSystem.slogln(n);
		msSystem.slogln(" networks found");
		bool firstAP = true;
		for (int i = 0; i < n; ++i) {
			if (!firstAP) {
				response += ",";
			}
			firstAP = false;

			response += "{\"ssid\":";
			response += "\"";
			response += WiFi.SSID(i);
			response += "\"";
			response += ",";
			response += "\"rssi\":";
			response += WiFi.RSSI(i);
			response += ",";
			response += "\"free\":";
			response +=
				(WiFi.encryptionType(i) ==
				 ENC_TYPE_NONE) ? "true" : "false";
			response += "}";
		}
	}


	response += "]";
	msSystem.Settings.resetAPList();

	msSystem.msESPServer.send(200, "text/plain", response);
}

void handlePOSTAPListAdd(void)
{
	msSystem.slogln("handlePOSTAPListAdd");

	if (msSystem.msESPServer.args() >= 2) {
		APAuth apInfo;
		memset(apInfo.ssid, 0, sizeof(apInfo.ssid));
		memset(apInfo.password, 0, sizeof(apInfo.password));

		if (parseAPInfoFromServerArgs(apInfo)) {
			if (!strcmp(apInfo.ssid, "") == 0) {
				msSystem.slogln("adding wifi");
				msSystem.Settings.addAP(&apInfo);
			}
			msSystem.msESPServer.send(200, "text/plain", "OK");
		} else {
			msSystem.msESPServer.send(500, "text/plain", "invalid args!");
		}
	} else {
		msSystem.msESPServer.send(500, "text/plain", "args missing!");
	}
}

void handlePOSTAPListDelete(void)
{
	msSystem.slogln("handlePOSTAPListDelete");

	if (msSystem.msESPServer.args() >= 1) {
		APAuth apInfo;
		strcpy(apInfo.ssid, "");
		strcpy(apInfo.password, "");

		if (parseAPInfoFromServerArgs(apInfo)) {
			if (!strcmp(apInfo.ssid, "") == 0) {
				msSystem.slogln("deleting wifi");
				msSystem.Settings.deleteAP(apInfo.ssid);
			}
			msSystem.msESPServer.send(200, "text/plain", "OK");
		} else {
			msSystem.msESPServer.send(500, "text/plain", "invalid args!");
		}
	} else {
		msSystem.msESPServer.send(500, "text/plain", "args missing!");
	}
}


void handleSetMode(void)
{
	msSystem.slogln("handleSetMode");
	if (msSystem.msESPServer.args() == 1) {
		ServerConfig config;
		msSystem.Settings.getServerConfig(&config);

		bool success = true;
		for (int i = 0; i < msSystem.msESPServer.args(); i++) {
			msSystem.slogln("argName: ");
			msSystem.slogln(msSystem.msESPServer.argName(i));

			msSystem.slogln("arg: ");
			msSystem.slogln(msSystem.msESPServer.arg(i));

			int newMode = 
				atoi(msSystem.msESPServer.arg(i).c_str());
			
			msSystem.setMode(newMode);

		}

		if (success) {
			String response = "{";
			response += "\"mode\":";
			response += "\"";
			response += msGlobals.ggCurrentMode;
			response += "\"";
			response += "}";
			msSystem.msESPServer.send(200, "text/plain", response);
		} else {
			msSystem.msESPServer.send(500, "text/plain", "invalid args!");
		}
	} else {
		msSystem.msESPServer.send(500, "text/plain", "argument missing!");
	}
}

void respondREQTime()
{
	int currentTime =
		msGlobals.ggTime + (millis() - msGlobals.ggTimePostedAt);

	String response = "{";
	response += "\"time\":" + String(currentTime) + ",";
	response += "\"postedAt\":" + String(msGlobals.ggTimePostedAt);
	response += "}";

	msSystem.msESPServer.send(200, "text/plain", response);
}

void handleGETTime()
{
	msSystem.slogln("handleGETTime");

	respondREQTime();
}

void handlePOSTTime()
{
	msSystem.slogln("handlePOSTTime");

	if (msSystem.msESPServer.argName(0) == "t") {
		msGlobals.ggTime = atoi(msSystem.msESPServer.arg(0).c_str());
		msGlobals.ggTimePostedAt = millis();
	}

	respondREQTime();
}

void handleLedSet()
{
	byte ledData[MAX_LEDS * 5];

	String message = "LedSet\n\n";

	if (msSystem.msESPServer.args() == 1) {
		const char *input = msSystem.msESPServer.arg(0).c_str();
		unsigned int inputLen = (int) msSystem.msESPServer.arg(0).length();
		msSystem.slogln("inputLen: ");
		msSystem.slogln(String(inputLen));

		if (inputLen > sizeof(ledData))
			inputLen = sizeof(ledData);

		msSystem.slogln(String(inputLen));

		unsigned int dataLen = 0;
		dataLen = base64_decode((char *) ledData, input, inputLen);

		for (int i = 0; i < dataLen; i += 5) {
			byte idx = ledData[i];
			msSystem.slogln("idx: ");
			msSystem.slogln(String((int) idx));
			if (idx >= MAX_LEDS)
				continue;
			msSystem.slogln("data+1: ");
			msSystem.slogln(String((int) ledData[i + 1]));

			msGlobals.ggRGBLEDBuf[idx * 4] = ledData[i + 1];
			msGlobals.ggRGBLEDBuf[idx * 4 + 1] = ledData[i + 2];
			msGlobals.ggRGBLEDBuf[idx * 4 + 2] = ledData[i + 3];
			msGlobals.ggRGBLEDBuf[idx * 4 + 3] = ledData[i + 4];
		}
	} else {
		message += "argument missing or too many arguments!";
	}
	msSystem.msESPServer.send(200, "text/plain", message);
}


void handleLedsSet()
{
	msSystem.slogln("handleLedsSet");

	String message = "LedsSet\n\n";

	if (msSystem.msESPServer.args() >= 1) {
		const char *input = msSystem.msESPServer.arg(0).c_str();
		int inputLen = BASE64_ENC_LEN(RGB_BUFFER_SIZE);

		base64_decode((char *) msGlobals.ggRGBLEDBuf, input, inputLen);

		message += "done";
	} else {
		message += "argument missing!";
	}
	msSystem.msESPServer.send(200, "text/plain", message);
}


/*
struct UIConfig
{
  int timeoutHighPower;
  int timeoutLowPower;
  int defaultBrightness;
};
*/

void handleGetUISettings(void)
{
  msSystem.slogln("handleGetUISettings");

  UIConfig config;
  msSystem.Settings.getUIConfig(&config);

  String response = "{";
    response += "\"timeoutHighPower\":";
    response += config.timeoutHighPower;
    response += ",";
    response += "\"timeoutLowPower\":";
    response += config.timeoutLowPower;
    response += ",";
    response += "\"defaultBrightness\":";
    response += config.defaultBrightness;
  response += "}";
  msSystem.msESPServer.send(200, "text/plain", response);
}

void handleSetUISettings(void)
{
  msSystem.slogln("handleSetUISettings");
  if (msSystem.msESPServer.args() >= 1)
  {
    UIConfig config;
    msSystem.Settings.getUIConfig(&config);

    bool success = true;
    for (int i = 0; i < msSystem.msESPServer.args(); i++)
    {
      msSystem.slogln("argName: ");
      msSystem.slogln(msSystem.msESPServer.argName(i));

      msSystem.slogln("arg: ");
      msSystem.slogln(msSystem.msESPServer.arg(i));

      if (strcmp(msSystem.msESPServer.argName(i).c_str(), "timeoutHighPower") == 0)
      {
        config.timeoutHighPower = atoi(msSystem.msESPServer.arg(i).c_str());
      }
      else if (strcmp(msSystem.msESPServer.argName(i).c_str(), "timeoutLowPower") == 0)
      {
        config.timeoutLowPower = atoi(msSystem.msESPServer.arg(i).c_str());
      }
      else if (strcmp(msSystem.msESPServer.argName(i).c_str(), "defaultBrightness") == 0)
      {
        config.defaultBrightness = atoi(msSystem.msESPServer.arg(i).c_str());
      }
      else
      {
        success = false;
      }
    }

    if (success)
    {
      msSystem.Settings.setUIConfig(&config);
      msSystem.msESPServer.send (200, "text/plain", "OK");
    }
    else
    {
      msSystem.msESPServer.send(500, "text/plain", "invalid args!");
    }
  }
  else
  {
    msSystem.msESPServer.send ( 500, "text/plain", "argument(s) missing!");
  }
}

#endif
