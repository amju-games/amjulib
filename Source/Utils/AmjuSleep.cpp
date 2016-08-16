#include <AmjuFirst.h>
#include "AmjuSleep.h"
#ifdef WIN32
#include <Windows.h>
#endif

#if defined(MACOSX) || defined(AMJU_IOS)
#include <unistd.h>
#endif

#include <AmjuFinal.h>

namespace Amju
{
void SleepMs(int ms)
{
  // TODO And probably GEKKO etc
#if defined(MACOSX) || defined(AMJU_IOS)
  useconds_t usecs = ms * 1000;
  usleep(usecs);
#endif

#ifdef WIN32
  ::Sleep(ms);
#endif
}
}

