#ifndef VE1_OBJECT_CHAR_H_INCLUDED
#define VE1_OBJECT_CHAR_H_INCLUDED

#include <Shadow.h>
#include <SceneMesh.h>
#include "Ve1Object.h"

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

  // Set direction which character should face - we rotate to face this
  //  direction over time - DEGREES
  void SetDir(float degs);

  virtual AABB* GetAABB();


protected:
  void TurnToFaceDir();

protected:
  std::string m_name;
  Vec3f m_newPos; // pos we are aiming for
  bool m_isMoving; // travelling towards m_newPos

  RCPtr<Ve1Character> m_sceneNode;
 
  float m_dir; // Direction of movement - DEGREES
  float m_dirCurrent; // Direction we currently face -- we will reduce the difference
   // between m_dirCurrent and m_dir until we face the direction of movement.

  RCPtr<Shadow> m_shadow;

  // Set when we enter a new location - TODO s/b in Ve1Object ?
  bool m_inNewLocation;

  // Start fading out when a player has logged out
  float m_fadeTime;

};
}

#endif

