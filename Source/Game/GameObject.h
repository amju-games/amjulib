#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <map>
#include "RCPtr.h"
#include "Vec3.h"
#include "AABB.h"

namespace Amju
{
class File;

class GameObject : public RefCounted
{
public:
  GameObject();
  virtual ~GameObject() {}; 

  virtual const char* GetTypeName() const = 0;

  virtual void Update();

  virtual void Draw();
  virtual void DrawBlended();

  virtual bool Load(File*); // Loads ID
  virtual void Reset() {}

  void SetId(int id) { m_id = id; }
  int GetId() const { return m_id; }

  void SetPos(const Vec3f&);
  void SetVel(const Vec3f&);
  void SetAcc(const Vec3f&);

  const Vec3f& GetPos() const;
  const Vec3f& GetVel() const;
  const Vec3f& GetAcc() const;

  const AABB& GetAABB() const; 

protected:
  int m_id;
  Vec3f m_pos;
  Vec3f m_vel;
  Vec3f m_acc;
  AABB m_aabb; // all Game Objects have a bounding box
};
typedef RCPtr<GameObject> PGameObject;
}

#endif
