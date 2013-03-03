#ifndef CRASH_MONITOR_H_INCLUDED
#define CRASH_MONITOR_H_INCLUDED

#include <Singleton.h>

namespace Amju
{
// Create at start of process. 
// Will be deleted at end of process if we exit gracefully.
// In ctor, we set a flag that is reset in the dtor. 

class CrashMonitor : public NonCopyable
{
private:
  // Check that the flag is reset - if not, we crashed last time.
  CrashMonitor();

public:
  // Reset flag
  ~CrashMonitor();

};

typedef Singleton<CrashMonitor> TheCrashMonitor;
}

#endif


