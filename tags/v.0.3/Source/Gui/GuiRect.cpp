#include <Texture.h>
#include <ResourceManager.h>
#include "GuiRect.h"

namespace Amju
{
const char* GuiRect::NAME = "gui-rect";

GuiRect::GuiRect()
{
  m_cornerRadius = 0;
  m_flags = 0;
}

void GuiRect::SetColour(const Colour& colour)
{
  m_colour = colour;
}

void GuiRect::SetRoundCorners(unsigned int flags)
{
  m_flags = flags;
}

void GuiRect::SetCornerRadius(float r)
{
  m_cornerRadius = r;
}

void GuiRect::Draw()
{
  if (!IsVisible())
  {
    return;
  }

  // Get corner texture sequence/font
  static Texture* texture = (Texture*)TheResourceManager::Instance()->GetRes("corner.png"); // TODO CONFIG

  // Draw tri list - rebuild if we have changed

  if (GetCombinedPos() != m_oldPos || GetSize() != m_oldSize)
  {
    BuildTris();
    m_oldPos = GetCombinedPos();
    m_oldSize = GetSize();
  }

  Assert(texture);
  texture->UseThisTexture();

  PushColour();
  AmjuGL::SetColour(m_colour);
  AmjuGL::DrawTriList(m_tris);
  PopColour();
}

void GuiRect::BuildTris()
{
  Vec2f size = GetSize();
  Vec2f pos = GetCombinedPos();
  
  m_tris.clear();
  AmjuGL::Tri tri;

  if (m_cornerRadius < 0.0001f || m_flags == 0x0f)
  {
    // Solid Rect
    AmjuGL::Vert verts[4] =
    {
      AmjuGL::Vert(pos.x + size.x, pos.y - size.y, 0,   0.5, 0.5,   0, 1, 0), // x, y, z, u, v, nx, ny, nz
      AmjuGL::Vert(pos.x + size.x, pos.y, 0,            0.5, 0.5,   0, 1, 0),
      AmjuGL::Vert(pos.x, pos.y, 0,                     0.5, 0.5,   0, 1, 0),
      AmjuGL::Vert(pos.x, pos.y - size.y, 0,            0.5, 0.5,   0, 1, 0)
    };

    m_tris.reserve(2);
    tri.m_verts[0] = verts[0];
    tri.m_verts[1] = verts[1];
    tri.m_verts[2] = verts[2];
    m_tris.push_back(tri);

    tri.m_verts[0] = verts[0];
    tri.m_verts[1] = verts[2];
    tri.m_verts[2] = verts[3];
    m_tris.push_back(tri);
  }
  else
  {
    // Get aspect ratio so corners are square
    int vx, vy, vw, vh;
    AmjuGL::GetViewport(&vx, &vy, &vw, &vh);
    float asp = (float)vw / (float)vh;

    float x[4] = { pos.x, pos.x + m_cornerRadius, pos.x + size.x - m_cornerRadius, pos.x + size.x };
    float y[4] = { pos.y, pos.y - m_cornerRadius * asp, pos.y - size.y + m_cornerRadius * asp, pos.y - size.y };

    AmjuGL::Vert v[16] =
    {
      AmjuGL::Vert(x[0], y[0], 0,   0.0, 0,   0, 1, 0), 
      AmjuGL::Vert(x[1], y[0], 0,   0.5, 0,   0, 1, 0),
      AmjuGL::Vert(x[2], y[0], 0,   0.5, 0,   0, 1, 0),
      AmjuGL::Vert(x[3], y[0], 0,   1.0, 0,   0, 1, 0),

      AmjuGL::Vert(x[0], y[1], 0,   0.0, 0.5,   0, 1, 0), 
      AmjuGL::Vert(x[1], y[1], 0,   0.5, 0.5,   0, 1, 0),
      AmjuGL::Vert(x[2], y[1], 0,   0.5, 0.5,   0, 1, 0),
      AmjuGL::Vert(x[3], y[1], 0,   1.0, 0.5,   0, 1, 0),

      AmjuGL::Vert(x[0], y[2], 0,   0.0, 0.5,   0, 1, 0), 
      AmjuGL::Vert(x[1], y[2], 0,   0.5, 0.5,   0, 1, 0),
      AmjuGL::Vert(x[2], y[2], 0,   0.5, 0.5,   0, 1, 0),
      AmjuGL::Vert(x[3], y[2], 0,   1.0, 0.5,   0, 1, 0),

      AmjuGL::Vert(x[0], y[3], 0,   0.0, 1.0,   0, 1, 0), 
      AmjuGL::Vert(x[1], y[3], 0,   0.5, 1.0,   0, 1, 0),
      AmjuGL::Vert(x[2], y[3], 0,   0.5, 1.0,   0, 1, 0),
      AmjuGL::Vert(x[3], y[3], 0,   1.0, 1.0,   0, 1, 0),
    };

    if (m_flags & AMJU_TL)
    {
      v[0].m_u = v[0].m_v = 0.5f;
      v[1].m_u = v[1].m_v = 0.5f;
      v[4].m_u = v[4].m_v = 0.5f;
    }

    if (m_flags & AMJU_TR)
    {
      v[2].m_u = v[2].m_v = 0.5f;
      v[3].m_u = v[3].m_v = 0.5f;
      v[7].m_u = v[7].m_v = 0.5f;
    }
 
    if (m_flags & AMJU_BL)
    {
      v[8].m_u = v[8].m_v = 0.5f;
      v[12].m_u = v[12].m_v = 0.5f;
      v[13].m_u = v[13].m_v = 0.5f;
    }

    if (m_flags & AMJU_BR)
    {
      v[11].m_u = v[11].m_v = 0.5f;
      v[14].m_u = v[14].m_v = 0.5f;
      v[15].m_u = v[15].m_v = 0.5f;
    }
 
    m_tris.reserve(14);
    m_tris.push_back(AmjuGL::Tri(v[5], v[1], v[0]));
    m_tris.push_back(AmjuGL::Tri(v[5], v[0], v[4]));

    m_tris.push_back(AmjuGL::Tri(v[7], v[3], v[2]));
    m_tris.push_back(AmjuGL::Tri(v[7], v[2], v[6]));

    m_tris.push_back(AmjuGL::Tri(v[13], v[9], v[8]));
    m_tris.push_back(AmjuGL::Tri(v[13], v[8], v[12]));

    m_tris.push_back(AmjuGL::Tri(v[15], v[11], v[10]));
    m_tris.push_back(AmjuGL::Tri(v[15], v[10], v[14]));

    for (int i = 0; i < 16; i++)
    {
      v[i].m_u = v[i].m_v = 0.5f;
    }

    m_tris.push_back(AmjuGL::Tri(v[6], v[2], v[1]));
    m_tris.push_back(AmjuGL::Tri(v[6], v[1], v[5]));

    m_tris.push_back(AmjuGL::Tri(v[11], v[7], v[4]));
    m_tris.push_back(AmjuGL::Tri(v[11], v[4], v[8]));

    m_tris.push_back(AmjuGL::Tri(v[14], v[10], v[9]));
    m_tris.push_back(AmjuGL::Tri(v[14], v[9], v[13]));
  }
}

bool GuiRect::Load(File* f)
{
  if (!GuiElement::Load(f))
  {
    return false;
  }

  // Colour
  std::string colour;
  if (!f->GetDataLine(&colour))
  {
    f->ReportError("Expected gui rect colour");
    return false;
  }
  m_colour = FromHexString(colour);
 
  // Corner radius
  if (!f->GetFloat(&m_cornerRadius))
  {
    f->ReportError("Expected corner radius");
    return false;
  }

  // TODO Fix radius if too big

  // Rounded corners flags -- 15 means all square, 0 means all rounded.
  if (!f->GetInteger((int*)&m_flags))
  {
    f->ReportError("Expected corner flags");
    return false;
  }

  return true; 
}

}

