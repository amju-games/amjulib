#include <AmjuFirst.h>
#include "DrawOBB3.h"
#include <AmjuGL.h>
#include <AmjuFinal.h>

namespace Amju
{
void DrawOBB3(const OBB3& b)
{
  AmjuGL::PushAttrib(AmjuGL::AMJU_TEXTURE_2D | AmjuGL::AMJU_LIGHTING);
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);

  LineSeg e[12];
  b.GetEdges(e);
  for (int i = 0; i < 12; i++)
  {
    AmjuGL::DrawLine(AmjuGL::Vec3(e[i].p0.x, e[i].p0.y, e[i].p0.z), 
                     AmjuGL::Vec3(e[i].p1.x, e[i].p1.y, e[i].p1.z));
  }

  AmjuGL::PopAttrib();
}

void DrawSolidOBB3(const OBB3& b)
{
  Vec3f c[8];
  b.GetCorners(c);

  AmjuGL::Tris tris;
  tris.reserve(12);
  AmjuGL::Tri t;

/*
          5
         / \
        /   \
       /     \
    4 +       + 6
      |\     /|
      | \   / |
      |  \ /  |
      |   7   |
    0 +   |   + 2
       \  |  /
        \ | /
         \|/
          3
*/

  // normals use box axes
  Vec3f A = b.GetAxis(0);
  Vec3f B = b.GetAxis(1);
  Vec3f C = b.GetAxis(2);

  t.m_verts[0] = AmjuGL::Vert(c[0].x, c[0].y, c[0].z, 0, 0,  -C.x, -C.y, -C.z);
  t.m_verts[1] = AmjuGL::Vert(c[1].x, c[1].y, c[1].z, 1, 1,  -C.x, -C.y, -C.z);
  t.m_verts[2] = AmjuGL::Vert(c[2].x, c[2].y, c[2].z, 1, 0,  -C.x, -C.y, -C.z); 
  tris.push_back(t);

  t.m_verts[0] = AmjuGL::Vert(c[0].x, c[0].y, c[0].z, 0, 0,  -C.x, -C.y, -C.z);  
  t.m_verts[1] = AmjuGL::Vert(c[2].x, c[2].y, c[2].z, 0, 1,  -C.x, -C.y, -C.z); 
  t.m_verts[2] = AmjuGL::Vert(c[3].x, c[3].y, c[3].z, 1, 1,  -C.x, -C.y, -C.z);  
  tris.push_back(t);

  t.m_verts[0] = AmjuGL::Vert(c[4].x, c[4].y, c[4].z, 1, 0,   C.x,  C.y,  C.z);   
  t.m_verts[1] = AmjuGL::Vert(c[6].x, c[6].y, c[6].z, 1, 1,   C.x,  C.y,  C.z);   
  t.m_verts[2] = AmjuGL::Vert(c[5].x, c[5].y, c[5].z, 0, 0,   C.x,  C.y,  C.z);   
  tris.push_back(t);

  t.m_verts[0] = AmjuGL::Vert(c[4].x, c[4].y, c[4].z, 1, 1,   C.x,  C.y,  C.z);  
  t.m_verts[1] = AmjuGL::Vert(c[7].x, c[7].y, c[7].z, 0, 1,   C.x,  C.y,  C.z);   
  t.m_verts[2] = AmjuGL::Vert(c[6].x, c[6].y, c[6].z, 0, 0,   C.x,  C.y,  C.z);   
  tris.push_back(t);

  t.m_verts[0] = AmjuGL::Vert(c[7].x, c[7].y, c[7].z, 1, 0,  -A.x, -A.y, -A.z);  
  t.m_verts[1] = AmjuGL::Vert(c[2].x, c[2].y, c[2].z, 1, 1,  -A.x, -A.y, -A.z);   
  t.m_verts[2] = AmjuGL::Vert(c[6].x, c[6].y, c[6].z, 0, 0,  -A.x, -A.y, -A.z);   
  tris.push_back(t);

  t.m_verts[0] = AmjuGL::Vert(c[7].x, c[7].y, c[7].z, 1, 1,  -A.x, -A.y, -A.z);   
  t.m_verts[1] = AmjuGL::Vert(c[3].x, c[3].y, c[3].z, 0, 1,  -A.x, -A.y, -A.z);   
  t.m_verts[2] = AmjuGL::Vert(c[2].x, c[2].y, c[2].z, 0, 0,  -A.x, -A.y, -A.z);   
  tris.push_back(t);

  t.m_verts[0] = AmjuGL::Vert(c[5].x, c[5].y, c[5].z, 1, 0,   A.x,  A.y,  A.z);   
  t.m_verts[1] = AmjuGL::Vert(c[0].x, c[0].y, c[0].z, 1, 1,   A.x,  A.y,  A.z);   
  t.m_verts[2] = AmjuGL::Vert(c[4].x, c[4].y, c[4].z, 0, 0,   A.x,  A.y,  A.z);   
  tris.push_back(t);

  t.m_verts[0] = AmjuGL::Vert(c[5].x, c[5].y, c[5].z, 1, 1,   A.x,  A.y,  A.z);   
  t.m_verts[1] = AmjuGL::Vert(c[1].x, c[1].y, c[1].z, 0, 1,   A.x,  A.y,  A.z);   
  t.m_verts[2] = AmjuGL::Vert(c[0].x, c[0].y, c[0].z, 0, 0,   A.x,  A.y,  A.z);   
  tris.push_back(t);

  t.m_verts[0] = AmjuGL::Vert(c[6].x, c[6].y, c[6].z, 1, 0,  -B.x, -B.y, -B.z); 
  t.m_verts[1] = AmjuGL::Vert(c[1].x, c[1].y, c[1].z, 1, 1,  -B.x, -B.y, -B.z); 
  t.m_verts[2] = AmjuGL::Vert(c[5].x, c[5].y, c[5].z, 0, 0,  -B.x, -B.y, -B.z); 
  tris.push_back(t);

  t.m_verts[0] = AmjuGL::Vert(c[6].x, c[6].y, c[6].z, 1, 1,  -B.x, -B.y, -B.z); 
  t.m_verts[1] = AmjuGL::Vert(c[2].x, c[2].y, c[2].z, 0, 1,  -B.x, -B.y, -B.z); 
  t.m_verts[2] = AmjuGL::Vert(c[1].x, c[1].y, c[1].z, 0, 0,  -B.x, -B.y, -B.z); 
  tris.push_back(t);

  t.m_verts[0] = AmjuGL::Vert(c[4].x, c[4].y, c[4].z, 1, 0,   B.x,  B.y,  B.z); 
  t.m_verts[1] = AmjuGL::Vert(c[3].x, c[3].y, c[3].z, 1, 1,   B.x,  B.y,  B.z); 
  t.m_verts[2] = AmjuGL::Vert(c[7].x, c[7].y, c[7].z, 0, 0,   B.x,  B.y,  B.z); 
  tris.push_back(t);

  t.m_verts[0] = AmjuGL::Vert(c[4].x, c[4].y, c[4].z, 1, 1,   B.x,  B.y,  B.z); 
  t.m_verts[1] = AmjuGL::Vert(c[0].x, c[0].y, c[0].z, 0, 1,   B.x,  B.y,  B.z); 
  t.m_verts[2] = AmjuGL::Vert(c[3].x, c[3].y, c[3].z, 0, 0,   B.x,  B.y,  B.z); 
  tris.push_back(t);
  
  AmjuGL::DrawTriList(tris);
}
}
