#ifndef GS_VE3MSGREPLY_H_INCLUDED
#define GS_VE3MSGREPLY_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSVe3MsgReply : public GSGui
{
  GSVe3MsgReply();
  friend class Singleton<GSVe3MsgReply>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  void SetMsgId(int msgId) { m_msgId = msgId; }

  // Called when send button pressed
  void SendReply();

private:
  int m_msgId;
};
typedef Singleton<GSVe3MsgReply> TheGSVe3MsgReply;
} // namespace
#endif
