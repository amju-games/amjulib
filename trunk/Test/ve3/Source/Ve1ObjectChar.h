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
  const Vec3f& GetMoveToPos() const { return m_newPos; }
  bool IsMoving() const;

  // Get where we are headed
  const Vec3f& GetTargetPos();

  virtual void SetEditMenu(GuiMenu*);
  
  virtual void SetKeyVal(const std::string& key, const std::string& val);

  // Show effect because local player wants to attack this char
  void ShowAttacked();
  AttackEffect* GetEffect() { return m_effect; }

  // Set (x, z) scale for 2D characters
  void SetScale(const Vec2f& scale);
  const Vec2f& GetScale() const;

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

