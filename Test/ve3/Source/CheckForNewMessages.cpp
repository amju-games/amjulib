#include <Game.h>
#include "CheckForNewMessages.h"
#include "MsgManager.h"
#include "GSVe3Guestbook.h"
#include "LocalPlayer.h"
#include "LurkMsg.h"
#include "GameConsts.h"

namespace Amju
{
static void OnNewMsgsNo()
{
  // ?
}

static void OnNewMsgsYes()
{
  static Game* game = TheGame::Instance();
  static GSVe3Guestbook* g = TheGSVe3Guestbook::Instance();
  g->SetPrevState(game->GetState());
  g->SetPlayer(GetLocalPlayer());
  game->SetCurrentState(g);
}

// Check MsgManager for new Msgs. If we find any, show a Lurk Msg.
// If player chooses to view msgs, go to our guestbook page.
void CheckForNewMessages()
{
  static MsgManager* mm = TheMsgManager::Instance();

  int msgs = mm->HasNewMsgs();
  if (msgs > 0)
  {
    mm->ResetNewMsgs();

    std::string str;
    if (msgs == 1)
    {
      str  = "You got a new message!\nWould you like to read it now?";
    }
    else
    {
      Assert(msgs > 1);
      str  = "You got some new messages!\nWould you like to read them now?";
    }

    TheLurker::Instance()->ShowYesNo(str, LURK_FG, LURK_BG, OnNewMsgsNo, OnNewMsgsYes);    
  }
}
}
