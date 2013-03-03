#ifndef VE1_OBJECT_CHAR_H_INCLUDED
#define VE1_OBJECT_CHAR_H_INCLUDED

#include <Shadow.h>
#include <SceneMesh.h>
#include "Ve1Object.h"
#include "AttackEffect.h"

namespace Amju
{
class Ve1Character; // corresponding scene node type

// Base class for Player (local and non-local) and NPCs
class Ve1ObjectChar : public Ve1Object
{
public:
  Ve1ObjectChar();

  virtual void Update();

  const std::string& GetName() const;
  void SetName(const std::string& name);

  virtual void MoveTo(const Vec3f& newPos);

  virtual AABB* GetAABB();

  bool IsMoving() const;

  // Get where we are headed
  const Vec3f& GetTargetPos();

  virtual void SetEditMenu(GuiMenu*);
  
  virtual void SetKeyVal(const std::string& key, const std::string& val);

  // Show effect because local player wants to attack this char
  void ShowAttacked();

protected:
  std::string m_name;
  Vec3f m_newPos; // pos we are aiming for
  bool m_isMoving; // travelling towards m_newPos

  // Start fading out when a player has logged out
  float m_fadeTime;

  bool m_recalcHeading; // if true, recalc

  RCPtr<AttackEffect> m_effect;

  int m_stamina; // health - like Fighting Fantasy

  // used to find if we overshot destination
  float sqLenLastTime;
};
}

#endif

