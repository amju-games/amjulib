#ifndef FILE_UPDATER_H_INCLUDED
#define FILE_UPDATER_H_INCLUDED

#include "Ve1Req.h"

namespace Amju
{
// Queries server for list of files which have been updated since the last time we checked.
// These files are deleted locally so they will be re-downloaded by the ObjectManager.
class FileUpdater : public Ve1Req
{
public:
  FileUpdater(const std::string& url);
  virtual void OnSuccess();
  virtual void OnFailure();

};
}

#endif

