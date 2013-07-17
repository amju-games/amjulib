#ifndef BADDIE_H_INCLUDED
#define BADDIE_H_INCLUDED

#include <Factory.h>
#include <Singleton.h>
#include "Ve1Object.h"
#include "SpriteNode.h"
#include "AttackEffect.h"

namespace Amju
{
class BaddieBehaviour;

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

  Sprite& GetSprite();

protected:
  int m_damage; // damage inflicked on player on contact
  bool m_isDestructible;

  Vec2f m_size;

  // Players can kill baddies 
  int m_maxHealth;
  int m_health;

  int m_points; // points player gets when destroyed

  RCPtr<BaddieBehaviour> m_bb;

  RCPtr<AttackEffect> m_effect;
};

// Base class for behaviours
class BaddieBehaviour : public RefCounted
{
public:
  BaddieBehaviour() : m_baddie(0) {}
  virtual ~BaddieBehaviour() {}
  void SetBaddie(Baddie* baddie) { m_baddie = baddie; }

  virtual void Update() = 0;
  virtual bool Load(File*) { return true; }

protected:
  Baddie* m_baddie;
};

typedef Singleton<Factory<BaddieBehaviour> > TheBBFactory;
}

#endif

