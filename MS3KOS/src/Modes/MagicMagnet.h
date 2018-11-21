#ifndef _MAGNETOMETER_MODE_H
#define _MAGNETOMETER_MODE_H

//
// render the magnetometer sensor data
// 

class MagicMagnetMode:public MagicShifterBaseMode {

  private:

	int magnet_submode =  msGlobals.pbuf.apps.magnet.mode;

#ifdef CONFIG_ENABLE_OSC
	// local OSC message
	OSCMessage lOSCMessage = OSCMessage("/magicshifter3000/magnetometer/LED/");
#endif

  public:

  	MagicMagnetMode() { 
		 	modeName = "Compass";

	}

	void start() {
		
	} 

	void stop() {
	}

	bool step() {
		static int autoCalResetCounter = 0;

		magnet_submode =  msGlobals.pbuf.apps.magnet.mode;

		// msSystem.slog(modeName + " x: ");

		if (msSystem.msButtons.msBtnAHit) {
			magnet_submode--;
			msSystem.msButtons.msBtnAHit = false;
		}
		if (msSystem.msButtons.msBtnBHit) {
			magnet_submode++;
			msSystem.msButtons.msBtnBHit = false;
		}

		// msSystem.slog(modeName + " a: ");

		if (magnet_submode < _MS3KG_App_Magnet_Mode_MIN)
			magnet_submode = _MS3KG_App_Magnet_Mode_MAX;
		
		if (magnet_submode > _MS3KG_App_Magnet_Mode_MAX)
			magnet_submode = _MS3KG_App_Magnet_Mode_MIN;

		msSystem.msSensor.readMagnetometerData(msGlobals.ggMagnet);

		// msSystem.slog(modeName + " b: ");

		if ((msSystem.msSensor.readRegister(0x5E) & 0x02)) {
			autoCalResetCounter++;
			if (autoCalResetCounter > 10) {
				// M_CTRL_REG2: Hybrid auto increment, Magnetic measurement min/max detection function reset
		// msSystem.slog(modeName + " c: ");

				msSystem.msSensor.writeRegister(0x5C, 0x24);
				autoCalResetCounter = 0;
		// msSystem.slog(modeName + " d: ");


			}
		} else {
			autoCalResetCounter = 0;
		}

		int degrees =
			int (atan2
				 (msGlobals.ggMagnet[YAXIS],
				  -msGlobals.ggMagnet[XAXIS]) * 180 / M_PI);

		msSystem.slog(modeName + " degrees: " + String(degrees));

		msSystem.msLEDs.fillLEDs(0, 0, 0, msGlobals.ggBrightness);
		int lednr = map(abs(degrees), 0, 180, 0, 15);

		// msSystem.slog(modeName + " e: ");

		int degNorth = -degrees;
		int degSouth = 180 - degrees;

		if (degSouth>180) degSouth-=360;
		int ledNorth = map(abs(degNorth), 0, 180, 0, 15);
		int ledSouth = map(abs(degSouth), 0, 180, 0, 15);

		// msSystem.slog(modeName + " f: ");

		if (magnet_submode <= MS3KG_App_Magnet_Mode_BARS_DOT) {
			for (int lC = 0; lC < lednr; lC++)
				msSystem.msLEDs.setLED(lC, 0, 255, 0, msGlobals.ggBrightness);	// !J! hack

			for (int lC = lednr + 1; lC < MAX_LEDS; lC++)
				msSystem.msLEDs.setLED(lC, 255, 0, 0, msGlobals.ggBrightness);	// !J! hack
		}

		// msSystem.slog(modeName + " g: ");

		if ((magnet_submode == MS3KG_App_Magnet_Mode_BARS) || 
			(magnet_submode == MS3KG_App_Magnet_Mode_DOTS))
			msSystem.msLEDs.setLED(lednr, 0, 255, 0,
								   msGlobals.ggBrightness);

		if (magnet_submode == MS3KG_App_Magnet_Mode_BARS_DOT)
			msSystem.msLEDs.setLED(lednr, 0, 0, 255,
								   msGlobals.ggBrightness);

		if (magnet_submode == MS3KG_App_Magnet_Mode_OTHER) {		
			msSystem.msLEDs.setLED(ledSouth, 0, 255, 0, msGlobals.ggBrightness);
			msSystem.msLEDs.setLED(ledNorth, 255, 0, 0, msGlobals.ggBrightness);
		}

		// msSystem.slog(modeName + " h: " + String(magnet_submode));

 		msGlobals.pbuf.apps.magnet.mode = (MS3KG_App_Magnet_Mode)magnet_submode;

		msSystem.msLEDs.updateLEDs();

		// !J! hak: send OSC Msg.
#ifdef CONFIG_ENABLE_OSC
		lOSCMessage.add(lednr);
#endif

		delay(30);

		return true;

	}

};

#endif
