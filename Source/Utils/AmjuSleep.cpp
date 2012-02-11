#include "AmjuSleep.h"

#ifdef MACOSX
#include <unistd.h>
#endif

void SleepMs(int ms)
{
#ifdef MACOSX
  useconds_t usecs = ms * 1000;
  usleep(usecs);
#endif

#ifdef WIN32
  
#endif
}


