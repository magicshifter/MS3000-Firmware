#ifndef _MAGNETOMETER_MODE_H
#define _MAGNETOMETER_MODE_H

//
// render the magnetometer sensor data
// 

class MagicMagnetMode:public MagicShifterBaseMode {

  private:

	MS3KG_App_Magnet &_magnet = msGlobals.pbuf.applications.magnet;
;

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

		int new_magnet_mode = _magnet.mode;


		if (msSystem.msButtons.msBtnAHit) {
			new_magnet_mode--;
			msSystem.msButtons.msBtnAHit = false;
		}
		if (msSystem.msButtons.msBtnBHit) {
			new_magnet_mode++;
			msSystem.msButtons.msBtnBHit = false;
		}


		if (new_magnet_mode < _MS3KG_App_Magnet_Mode_MIN)
			new_magnet_mode = _MS3KG_App_Magnet_Mode_MAX;
		
		if (new_magnet_mode > _MS3KG_App_Magnet_Mode_MAX)
			new_magnet_mode = _MS3KG_App_Magnet_Mode_MIN;

		msSystem.msSensor.readMagnetometerData(msGlobals.ggMagnet);


		if ((msSystem.msSensor.readRegister(0x5E) & 0x02)) {
			autoCalResetCounter++;
			if (autoCalResetCounter > 10) {
				// M_CTRL_REG2: Hybrid auto increment, Magnetic measurement min/max detection function reset

				msSystem.msSensor.writeRegister(0x5C, 0x24);
				autoCalResetCounter = 0;


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


		int degNorth = -degrees;
		int degSouth = 180 - degrees;

		if (degSouth>180) degSouth-=360;
		int ledNorth = map(abs(degNorth), 0, 180, 0, 15);
		int ledSouth = map(abs(degSouth), 0, 180, 0, 15);


		if (new_magnet_mode <= MS3KG_App_Magnet_Mode_BARS_DOT) {
			for (int lC = 0; lC < lednr; lC++)
				msSystem.msLEDs.setLED(lC, 0, 255, 0, msGlobals.ggBrightness);	// !J! hack

			for (int lC = lednr + 1; lC < MAX_LEDS; lC++)
				msSystem.msLEDs.setLED(lC, 255, 0, 0, msGlobals.ggBrightness);	// !J! hack
		}


		if ((new_magnet_mode == MS3KG_App_Magnet_Mode_BARS) || 
			(new_magnet_mode == MS3KG_App_Magnet_Mode_DOTS))
			msSystem.msLEDs.setLED(lednr, 0, 255, 0,
								   msGlobals.ggBrightness);

		if (new_magnet_mode == MS3KG_App_Magnet_Mode_BARS_DOT)
			msSystem.msLEDs.setLED(lednr, 0, 0, 255,
								   msGlobals.ggBrightness);

		if (new_magnet_mode == MS3KG_App_Magnet_Mode_OTHER) {		
			msSystem.msLEDs.setLED(ledSouth, 0, 255, 0, msGlobals.ggBrightness);
			msSystem.msLEDs.setLED(ledNorth, 255, 0, 0, msGlobals.ggBrightness);
		}


		_magnet.mode = (MS3KG_App_Magnet_Mode)new_magnet_mode;

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
