/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextureSequence.h,v $
Revision 1.6  2007/11/13 21:33:13  jay
Remove all OpenGL calls from game code

Revision 1.5  2006/11/29 23:01:37  jay
Print text in world space, for PetZoo pet names.

Revision 1.4  2006/03/10 19:20:28  jay
Add Set functions

Revision 1.3  2006/02/17 19:01:46  jay
New size parameter added to Draw(). Use this rather than glScale.

Revision 1.2  2004/10/17 18:38:33  jay
Load from File; store size

Revision 1.1  2004/09/08 15:42:59  jay
Added to repository
  
*/

#if !defined(TEXTURE_SEQUENCE_H_INCLUDED)
#define TEXTURE_SEQUENCE_H_INCLUDED

#include <string>
#include "RCPtr.h"
#include <Texture.h>
#include <AmjuGL.h>

namespace Amju
{
class File;

// Loads a texture which consists of a sequence of sub-textures.
// Examples are fonts and explosion sequences.
class TextureSequence : public RefCounted
{
public:
  TextureSequence();
  ~TextureSequence();

  bool Load(File* );

  bool Load(
    const std::string& texturename, 
    int numElementsX, int numElementsY,
    float sizeX, float sizeY);

  void Set(Texture* pTex,
    int numElementsX, int numElementsY,
    float sizeX, float sizeY);

  int GetNumElements() const;

  void MakeTris(int element, float size, AmjuGL::Tri tris[2], float xOff, float yOff);

  // Bind the texture. Doing this once up front instead of every call to
  // Draw() may be more efficient.
  void Bind();
  
  int GetTextureWidth();
  int GetTextureHeight();

  float GetSizeX() const { return m_sizex; }
  float GetSizeY() const { return m_sizey; }

  void SetSize(float width, float height);

  Texture* GetTexture() { return m_pTexture; }

protected:
  PTexture m_pTexture; 
  int m_numElements;
  float m_sizex, m_sizey;
  // Number of elements in the texture in x and y axes
  int m_numElementsX, m_numElementsY;
};
}

#endif

