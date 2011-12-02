#ifndef VE1_ONLINE_REQ_MANAGER_H
#define VE1_ONLINE_REQ_MANAGER_H

#include <OnlineReqManager.h>
#include <Singleton.h>

namespace Amju
{
enum Task
{
  MOVE_REQ,
  OBJECT_CHECK_REQ
};

// Make URL for connecting to server
std::string MakeUrl(Task);

// Make URL for simple file download request - no authentication
std::string MakeDownloadUrl(const std::string& filename);

typedef Singleton<OnlineReqManager> TheOnlineReqManager;
}

#endif

