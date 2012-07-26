#include <Directory.h>
#include "CrashMonitor.h"
#include "File.h"

namespace Amju
{
static const char* CM_FILENAME = "/crashmonitor.txt";

CrashMonitor::CrashMonitor()
{
  // Check if file exists
  if (FileExists(File::GetRoot() + CM_FILENAME))
  {
std::cout << "Found crash monitor file from last run!\n";
    //  Report this to server! Show a message to player!
  }

  File f;
  if (!f.OpenWrite(CM_FILENAME))
  {
    // Can't write file - permissions problem ? 
    // Would be good to send report to server
  } 
}

CrashMonitor::~CrashMonitor()
{
  DeleteResult res = AmjuDeleteFile(File::GetRoot() + CM_FILENAME);
  if (res != AMJU_DELETED_OK)
  {
    // Argh, now we will think we crashed next process run :-(
std::cout << "*** FAILED TO DELETE CRASH MONITOR FILE ***\n";
    Assert(0);
  }
}

}


