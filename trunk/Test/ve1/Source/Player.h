#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Ve1Object.h"

namespace Amju
{
class Animated;

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
  virtual bool UpdateState(PXml xml);

protected:
  bool m_posHasBeenSet; // first pos request we set pos immediately, then move to it subsequently
  Vec3f m_newPos; // pos we are aiming for
  Animated* m_sceneNode;
};

}

#endif

