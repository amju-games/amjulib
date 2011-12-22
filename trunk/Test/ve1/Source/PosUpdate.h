#ifndef POS_UPDATE_H
#define POS_UPDATE_H

#include "Ve1Req.h"

namespace Amju
{
class PosUpdateReq : public Ve1Req
{
public:
  PosUpdateReq(const std::string& url);

  virtual void HandleResult();
};
}

#endif

