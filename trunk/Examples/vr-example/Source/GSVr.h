#ifndef GS_VR_H_INCLUDED
#define GS_VR_H_INCLUDED

#include <Singleton.h>
#include <GameState.h>
#include <ObjMesh.h>

namespace Amju 
{
class GSVr : public GameState
{
  GSVr();
  friend class Singleton<GSVr>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();
  
  virtual bool OnRotationEvent(const RotationEvent&) override;
  // Movement - TODO use messages
  virtual bool OnKeyEvent(const KeyEvent&) override;

private:
  void DrawScene();
  
  Vec3f m_viewDir;
  Vec3f m_upDir;
  
  Vec3f m_pos;
  Vec3f m_vel;

  ObjMesh* m_mesh;
};
typedef Singleton<GSVr> TheGSVr;
} // namespace
#endif
