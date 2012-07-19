#include "AmjuSleep.h"
#ifdef WIN32
#include <Windows.h>
#endif

#ifdef MACOSX
#include <unistd.h>
#endif

namespace Amju
{
void SleepMs(int ms)
{
#ifdef MACOSX
  useconds_t usecs = ms * 1000;
  usleep(usecs);
#endif

#ifdef WIN32
  ::Sleep(ms);
#endif
}
}

