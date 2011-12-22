#ifndef STATE_UPDATE_H
#define STATE_UPDATE_H

#include "Ve1Req.h"

namespace Amju
{
class GetStateUpdatesReq : public Ve1Req
{
public:
  GetStateUpdatesReq(const std::string& url);

  virtual void HandleResult();
};
}

#endif

