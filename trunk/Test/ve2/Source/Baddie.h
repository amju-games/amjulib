#ifndef BADDIE_H_INCLUDED
#define BADDIE_H_INCLUDED

#include "Ve1ObjectChar.h"

namespace Amju
{
class Baddie : public Ve1Object
{
public:
  static const char* TYPENAME;

  Baddie();
  virtual const char* GetTypeName() const;
  virtual void Update();
  virtual bool Load(File*);
  virtual void SetMenu(GuiMenu* menu);

  virtual int GetDamage() const { return m_damage; }

  void OnCollideBullet();

protected:
  int m_damage; // damage inflicked on player on contact
  bool m_isDestructible;

  // So players can kill baddies - TODO
  int m_maxHealth;
  int m_health;

  // Chase player?
  float m_chaseSpeed;
};

// Toggles between harmful and harmless state
// e.g. grabber, moving spike, guillotine
class ToggleBaddie : public Baddie
{
public:
  ToggleBaddie();

  virtual void Update();
  virtual int GetDamage() const;
  virtual bool Load(File*);

protected:
  bool m_harmful; // toggles
  // 0-1 (min, max) time in harmless state, 2-3 (min, max) time in harmful state
  float m_time[4]; 
  int m_cellRange[4]; // 0-1 harmless, 2-3 harmful cell range
  float m_timeInState;
  float m_maxTimeInState;
};
}

#endif

