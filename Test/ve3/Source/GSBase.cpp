#include <AmjuFirst.h>
#include "GSBase.h"
#include <Game.h>
#include <Timer.h>
#include <AmjuGL.h>
#include <GuiText.h>
#include <iostream>
#include "ObjectManager.h"
#include "PosUpdate.h"
#include "GSNetError.h"
#include "JoystickToCursor.h"
#include "ROConfig.h"
#include <AmjuFinal.h>

#ifdef _DEBUG
#define SHOW_QUEUE
#define SHOW_NUM_ERRORS
#endif

namespace Amju
{
GSBase::GSBase() : m_time(0), m_maxTime(5.0f)
{
  m_nextState = 0;
  m_drag = false;
  m_yrot = 0;
}

void GSBase::OnActive()
{
  GameState::OnActive();

  m_time = 0;

  // Set this state to be the previous state for the Error screen.
  // But not for the Error state!
  if (this != TheGSNetError::Instance())
  {
    TheGSNetError::Instance()->SetPrevState(this);
  }
}

void GSBase::SetNextState(GameState* next)
{
  m_nextState = next;
}

void GSBase::Draw2d()
{
#ifdef SHOW_QUEUE
  {
    static GuiText t;
    t.SetFgCol(Colour(1, 1, 1, 1));
    t.SetBgCol(Colour(0, 0, 0, 1));
    t.SetFontSize(0.5f); // TODO CONFIG
    t.SetIsMulti(true);
    t.SetLocalPos(Vec2f(-1.0f, 0.8f));
    t.SetSize(Vec2f(1.0f, 1.0f));
    t.SetJust(GuiText::AMJU_JUST_LEFT);

    static std::string old;
    std::string s;
    Strings strs = TheVe1ReqManager::Instance()->GetRequestNames();
    for (Strings::iterator it = strs.begin(); it != strs.end(); ++it)
    {
      s += *it;
      s += "\n";
    }

    //if (!s.empty()) // ? So on screen for longer ?
    {
      t.SetText(s);
    }
    t.Draw();

    s = Replace(s, "\n", " ");
    if (old != s)
    {
      std::cout << "Queue: " << s << "\n";
      old = s;
    }
  }
#endif

#ifdef SHOW_NUM_ERRORS
  static int showErrs = ROConfig()->GetInt("show-errors", 0);
  if (showErrs > 0)
  {
    // Show number of critical/non critical errors from server 
    static GuiText t;
    int critical = 0;
    int nonCritical = 0;
    Ve1Req::GetNumErrors(&critical, &nonCritical);
    // Get serial request queue size
    int q = TheVe1ReqManager::Instance()->CountAllReqs();
    t.SetSize(Vec2f(1.0f, 0.1f));
    t.SetJust(GuiText::AMJU_JUST_LEFT);
    t.SetDrawBg(true);
    t.SetLocalPos(Vec2f(-1.0f, 0.9f));
    std::string s = "Errs CR:" + ToString(critical) + " NC:" + ToString(nonCritical) + " Q: " + ToString(q);
    t.SetText(s);
    t.Draw();
  }
#endif

}

void GSBase::Draw()
{
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  const float FOVY = 60.0f;
  const float NEAR_PLANE = 1.0f;
  const float FAR_PLANE = 3000.0f;
  float aspect = 1.3f;
  AmjuGL::SetPerspectiveProjection(FOVY, aspect, NEAR_PLANE, FAR_PLANE);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();

  AmjuGL::LookAt(0, 5, 20,  0, 0, 0,  0, 1, 0);

  AmjuGL::RotateY(m_yrot);
}

void GSBase::Update()
{
  // Handle any completed requests
  TheVe1ReqManager::Instance()->Update();

  float dt = TheTimer::Instance()->GetDt();
  m_time += dt;

  TheJoystickToCursor()->Update();

/*
  if (m_time >= m_maxTime)
  {
    m_time = 0;
    if (!m_nextState)
    {
      std::cout << "Oops, no next state set\n";
      Assert(0);
    }
    TheGame::Instance()->SetCurrentState(m_nextState);
  }
*/
}
}
