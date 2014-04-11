#include <GuiComposite.h>
#include "MsgManager.h"
#include "Player.h"
#include "Mugshot.h"
#include "ObjectManager.h"

namespace Amju
{
void ShowMsgHelper(int msgId, GuiElement* gui)
{
  // Get msg from msg manager.
  const MsgManager::Msg* msg = TheMsgManager::Instance()->GetMsg(msgId);
  Assert(msg);

  // Display msg text
  GuiText* text = (GuiText*)GetElementByName(gui, "orig-msg-text");
  Assert(text);
  text->SetText(msg->m_text);

  // mugshot of sender
  Player* sender = dynamic_cast<Player*>(
    TheObjectManager::Instance()->GetGameObject(msg->m_senderId).GetPtr());

  if (sender)
  {
    // TODO display this under mugshot
    std::string senderName = sender->GetName();

    Mugshot* mug = new Mugshot;
    mug->Init(sender);
    GuiComposite* comp = dynamic_cast<GuiComposite*>(gui);
    if (comp)
    {
      mug->SetLocalPos(Vec2f(-0.9f, 0.7f));
      comp->AddChild(mug);
    }
  }
}
}

