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

#include "PowerEmergency.h"

#include "ModeSelector.h"

#ifdef CONFIG_MIDI_ONBOARD
#include "MIDI/MidiShifter.h"
MIDIShifterMode msMIDIShifter;
#include "MIDI/MibyConfig.h"
#endif

PowerEmergencyMode msPowerEmergencyMode;

ModeSelectorMode msModeSelector;

MagicShakeMode msMagicShake;
SystemTextMode msSysText;
MagicMagnetMode msMagicMagnet;
MagicLightMode msMagicLight;
MagicRemoteMode msMagicRemote;
MagicBeatMode msMagicBeat;

#endif
