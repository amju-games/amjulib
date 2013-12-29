#ifndef GS_VE3GUESTBOOK_H_INCLUDED
#define GS_VE3GUESTBOOK_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"
#include "Player.h"

namespace Amju 
{
class GSVe3Guestbook : public GSGui
{
protected:
  GSVe3Guestbook();
  friend class Singleton<GSVe3Guestbook>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  void SetPlayer(Player*);
  void OnAddCommentButton();

protected:
  virtual void InitGB();

protected:
  RCPtr<Player> m_player;
};
typedef Singleton<GSVe3Guestbook> TheGSVe3Guestbook;
} // namespace
#endif
