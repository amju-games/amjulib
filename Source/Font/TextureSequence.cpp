/*
Amju Games source code (c) Copyright Jason Colman 2004
*/

#include "AmjuFirst.h"
#include <iostream>
#include "Common.h"
#include "TextureSequence.h"
#include "ResourceManager.h"
#include "File.h"
#include "Matrix.h"
#include "AmjuAssert.h"
#include "AmjuFinal.h"

namespace Amju
{
TextureSequence::TextureSequence()
{
  AMJU_CALL_STACK;

  m_pTexture = 0;
  m_sizex = 0;
  m_sizey = 0;
  m_numElementsX = 0;
  m_numElementsY = 0;
}

TextureSequence::~TextureSequence()
{
  AMJU_CALL_STACK;

}

int TextureSequence::GetTextureWidth()
{
  AMJU_CALL_STACK;

  return m_pTexture->GetWidth();
}

int TextureSequence::GetTextureHeight()
{
  AMJU_CALL_STACK;

  return m_pTexture->GetHeight();
}

bool TextureSequence::Load(File* pf)
{
  AMJU_CALL_STACK;

  std::string texname;
  std::string alphaname;
  int numx = 0;
  int numy = 0;
  float sizex = 0;
  float sizey = 0;

  if (!pf->GetDataLine(&texname))
  {
    pf->ReportError("Expected texture name.");
    return false;
  }
  m_pTexture = (Texture*)TheResourceManager::Instance()->GetRes(texname);

  if (!pf->GetInteger(&numx))
  {
    pf->ReportError("Expected num of x elements");
    return false;
  }
  if (!pf->GetInteger(&numy))
  {
    pf->ReportError("Expected num of y elements.");
    return false;
  }
  if (!pf->GetFloat(&sizex))
  {
    pf->ReportError("Expected x size.");
    return false;
  }
  if (!pf->GetFloat(&sizey))
  {
    pf->ReportError("Expected y size.");
    return false;
  }
 
  return Load(texname, numx, numy, sizex, sizey);
}

void TextureSequence::SetSize(float width, float height)
{
  AMJU_CALL_STACK;

  m_sizex = width;
  m_sizey = height;

#ifdef TEXTURE_SEQ_DEBUG
std::cout << "TextureSeq: m_sizex: " << m_sizex << " m_sizey: " << m_sizey << "\n";
#endif
}

void TextureSequence::Set(Texture* pTex, 
  int numElementsX, int numElementsY,
  float sizeX, float sizeY)
{
  AMJU_CALL_STACK;

  m_pTexture = pTex;
  m_numElementsX = numElementsX;
  m_numElementsY = numElementsY;
  m_numElements = numElementsX * numElementsY;
  m_sizex = sizeX;
  m_sizey = sizeY;
}

bool TextureSequence::Load(
  const std::string& texturename, 
  int numElementsX, int numElementsY,
  float sizeX, float sizeY)
{
  AMJU_CALL_STACK;

  PTexture pTexture = (Texture*)TheResourceManager::Instance()->GetRes(texturename);

  if (!pTexture)
  {
    return false;
  }
  //pTexture->CreateBinding(Texture::AMJU_TEXTURE_REGULAR, false /* clamp, don't wrap */);

  Set(pTexture, numElementsX, numElementsY, sizeX, sizeY);

  return true;
}

int TextureSequence::GetNumElements() const
{
  AMJU_CALL_STACK;

  return m_numElements;
}

void TextureSequence::MakeTris(int element, float size, AmjuGL::Tri tris[2], float xOff, float yOff)
{
  AMJU_CALL_STACK;

  Assert(m_pTexture);

  float dx = 1.0f / (float)m_numElementsX;
  float dy = 1.0f / (float)m_numElementsY;
    
  float x = float(element % m_numElementsX) * dx;
  float y = float(element / m_numElementsX) * dy; // yes this is correct

  float sizeX = m_sizex * size;
  float sizeY = m_sizey * size;        

  const float Z = 0.5f;
  AmjuGL::Vert v[4] =
  {
    AmjuGL::Vert(xOff,         yOff,         Z,  x,      1.0f - y - dy,     0, 1.0f, 0),
    AmjuGL::Vert(xOff + sizeX, yOff,         Z,  x + dx, 1.0f - y - dy,     0, 1.0f, 0),
    AmjuGL::Vert(xOff + sizeX, yOff + sizeY, Z,  x + dx, 1.0f - y,          0, 1.0f, 0),
    AmjuGL::Vert(xOff,         yOff + sizeY, Z,  x,      1.0f - y,          0, 1.0f, 0)
  };

  tris[0].m_verts[0] = v[0];
  tris[0].m_verts[1] = v[1];
  tris[0].m_verts[2] = v[2];

  tris[1].m_verts[0] = v[0];
  tris[1].m_verts[1] = v[2];
  tris[1].m_verts[2] = v[3];
}

void TextureSequence::Draw(int element, float size)
{
  AMJU_CALL_STACK;

  Assert(m_pTexture);

  float dx = 1.0f / (float)m_numElementsX;
  float dy = 1.0f / (float)m_numElementsY;
    
  float x = float(element % m_numElementsX) * dx;
  float y = float(element / m_numElementsX) * dy; // yes this is correct

  float sizeX = m_sizex * size;
  float sizeY = m_sizey * size;        

  const float Z = 0.5f;
  AmjuGL::Vert v[4] =
  {
    AmjuGL::Vert(0,     0,     Z,  x,      1.0f - y - dy,     0, 1.0f, 0),
    AmjuGL::Vert(sizeX, 0,     Z,  x + dx, 1.0f - y - dy,     0, 1.0f, 0),
    AmjuGL::Vert(sizeX, sizeY, Z,  x + dx, 1.0f - y,          0, 1.0f, 0),
    AmjuGL::Vert(0,     sizeY, Z,  x,      1.0f - y,          0, 1.0f, 0)
  };

  AmjuGL::SetTextureMode(AmjuGL::AMJU_TEXTURE_REGULAR);
  AmjuGL::DrawQuad(v);

/*
  glBegin(GL_QUADS);

    glTexCoord2f(x, 1.0f - y - dy);
    glVertex2f(0, 0);
    
    glTexCoord2f(x + dx, 1.0f - y - dy);
    glVertex2f(sizeX, 0);
    
    glTexCoord2f(x + dx, 1.0f - y);
    glVertex2f(sizeX, sizeY);
    
    glTexCoord2f(x, 1.0f - y);
    glVertex2f(0, sizeY);
        
  glEnd();
*/
}

void TextureSequence::DrawBillboard(int element, float size, bool isVertical)
{
  AMJU_CALL_STACK;

  Assert(m_pTexture);

  float dx = 1.0f / (float)m_numElementsX;
  float dy = 1.0f / (float)m_numElementsY;
    
  float x = float(element % m_numElementsX) * dx;
  float y = float(element / m_numElementsX) * dy; // yes this is correct

//  float sizeX = m_sizex * size;
//  float sizeY = m_sizey * size;        

  Matrix m;
  m.ModelView();

  Matrix pr;
  pr.Projection();
  m = m * pr;

//  Vec3f right(m.at(0), m.at(4), m.at(8));
  Vec3f right(m[0], m[4], m[8]);
  right.Normalise();
  Vec3f up;

  if (isVertical)
  {
    up = Vec3f(0, 1, 0);
  }
  else
  {
    up = Vec3f(m[1], m[5], m[9]);
    up.Normalise();
  }

  // Get centre of billboard. As Billboards are SolidLeaves, we have an
  // orientation member.
  //Vec3f PT = m_or.GetVertex();
  Vec3f PT(0, 0, 0); // TODO TEMP TEST current orientation already in matrix,
   // because we got the ModelView matrix ?

  Vec3f t; // temp

  t = right;
  t *= -1;
  t -= up;
  t *= size;
  
  Vec3f A = PT;
  A += t;

  t = right;
  t -= up;
  t *= size;
  Vec3f B = PT;
  B += t;

  t = right;
  t += up;
  t *= size;
  Vec3f C = PT;
  C += t;

  t = up;
  t -= right;
  t *= size;
  Vec3f D = PT;
  D += t;

  AmjuGL::Vert v[4] =
  {
    AmjuGL::Vert(A.x, A.y, A.z,  x,      1.0f - y - dy,     0, 1.0f, 0),
    AmjuGL::Vert(B.x, B.y, B.z,  x + dx, 1.0f - y - dy,     0, 1.0f, 0),
    AmjuGL::Vert(C.x, C.y, C.z,  x + dx, 1.0f - y,          0, 1.0f, 0),
    AmjuGL::Vert(D.x, D.y, D.z,  x,      1.0f - y,          0, 1.0f, 0)
  };

  AmjuGL::SetTextureMode(AmjuGL::AMJU_TEXTURE_REGULAR);
  AmjuGL::DrawQuad(v);

/*
  glBegin(GL_QUADS);

    glTexCoord2f(x, 1.0f - y - dy);
    SceneVertex(A).DrawVertex();
    
    glTexCoord2f(x + dx, 1.0f - y - dy);
    SceneVertex(B).DrawVertex();
    
    glTexCoord2f(x + dx, 1.0f - y);
    SceneVertex(C).DrawVertex();
    
    glTexCoord2f(x, 1.0f - y);
    SceneVertex(D).DrawVertex();
        
  glEnd();
*/

}

void TextureSequence::Bind()
{
  AMJU_CALL_STACK;

  m_pTexture->UseThisTexture();
}
}
