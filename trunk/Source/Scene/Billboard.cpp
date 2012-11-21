#include "Billboard.h"
#include "File.h"
#include "ResourceManager.h"
#include <AmjuGL.h>

namespace Amju
{
Billboard::Billboard()
{
  m_size = 0;
  SetBlended(true);
}

void Billboard::SetTexture(Texture* tex)
{
  m_texture = tex;
}

void Billboard::SetSize(float size)
{
  m_size = size;
}

bool Billboard::Load(File* f)
{
  // Texture
  std::string s;
  if (!f->GetDataLine(&s))
  {
    f->ReportError("Expected texture name");
    return false;
  }
  m_texture = (Texture*)TheResourceManager::Instance()->GetRes(s);
  Assert(m_texture);

  if (!f->GetFloat(&m_size))
  {
    f->ReportError("Expected billboard size");
    return false;
  }
  return true;
}

void Billboard::Draw()
{
  Matrix mat;
  mat.ModelView(); // Get Modelview matrix
  mat = m_combined * mat;
  Vec3f up(mat[1], mat[5], mat[9]);
  Vec3f right(mat[0], mat[4], mat[8]);

  m_tris.resize(2);

  Vec3f v0 = ( up + right) * m_size;
  Vec3f v1 = ( up - right) * m_size;
  Vec3f v2 = (-up - right) * m_size;
  Vec3f v3 = (-up + right) * m_size;

  AmjuGL::Vert verts[4] = 
  {
    AmjuGL::Vert(v0.x, v0.y, v0.z,   1, 0,   0, 1, 0), // x, y, z, u, v, nx, ny, nz  
    AmjuGL::Vert(v1.x, v1.y, v1.z,   1, 1,   0, 1, 0),
    AmjuGL::Vert(v2.x, v2.y, v2.z,   0, 1,   0, 1, 0),
    AmjuGL::Vert(v3.x, v3.y, v3.z,   0, 0,   0, 1, 0)
  };

  AmjuGL::Tri* tri = &m_tris[0];
  tri->m_verts[0] = verts[0];
  tri->m_verts[1] = verts[1];
  tri->m_verts[2] = verts[2];

  tri = &m_tris[1];
  tri->m_verts[0] = verts[0];
  tri->m_verts[1] = verts[2];
  tri->m_verts[2] = verts[3];

  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_WRITE); // TODO have a member flag
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::PushMatrix();
  AmjuGL::MultMatrix(m_combined); // NB combined
  m_texture->UseThisTexture();
  AmjuGL::DrawTriList(m_tris);
  AmjuGL::PopMatrix();
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_WRITE);
}

void Billboard::Update()
{
}
}

