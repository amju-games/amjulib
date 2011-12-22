#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Ve1Object.h"

namespace Amju
{
class Ve1Character;

class Player : public Ve1Object
{
public:
  static const char* TYPENAME;

  Player();
  virtual const char* GetTypeName() const { return TYPENAME; }
  virtual void Update();
  virtual void Draw();
  virtual bool Load(File*);
 
  bool Init();

  // Ve1Object overrides
  virtual void MoveTo(const Vec3f& newPos);
  virtual void Set(const std::string& key, const std::string& val);
  virtual void SetMenu(GuiMenu*);

  // Set direction which character should face - we rotate to face this
  //  direction over time - DEGREES
  void SetDir(float degs);

  virtual AABB* GetAABB();

protected:
  void TurnToFaceDir();

protected:
  bool m_posHasBeenSet; // first pos request we set pos immediately, then move to it subsequently
  Vec3f m_newPos; // pos we are aiming for
  Ve1Character* m_sceneNode;

  float m_dir; // Direction of movement - DEGREES
  float m_dirCurrent; // Direction we currently face -- we will reduce the difference
   // between m_dirCurrent and m_dir until we face the direction of movement.
};

}

#endif

