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

  int GetDamage() const { return m_damage; }

protected:
  int m_damage;

  // So players can kill baddies - TODO
  int m_maxHealth;
  int m_health;
};
}

#endif

