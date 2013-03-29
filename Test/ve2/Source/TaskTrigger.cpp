/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
*/


#include <AmjuFirst.h>
#include <File.h>
#include <AmjuTime.h>
#include "TaskTrigger.h"
#include "LocalPlayer.h"
#include <AmjuFinal.h>

namespace Amju
{
TaskTrigger::TaskTrigger()
{
  AMJU_CALL_STACK;

  m_activated = false;
  m_setterId = -1; // not set
  m_pTask = 0; 
}

void TaskTrigger::SetTaskSetterId(int setterId)
{
  AMJU_CALL_STACK;

  m_setterId = setterId;
}

void TaskTrigger::SetTask(Task* pTask)
{
  AMJU_CALL_STACK;

  m_pTask = pTask;
}

/*
Pet* TaskTrigger::GetSetter() const
{
  AMJU_CALL_STACK;

  if (m_setterId == -1)
  {
    return 0;
  }

  PGameObject pGo = Engine::Instance()->GetGameObject(m_setterId);
  Assert(pGo.GetPtr());
  Pet* pPet = dynamic_cast<Pet*>(pGo.GetPtr());
  return pPet;
}
*/

bool TaskTriggerAlways::Load(File*)
{
  AMJU_CALL_STACK;

  return true;
}

bool TaskTriggerAlways::Save(File*)
{
  AMJU_CALL_STACK;

  return true;
}

bool TaskTriggerAlways::IsSatisfied() const
{
  AMJU_CALL_STACK;

  return true;
}

const char* TaskTriggerAlways::GetTypeName()
{
  AMJU_CALL_STACK;

  return TYPENAME;
}

const char* TaskTriggerAlways::TYPENAME = "always";
const char* TaskTriggerObjective::TYPENAME = "objective";
const char* TaskTriggerTime::TYPENAME = "time";

const char* TaskTriggerObjective::GetTypeName()
{
  AMJU_CALL_STACK;

  return TYPENAME;
}

bool TaskTriggerObjective::Load(File* pf)
{
  AMJU_CALL_STACK;

  return m_objective.Load(pf);
}

bool TaskTriggerObjective::Save(File* pf)
{
  AMJU_CALL_STACK;

  return m_objective.Save(pf);
}

void TaskTriggerObjective::SetTaskSetterId(int setterId) 
{
  AMJU_CALL_STACK;

  m_objective.ReplaceId(-1, setterId);
}

bool TaskTriggerObjective::IsSatisfied() const
{
  AMJU_CALL_STACK;

  return m_objective.IsSatisfied(false /* non-existent Game Objects do NOT fail immediately */);
}

TaskTriggerTime::TaskTriggerTime() :
  m_totalSecsPlayed(0),
  m_minutes(-1),
  m_hours(-1),
  m_dayOfWeek(-1),
  m_dayOfMonth(-1),
  m_month(-1),
  m_year(-1)
{
  AMJU_CALL_STACK;

}

bool TaskTriggerTime::Load(File* pf)
{
  AMJU_CALL_STACK;

  if (!pf->GetFloat(&m_totalSecsPlayed) ||
      !pf->GetInteger(&m_minutes) ||
      !pf->GetInteger(&m_hours) ||
      !pf->GetInteger(&m_dayOfWeek) ||
      !pf->GetInteger(&m_dayOfMonth) ||
      !pf->GetInteger(&m_month) ||
      !pf->GetInteger(&m_year) ||
      !pf->GetInteger(&m_validSecs))
  {
    pf->ReportError("Expected time trigger: secs/min/hr/dw/dm/m/y/<window>");
    return false;
  }
  return true;
}

bool TaskTriggerTime::Save(File* pf)
{
  AMJU_CALL_STACK;

  pf->WriteFloat(m_totalSecsPlayed);
  pf->WriteInteger(m_minutes);
  pf->WriteInteger(m_hours);
  pf->WriteInteger(m_dayOfWeek);
  pf->WriteInteger(m_dayOfMonth);
  pf->WriteInteger(m_month);
  pf->WriteInteger(m_year);
  pf->WriteInteger(m_validSecs);
  return true;
}

bool TaskTriggerTime::IsSatisfied() const
{
  AMJU_CALL_STACK;

  // Get total time the local player has been playing the game. 
  // This is in a key/val pair which we can update periodically on the current client, sending the
  //  new version to the server.
  float totalTime = 0;
  if (GetLocalPlayer()->Exists(TOTAL_PLAY_TIME_KEY))
  {
    totalTime = ToFloat(GetLocalPlayer()->GetVal(TOTAL_PLAY_TIME_KEY));
  }

#ifdef TIME_TRIGGER_DEBUG
std::cout << "Time trigger satisfied ? m_totalSecsPlayed: " 
  << m_totalSecsPlayed 
  << " total playing time: "  << totalTime << "\n";

if (m_totalSecsPlayed >= 0)
{
  std::cout << "Time until task will trigger: " 
    << m_totalSecsPlayed - totalTime 
    << "\n";
}
#endif

  // For each member with a value, check against the time (or time played).

  if (m_totalSecsPlayed >= 0)
  {
    return (m_totalSecsPlayed < totalTime);
  }

  Time t(Time::Now());
  if (m_minutes > -1 && m_minutes < t.GetMins())
  {
    return false;
  }
  if (m_hours > -1 && m_hours < t.GetHours())
  {
    return false;
  }
  if (m_dayOfMonth > -1 && m_dayOfMonth < t.GetDayOfMonth())
  {
    return false;
  }
  if (m_month > -1 && m_month < t.GetMonths())
  {
    return false;
  }
  if (m_year> -1 && m_year < t.GetYears())
  {
    return false;
  }
  // Now we have to work out if the current time exceeds the window since the trigger
  // would have first been activated.
  // TODO
  return true;
}

const char* TaskTriggerTime::GetTypeName()
{
  AMJU_CALL_STACK;

  return TYPENAME;
}

}
