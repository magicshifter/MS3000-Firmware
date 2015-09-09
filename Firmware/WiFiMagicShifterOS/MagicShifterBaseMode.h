#ifndef _MAGICSHIFTERBASEMODE_H
#define _MAGICSHIFTERBASEMODE_H

#include "MagicShifterSystem.h"

// A MagicShifter mode can be activated by the user through system-events, i.e. buttons, shaking, etc.
// start happens when the user selects the mode
// stop happens when the user switches to another mode, before any other modes are started
// step allows the mode time to itself to compute, do and schedule things
class IMode
{
public:
  // start the mode
  void start(MagicShifterSystem *msSystem);
  // stop the mode
  void stop(void);
  // step through an event frame of the mode
  void step();
};

class MagicShifterBaseMode : public IMode
{
private:
  MagicShifterSystem *m_msSystem;

  public:
    // start the mode
    void start(MagicShifterSystem *msSystem)
    {
      m_msSystem = msSystem;
    }

    // stop the mode
    void stop(void)
    {
      m_msSystem = NULL;
    }

    // step through a frame of the mode 
    void step()
    {
      //frame++; &etc.
    }
};

#endif
