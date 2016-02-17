#ifndef _MODES_H
#define _MODES_H

#define NUM_MS_MODES 5

// rudimentary mode for displaying an msImage with POV
#include "UI/ShakeSync.h"

#include "BaseMode.h"
#include "MagicPOV.h"
#include "SystemText.h"
#include "MagicMagnet.h"
#include "BouncingBall.h"
#include "MagicShake.h"
#include "MagicLight.h"

#include "ModeSelector.h"


// MIDI can be configured on or off 
#ifdef CONFIG_ENABLE_MIDI
#include "MIDI/MidiShifter.h"
#endif


MagicShakeMode msMagicShake;
SystemTextMode msSysText;
MagicMagnetMode msMagicMagnet;
MagicLightMode msMagicLight;
ModeSelectorMode msModeSelector;



#endif
