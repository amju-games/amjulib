#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <GameObject.h>

namespace Amju
{
class Animated;

class Player : public GameObject
{
public:
  static const char* TYPENAME;

  Player();
  virtual const char* GetTypeName() const { return TYPENAME; }
  virtual void Update();
  virtual void Draw();
  virtual bool Load(File*);
 
  bool Init();

  void MoveTo(const Vec3f& newPos) { m_newPos = newPos; }

protected:
  Vec3f m_newPos;
  Animated* m_sceneNode;
};

}

#endif

