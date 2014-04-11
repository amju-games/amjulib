#pragma once

#include <Singleton.h>
#include "GSGui.h"

namespace Amju
{
class GSVe3ShowTrade : public GSGui
{
  GSVe3ShowTrade();
  friend class Singleton<GSVe3ShowTrade>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  void SetMsgId(int msgId) { m_msgId = msgId; }

  void OnTradeReject();

private:
  int m_msgId;
};
typedef Singleton<GSVe3ShowTrade> TheGSVe3ShowTrade;
}
