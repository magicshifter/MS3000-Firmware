#ifndef _MODES_H
#define _MODES_H

#define NUM_MS_MODES 6

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

MagicShakeMode msMagicShake;
SystemTextMode msSysText;
MagicMagnetMode msMagicMagnet;
MagicLightMode msMagicLight;
ModeSelectorMode msModeSelector;



#endif
