#ifndef REQ_SET_OBJECT_FILENAMES_H_INCLUDED
#define REQ_SET_OBJECT_FILENAMES_H_INCLUDED

#include "Ve1Req.h"

namespace Amju
{
void SendReqSetObjectFilenames(int objId, const std::string& assetFilename, const std::string& dataFilename);

class ReqSetObjectFilenames : public Ve1Req
{
public:
  ReqSetObjectFilenames(const std::string& url);

  virtual void OnSuccess();
  virtual void OnFailure();
};
}
#endif

