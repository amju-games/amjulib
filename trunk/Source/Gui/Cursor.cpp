#include "Cursor.h"
#include "ResourceManager.h"
#include "Pause.h"
#include "EventPoller.h"
#include "Screen.h"

namespace Amju
{
static const int CURSOR_PRIORITY = -2; // highest, so always responds

Cursor::Cursor() : m_id(-1)
{
  m_isActive = false;
  TheEventPoller::Instance()->AddListener(this, CURSOR_PRIORITY);

  m_rot= 0;

  static const float W = 0.1f;
  static const float H = 0.1f;

  AmjuGL::Vert verts[4] = 
  {
    AmjuGL::Vert( W, -H, 0,   1, 0,   0, 1, 0), // x, y, z, u, v, nx, ny, nz  
    AmjuGL::Vert( W,  H, 0,   1, 1,   0, 1, 0),
    AmjuGL::Vert(-W,  H, 0,   0, 1,   0, 1, 0),
    AmjuGL::Vert(-W, -H, 0,   0, 0,   0, 1, 0)
  };

  m_tris.reserve(2);
  AmjuGL::Tri tri;
  tri.m_verts[0] = verts[0];
  tri.m_verts[1] = verts[1];
  tri.m_verts[2] = verts[2];
  m_tris.push_back(tri);

  tri.m_verts[0] = verts[0];
  tri.m_verts[1] = verts[2];
  tri.m_verts[2] = verts[3];
  m_tris.push_back(tri);
}

const Vec2f& Cursor::GetPos() const
{
  return m_pos;
}

bool Cursor::Load(int id)
{
  m_id = id;
  // TODO CONFIG
  m_pTex = (Texture*)TheResourceManager::Instance()->GetRes("hand.png");
  Assert(m_pTex);
  return true;
}

bool Cursor::OnCursorEvent(const CursorEvent& ce)
{
  Assert(m_id > -1);
  
  if (ce.controller == m_id) 
  {
    m_isActive = true;
    m_pos.x = ce.x;
    m_pos.y = ce.y;
  }
  return false;  // never treat as handled
}

bool Cursor::OnRotationEvent(const RotationEvent& re)
{
  Assert(m_id > -1);
  
  if (re.controller == m_id) 
  {
    if (re.axis == AMJU_AXIS_Z)
    {
      m_rot = re.degs;
    }
  }
  return false;
}

void Cursor::Draw()
{
  if (m_isActive)
  {
    m_pTex->UseThisTexture();

    AmjuGL::PushMatrix();
    // Convert coords from 0..1 to -1..1
    AmjuGL::Translate(m_pos.x, m_pos.y, 0);
    // Rotate cursor
    AmjuGL::RotateZ(m_rot);
    AmjuGL::DrawTriList(m_tris);
    AmjuGL::PopMatrix();
  }
}
}
