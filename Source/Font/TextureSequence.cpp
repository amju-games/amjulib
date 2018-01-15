/*
Amju Games source code (c) Copyright Jason Colman 2004
*/

#include <AmjuFirst.h>
#include <iostream>
#include "Common.h"
#include "TextureSequence.h"
#include "ResourceManager.h"
#include "File.h"
#include "Matrix.h"
#include "AmjuAssert.h"
#include <AmjuFinal.h>

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
  // Not required here, done later anyway?!
  //m_pTexture = (Texture*)TheResourceManager::Instance()->GetRes(texname);

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
  m_pTexture->SetWrapMode(AmjuGL::AMJU_TEXTURE_CLAMP); // helps improve look of text?

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

void TextureSequence::GetElementInAtlas(
  int element, // the element index, e.g. a character code, from ' ' to 'z' etc. Can be >255 for unicode
  float& x, float& y, // position offset (would be zero if all elements are equal size)
  float& w, float& h, // size of glyph in relation to entire atlas
  float& u0, float& v0, // top left UV coord
  float& u1, float& v1 // bottom right UV coord
)
{
  x = 0;
  y = 0; // As each element is the same size, there is no position offset for any element

  // Size of elements in x and y in UV space
  float usize = 1.0f / (float)m_numElementsX;
  float vsize = 1.0f / (float)m_numElementsY;
  w = 1.f; // usize;
  h = 1.f; // vsize;

  u0 = float(element % m_numElementsX) * usize;
  v0 = float(element / m_numElementsX) * vsize;

  u1 = u0 + usize;
  v1 = v0 + vsize;
}

void TextureSequence::MakeTris(int element, float size, AmjuGL::Tri tris[2], float xOff, float yOff)
{
  AMJU_CALL_STACK;

  Assert(m_pTexture);

  // Find element UV region and (x, y) position offset. 
  float x = 0;
  float y = 0;
  float w = 0;
  float h = 0;
  float u0 = 0;
  float v0 = 0;
  float u1 = 0;
  float v1 = 0;
  GetElementInAtlas(element, x, y, w, h, u0, v0, u1, v1);

  float sizeX = m_sizex * size * w;
  float sizeY = m_sizey * size * h;
  x *= m_sizex * size;
  y *= m_sizey * size;

  const float Z = 0.0f;
  AmjuGL::Vert v[4] =
  {
    AmjuGL::Vert(xOff + x,         yOff + y,         Z,  u0, 1.0f - v1,     0, 1.0f, 0),
    AmjuGL::Vert(xOff + x + sizeX, yOff + y,         Z,  u1, 1.0f - v1,     0, 1.0f, 0),
    AmjuGL::Vert(xOff + x + sizeX, yOff + y + sizeY, Z,  u1, 1.0f - v0,     0, 1.0f, 0),
    AmjuGL::Vert(xOff + x,         yOff + y + sizeY, Z,  u0, 1.0f - v0,     0, 1.0f, 0)
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
