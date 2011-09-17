#ifndef CURSOR_H
#define CURSOR_H

#include "EventListener.h"
#include "Vec2.h"
#include "Texture.h"

namespace Amju
{
class Cursor : public EventListener
{
public:
  Cursor();
  
  virtual bool OnCursorEvent(const CursorEvent& ce);  
  virtual bool OnRotationEvent(const RotationEvent&);
  
  bool Load(int id);
  void Draw();

  const Vec2f& GetPos() const;

private:
  PTexture m_pTex;
  float m_rot; // rotation about z
  Vec2f m_pos;
	AmjuGL::Tris m_tris;
  int m_id;
  bool m_isActive;
};
} 

#endif
