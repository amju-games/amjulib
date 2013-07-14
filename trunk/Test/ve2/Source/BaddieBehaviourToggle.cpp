#include <Timer.h>
#include <AmjuAssert.h>
#include <SoundManager.h>
#include <AmjuRand.h>
#include "BaddieBehaviourToggle.h"
#include "SpriteNode.h"

namespace Amju
{
BBToggle::BBToggle()
{
  m_harmful = false;
  for (int i = 0; i < 4; i++)
  {
    m_time[i] = 0;
    m_cellRange[i] = 0;
  }
  m_timeInState = 0;
  m_maxTimeInState = 0;
}

void BBToggle::Update()
{
  float dt = TheTimer::Instance()->GetDt();
  m_timeInState += dt;
  if (m_timeInState > m_maxTimeInState)
  {
    Sprite* spr = &(m_baddie->GetSprite());

    m_timeInState = 0;
    m_harmful = !m_harmful;
    // New random time, within range for this state
    if (m_harmful) 
    {
      TheSoundManager::Instance()->PlayWav(m_sound[1]);

      m_maxTimeInState = Rnd(m_time[2], m_time[3]);
      spr->SetCellRange(m_cellRange[2], m_cellRange[3]);
      spr->SetCell(m_cellRange[2]);
    }
    else
    {
      TheSoundManager::Instance()->PlayWav(m_sound[0]);

      m_maxTimeInState = Rnd(m_time[0], m_time[1]);
      spr->SetCellRange(m_cellRange[0], m_cellRange[1]);
      spr->SetCell(m_cellRange[1]);
    }
  }
}

bool BBToggle::Load(File* f)
{
  for (int i = 0; i < 4; i++)
  {
    if (!f->GetFloat(&m_time[i]))
    {
      f->ReportError("Expected time " + ToString(i));
      return false;
    }
  }

  for (int i = 0; i < 4; i++)
  {
    if (!f->GetInteger(&m_cellRange[i]))
    {
      f->ReportError("Expected cell range " + ToString(i));
      return false;
    }
  }

  m_timeInState = 0;
  m_maxTimeInState = 0;

  // Get sound filenames or <none>
  for (int i = 0; i < 2; i++)
  {
    if (!f->GetDataLine(&m_sound[i]))
    {
      f->ReportError("Expected sound " + ToString(i));
      return false;
    }
    if (m_sound[i] == "<none>")
    {
      m_sound[i] = "";
    }
  }

  return true;
}
}
