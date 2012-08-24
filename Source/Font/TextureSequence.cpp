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

void TextureSequence::Bind()
{
  AMJU_CALL_STACK;

  m_pTexture->UseThisTexture();
}
}
