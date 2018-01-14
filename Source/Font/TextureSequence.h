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
  virtual ~TextureSequence();

  virtual bool Load(File*);

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
  // Override this for packed atlases. This default impl assumes a grid of fixed-size
  //  elements in the atlas.
  virtual void GetElementInAtlas(
    int ch, // the element index, e.g. a character code, from ' ' to 'z' etc. Can be >255 for unicode
    float& x, float& y, // position offset (would be zero if all elements are equal size)
    float& w, float& h, // size of glyph in relation to entire atlas
    float& u0, float& v0, // top left UV coord
    float& u1, float& v1 // bottom right UV coord
  );

protected:
  PTexture m_pTexture; 
  int m_numElements;

  // Scale factor in x and y ... not much use?
  float m_sizex, m_sizey;

  // Number of elements in the texture in x and y axes
  // Only makes sense for a grid of fixed-size cells, e.g. a character atlas created
  //  with Bitmap Font Builder, but not a packed atlas, e.g. created with BMFont.
  // Also does make sense for a sprite sheet, again of fixed-size cells.
  int m_numElementsX, m_numElementsY;
};

}

#endif

