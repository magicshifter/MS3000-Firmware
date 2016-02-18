#ifndef _MODES_H
#define _MODES_H


#define NUM_MS_MODES 5

// rudimentary mode for displaying an msImage with POV
#include "UI/ShakeSync.h"

#include "MagicPOV.h"
#include "SystemText.h"
#include "MagicMagnet.h"
#include "BouncingBall.h"
#include "MagicShake.h"
#include "MagicLight.h"
#include "MagicRemote.h"
#include "MagicBeat.h"

#include "ModeSelector.h"


// MIDI can be configured on or off 
#ifdef CONFIG_ENABLE_MIDI
#include "MIDI/MidiShifter.h"
MIDIShifterMode msMIDIShifter;
#endif

ModeSelectorMode msModeSelector;

MagicShakeMode msMagicShake;
SystemTextMode msSysText;
MagicMagnetMode msMagicMagnet;
MagicLightMode msMagicLight;
MagicRemoteMode msMagicRemote;
MagicBeatMode msMagicBeat;

#endif
