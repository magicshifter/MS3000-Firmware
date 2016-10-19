#ifndef _MAGICSHIFTERBASEMODE_H
#define _MAGICSHIFTERBASEMODE_H


// #include "msSystem.h"

// A MagicShifter mode can be activated by the user through system-events, i.e. buttons, shaking, etc.
// start happens when the user selects the mode
// stop happens when the user switches to another mode, before any other modes are started
// step allows the mode time to itself to compute, do and schedule things

class MagicShifterBaseMode {

  private:

  public:
  	// a unique name for the  mode
	String modeName = "BaseMode";
	// start the mode

	// void start(MagicShifterSystem &msSystem)
	virtual void start() {};

	// stop the mode 
	virtual void stop(void) {};

	// step through a frame of the mode 
	virtual bool step() {};

};

#endif
