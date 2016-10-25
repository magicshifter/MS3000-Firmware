#ifndef _MODES_H
#define _MODES_H


#define NUM_MS_MODES 5

// rudimentary mode for displaying an msImage with POV
#include "UI/ShakeSync.h"

// OSC = Open Sound Control.  Can be used to send magnetometer/sensor data, for example
#ifdef CONFIG_ENABLE_OSC
#include "OSCMessage.h"
#endif // CONFIG_ENABLE_OSC

// MIDI modes and features can be configured, i.e. MIDIShifter project
#ifdef CONFIG_ENABLE_MIDI
#include "miby.h"
#include "MIDI/MibyConfig.h"
#include "MIDI/midi_defs.h"
#include "MIDI/envelope.h"
#include "MIDI/MagicArpeggiator.h"
MIDIArpeggiatorMode msMIDIArpeggiator;
#include "MIDI/RawMIDIHandlers.h"
#endif // CONFIG_ENABLE_MIDI

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

PowerEmergencyMode msPowerEmergencyMode;
ModeSelectorMode msModeSelector;
MagicShakeMode msMagicShake;
SystemTextMode msSysText;
MagicMagnetMode msMagicMagnet;
MagicLightMode msMagicLight;
MagicRemoteMode msMagicRemote;
MagicBeatMode msMagicBeat;

#endif
