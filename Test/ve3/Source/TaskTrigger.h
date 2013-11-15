/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
*/

#ifndef TASK_TRIGGER_H_INCLUDED
#define TASK_TRIGGER_H_INCLUDED

#include <RCPtr.h>
#include "TimePeriod.h"
#include "Objective.h"

namespace Amju
{
class Task;

// Trigger for detecting when a Task should become enabled,
// and for detecting when a Task is completed.
class TaskTrigger : public RefCounted
{
public:
  TaskTrigger();
  virtual ~TaskTrigger() {}

  virtual bool Load(File*) = 0;
  virtual bool Save(File*) = 0;
  virtual bool IsSatisfied() const = 0;
  virtual const char* GetTypeName() = 0;
  // Call when the trigger is first activated.
  // Until the trigger is activated, IsSatisfied should return false.
  virtual void Activate() { m_activated = true; }

  // Set the ID of the object which has set the Task. The Task Triggers may
  // use this to check the state of the setter object.
  virtual void SetTaskSetterId(int setterId);

  // Set the Task for which this is a trigger
  void SetTask(Task*);

//protected:
  // Convenience function, gets the Pet which set the Task.
  // Returns 0 if no setter, or not a Pet.
//  Pet* GetSetter() const;

protected:
  bool m_activated;
  // ID of object which set the task
  int m_setterId;
  // Task for which this is a trigger
  Task* m_pTask;
};

// Trivial Trigger, always fires
class TaskTriggerAlways : public TaskTrigger
{
public:
  virtual bool Load(File*);
  virtual bool Save(File*);
  virtual bool IsSatisfied() const;
  virtual const char* GetTypeName();
  static const char* TYPENAME;
};


// Commonly used subclass, wraps Objective
class TaskTriggerObjective : public TaskTrigger
{
public:
  virtual bool Load(File*);
  virtual bool Save(File*);
  virtual bool IsSatisfied() const;
  virtual const char* GetTypeName();
  static const char* TYPENAME;
  // Checks object IDs in the Objective. Replaces -1 IDs with setterId.
  virtual void SetTaskSetterId(int setterId);
private:
  Objective m_objective;
};

// This kind of trigger is satisfied when the current time reaches some
// value, or the player has been playing for a certain amount of time.
class TaskTriggerTime : public TaskTrigger
{
public:
  TaskTriggerTime();
  virtual bool Load(File*);
  virtual bool Save(File*);
  virtual bool IsSatisfied() const;
  virtual const char* GetTypeName();
  static const char* TYPENAME;
protected:
  // NB All the tests below are "AND"ed together. If there is more than
  // one test, they must all be true for the trigger to be satisfied.
  // "OR" is done by having multiple triggers.

  // If >= 0, the trigger is satisfied when the player's total
  // time reaches this value.
  float m_totalSecsPlayed;

  // If >= 0, trigger is satisfied when the current system time MINS reaches this value
  int m_minutes;

  // If >= 0, trigger is satisfied when current system time HOURS reaches this value
  int m_hours;

  // If >= 0, trigger is satisfied when current system time DAY OF WEEK (1-7) reaches this value
  int m_dayOfWeek;

  // If >= 0, trigger is satisfied when current system time DAY OF MONTH (1- 31) reaches this value
  int m_dayOfMonth;

  // If >= 0, trigger is satisfied when current system time MONTH (1-12) reaches this value
  int m_month;

  // If >= 0, trigger is satisfied when current system time YEAR (full, e.g. 2006, not 6) reaches this value
  int m_year;

  // If the current/played time matches the above, there is a 'window' for which the trigger
  // is enabled. If the trigger is tested after this window it is not triggered.
  // So e.g. a trigger which is valid on your birthday can have a window of one day
  // so it will not be activated if the game is not started until the next day.
  int m_validSecs;
};

}

#endif

