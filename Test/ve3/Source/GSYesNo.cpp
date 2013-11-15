#include <AmjuFirst.h>
#include <Game.h>
#include <GuiButton.h>
#include <GuiText.h>
#include "GSYesNo.h"
#include <AmjuFinal.h>

namespace Amju
{
static void OnYes()
{
  GSYesNo* yn = dynamic_cast<GSYesNo*>(TheGame::Instance()->GetState());
  Assert(yn);
  yn->OnYes();
}

static void OnNo()
{
  GSYesNo* yn = dynamic_cast<GSYesNo*>(TheGame::Instance()->GetState());
  Assert(yn);
  yn->OnNo();
}

GSYesNo::GSYesNo()
{
}

void GSYesNo::Update()
{
  GSGui::Update();

}

void GSYesNo::Draw()
{
  GSGui::Draw();

}

void GSYesNo::Draw2d()
{
  GSGui::Draw2d();
}

void GSYesNo::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-yesno.txt");
  Assert(m_gui);

  GuiButton* yes = (GuiButton*)GetElementByName(m_gui, "yes-button");
  yes->SetCommand(Amju::OnYes);
  yes->SetHasFocus(true);
  yes->SetText(m_yesText);

  GuiButton* no = (GuiButton*)GetElementByName(m_gui, "no-button");
  no->SetCommand(Amju::OnNo);
  no->SetIsCancelButton(true); 
  no->SetText(m_noText);
  
  GuiText* q = (GuiText*)GetElementByName(m_gui, "question");
  q->SetText(m_question);
}

} // namespace
