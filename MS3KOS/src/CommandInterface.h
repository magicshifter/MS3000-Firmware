
extern MagicShifterSystem msSystem;

#define CMD_TEXT_MAX_LEN 32

// blocking USB functions
uint8_t SerialReadByte()
{
	while (!Serial.available());
	return Serial.read();
}

uint16_t SerialReadWord()
{
	uint16_t result = ((uint16_t) SerialReadByte()) << 8;
	result |= SerialReadByte();

	return result;
}

uint32_t SerialReadLong()
{
	uint32_t result = ((uint32_t) SerialReadByte()) << 24;
	result |= ((uint32_t) SerialReadByte()) << 16;
	result |= ((uint32_t) SerialReadByte()) << 8;
	result |= ((uint32_t) SerialReadByte()) << 0;

	return result;
}

void CommandInterfacePoll()
{
	static uint8_t pingStatus;

	if (Serial.available()) {
		uint8_t cmdBufferIdx = 0;
		uint8_t usbBuffer[CMD_TEXT_MAX_LEN];

		msSystem.msLEDs.fillLEDs(20, 0, 0, 0x0f);
		msSystem.msLEDs.updateLEDs();
		delay(1);

		while (Serial.available() && cmdBufferIdx < CMD_TEXT_MAX_LEN - 1) {
			char b = Serial.read();
			if (b == '\n' || b == '\r')
				break;
			usbBuffer[cmdBufferIdx++] = b;
		}
		usbBuffer[cmdBufferIdx++] = '\0';

		bool executed = false;

		if (!strcmp((const char *) usbBuffer, "MAGIC_FORMAT")) {
			SPIFFS.format();
			Serial.println("Formated FS!!!");
		}

		if (!strcmp((const char *) usbBuffer, "MAGIC_DUMP")) {
			Serial.println("TODO: implement DUMP for calibration values");
		}
		if (!strcmp((const char *) usbBuffer, "MAGIC_RESET")) {
			Serial.println("TODO: implement MAGIC_RESET");
		} else if (!strcmp((const char *) usbBuffer, "MAGIC_PING")) {
			pingStatus = !pingStatus;
			/*
			   Serial.print(" ea: ");
			   Serial.print(font16px.header.eepromAddr, HEX);
			   Serial.print(" bit: ");
			   Serial.print(font16px.header.pixelFormat);
			   Serial.print(" max: ");
			   Serial.print(font16px.header.maxFrame);
			   Serial.print(" w: ");
			   Serial.print(font16px.header.frameWidth);
			   Serial.print(" h: ");
			   Serial.print(font16px.header.frameHeight);

			   Serial.print(" st: ");
			   Serial.print(font16px.header.subType, HEX);
			   Serial.print(" f: ");
			   Serial.print(font16px.header.firstChar);
			   Serial.print(" d: ");
			   Serial.print(font16px.header.animationDelay);
			 */

			Serial.print("Ping Status now: ");
			Serial.println(pingStatus);
		} else if (!strcmp((const char *) usbBuffer, "MAGIC_UPLOAD")) {
			byte sector;
			uint32_t dataSize;

			// sector
			SerialReadLong();	// ignore MAGIC header
			// frames (bytes)
			dataSize = SerialReadLong();

			char fnA[256 - 8];
			for (int i = 0; i < sizeof(fnA); i++) {
				fnA[i] = SerialReadByte();
			}
			//String filenameStr = String(fnA);

			const char *filename = fnA;	//filenameStr.c_str();

			//l_safeStrncpy(msGlobals.ggUploadFileName, (char *)upload.filename.c_str(), MAX_FILENAME_LENGTH);//.c_str();
			msSystem.slog("upload started: ");
			msSystem.slog(filename);

			msSystem.slog("upload open.. ");

			if (SPIFFS.exists(filename)) {
				msSystem.msLEDs.fillLEDs(10, 10, 0, 0x0f);
				msSystem.msLEDs.updateLEDs();
				msSystem.slogln("Removing previous copy..");
				SPIFFS.remove(filename);
				
			}

			File spiffsFile = SPIFFS.open(filename, "w");
			msSystem.slog("ggUploadFile opened:");
			msSystem.slogln(filename);

			msSystem.msLEDs.fillLEDs(20, 0, 0, 0x0f);
			msSystem.msLEDs.updateLEDs();

			if (!spiffsFile) {
				msSystem.slogln("ERROR: COULD NOT open file!!!");
			} else {
				int ledIdx = -1;

				msSystem.slog("Opened file for writing...");
				long time = millis();
				for (uint32_t index = 0; index < dataSize;
					 index += CMD_TEXT_MAX_LEN) {
					byte byteIndex;
					for (byteIndex = 0;
						 (byteIndex < CMD_TEXT_MAX_LEN)
						 && ((index + byteIndex) < dataSize);
						 byteIndex++) {
						usbBuffer[byteIndex] = SerialReadByte();
					}

					yield();

					bool result = spiffsFile.write(usbBuffer, byteIndex);
					if (!result) {
						msSystem.slogln("ERROR: could not write!");
					}
					//WriteBytes(sectorAddr + index, usbBuffer, byteIndex);

					yield();

					int currentLedIdx = (16 * index)/dataSize;
					if (currentLedIdx > ledIdx && ledIdx <= 15) {
						ledIdx = currentLedIdx;
						msSystem.msLEDs.setLED(ledIdx, 0,15, 0, 2);
						msSystem.msLEDs.updateLEDs();
					}
				}
				time = millis() - time;

				spiffsFile.close();
				msSystem.slogln("File Upload DONE!");
			}
		} else {
			Serial.print("UNKNOWN CMD: \"");
			Serial.print((const char *) usbBuffer);
			Serial.println("\"");
		}
		msSystem.msLEDs.fillLEDs(0, 20, 0, 0x0f);
		msSystem.msLEDs.updateLEDs();
	}
}
