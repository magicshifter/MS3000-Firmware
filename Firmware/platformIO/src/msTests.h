

void testAccelerometer()
{
	Serial.println("trying accel!");

	int d = 500;
	int b = 20;
	while (1) {
		if (msSystem.msSensorOK) {
			msSystem.msLEDs.fillLEDs(0, b, 0, 0xff);
		} else {
			msSystem.msLEDs.fillLEDs(b, 0, 0, 0xff);
		}
		msSystem.msLEDs.updateLEDs();
		delay(d);

		msSystem.msLEDs.fillLEDs(b, b, b, 0xff);
		msSystem.msLEDs.updateLEDs();
		delay(d);
	}
}


void testButtonForBOM_X()
{
	if (msGlobals.ggCurrentFrame % 1000 == 0) {
		msSystem.slogln("_");
	}

	if (msSystem.msButtons.msBtnPwrLongHit) {
		msSystem.msLEDs.setLED(1, 0, 0, 20, 20);
		msSystem.msLEDs.updateLEDs();
		delay(200);
	}
	if (msSystem.msButtons.msBtnPwrHit) {
		msSystem.msLEDs.setLED(1, 20, 20, 0, 15);
		msSystem.msLEDs.updateLEDs();
		delay(200);
	}
	if (msSystem.msButtons.msBtnALongHit) {
		msSystem.msLEDs.setLED(0, 20, 0, 20, 20);
		msSystem.msLEDs.updateLEDs();
		delay(200);
	}
	if (msSystem.msButtons.msBtnAHit) {
		msSystem.msLEDs.setLED(0, 20, 20, 0, 20);
		msSystem.msLEDs.updateLEDs();
		delay(200);
	}
	if (msSystem.msButtons.msBtnBLongHit) {
		msSystem.msLEDs.setLED(2, 20, 0, 20, 20);
		msSystem.msLEDs.updateLEDs();
		delay(200);
	}
	if (msSystem.msButtons.msBtnBHit) {
		msSystem.msLEDs.setLED(2, 20, 20, 0, 20);
		msSystem.msLEDs.updateLEDs();
		delay(200);
	}
}


void testSimpleButtons()
{

	double avgVal = 800;
	double avgF = 0.01;

	int cnt = 0;

	int minV = 1000;
	int maxV = 0;

	cnt++;

	int adVal = msSystem.getADValue();

	avgVal = avgF * adVal + (1 - avgF) * avgVal;

	if (minV > adVal)
		minV = adVal;
	if (maxV < adVal)
		maxV = adVal;

	if (adVal > 950) {
		msSystem.msLEDs.setLED(7, 0, 10, 0, msGlobals.ggBrightness);
		msSystem.msLEDs.setLED(8, 0, 10, 0, msGlobals.ggBrightness);
	} else {
		msSystem.msLEDs.setLED(7, 10, 0, 0, msGlobals.ggBrightness);
		msSystem.msLEDs.setLED(8, 10, 0, 0, msGlobals.ggBrightness);
	}

	if (!digitalRead(PIN_BUTTON_A)) {
		msSystem.msLEDs.setLED(9, 0, 10, 0, msGlobals.ggBrightness);
	} else {
		msSystem.msLEDs.setLED(9, 10, 0, 0, msGlobals.ggBrightness);
	}

	if (!digitalRead(PIN_BUTTON_B)) {
		msSystem.msLEDs.setLED(6, 0, 10, 0, msGlobals.ggBrightness);
	} else {
		msSystem.msLEDs.setLED(6, 10, 0, 0, msGlobals.ggBrightness);
	}
	if (cnt % 300 == 550) {
		minV++;
		maxV--;
	}

	if (cnt % 40 == 0) {
		Serial.print(minV);
		Serial.print(":");
		Serial.print(maxV);
		Serial.print("|");
		Serial.print(adVal);
		Serial.print("/");
		Serial.println(avgVal);
	}
	msSystem.msLEDs.updateLEDs();
	delay(10);
	//return;
}

void logButtons()
{
	msSystem.slog("msBtnA:");
	msSystem.slogln(String(msSystem.msButtons.msBtnAPressTime));
	msSystem.slog("msBtnPwr:");
	msSystem.slogln(String(msSystem.msButtons.msBtnPwrPressTime));
	msSystem.slog("msBtnB:");
	msSystem.slogln(String(msSystem.msButtons.msBtnBPressTime));
}

void doTests()
{

	// do some tests
	// testAccelerometer();
	// testButtonForBOM_X();
	// testSimpleButtons();
	// logButtons();
}
