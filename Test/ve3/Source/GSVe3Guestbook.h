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
  virtual void OnDeactive();

  void SetPlayer(Player*);

  // Display Guestbook messages, or other types of message?
  void SetIsGuestbookOnly(bool guestbookOnly);

  void SetShowSentMsgs(bool sent) { m_showSentMsgs = sent; }
 
  void OnAddCommentButton();

  void OnShowSentMsgs();

protected:
  virtual void InitGB();

protected:
  RCPtr<Player> m_player;
  std::string m_unfinishedCommentStr;

  // If true, only show <gb> msgs
  bool m_guestbookOnly;

  // If true, only show msgs sent by player (non guestbook)
  bool m_showSentMsgs;
};
typedef Singleton<GSVe3Guestbook> TheGSVe3Guestbook;
} // namespace
#endif
