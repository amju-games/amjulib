#ifndef BADDIE_H_INCLUDED
#define BADDIE_H_INCLUDED

#include "Ve1ObjectChar.h"

namespace Amju
{
// BADDIE TYPE
// Can inflict damage on local player.
// Toggles between harmful and harmless state
// e.g. grabber, moving spike, guillotine.
// In harmful mode, can chase player.
// Take damage, and so player can kill them.
class Baddie : public Ve1Object
{
public:
  static const char* TYPENAME;

  Baddie();
  virtual const char* GetTypeName() const;
  virtual void Update();
  virtual bool Load(File*);
  virtual void SetMenu(GuiMenu* menu);

  int GetDamage() const; 

  void OnCollideBullet();

protected:
  int m_damage; // damage inflicked on player on contact
  bool m_isDestructible;

  Vec2f m_size;

  // Players can kill baddies 
  int m_maxHealth;
  int m_health;

  // Chase player in harmful mode if speed > 0.
  // (Flees player if speed < 0! -- is this useful?)
  float m_chaseSpeed;

  bool m_harmful; // toggles

  // 0-1 (min, max) time in harmless state, 2-3 (min, max) time in harmful state
  float m_time[4]; 

  int m_cellRange[4]; // 0-1 harmless, 2-3 harmful cell range

  float m_timeInState;
  float m_maxTimeInState;

  std::string m_sound[2]; // harmless/harmful state change sound filename
};
}

#endif

