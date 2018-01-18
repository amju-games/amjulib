/*
Amju Games source code (c) Copyright Jason Colman 2005
*/

#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

#include <map>
#include <string>
#include <vector>
#include "ResourceManager.h"
#include "TextureSequence.h"
#include "Vec3.h"
#include <TriList.h>

namespace Amju
{
class File;
  
class Font : public Resource
{
public:
  Font(const std::string& name);
  ~Font();

  // Load font data from a file. 
  // I.e. point size, texture file names, etc.
  virtual bool Load(File*);
   
  // Get "point size" of font (it's a relative size rather than an
  //  absolute point size) 
  float GetSize() const;

  // Change the size 
  void SetSize(float size);

  // Get width of one character (unicode code point)
  virtual float GetCharacterWidth(int c);

  // Get width of a string
  float GetTextWidth(const std::string& stringUtf8);

  // Use this to make a TriList when text changes.
  // Use this in preference to Print*()
  TriList* MakeTriList(float x, float y, const char* textUtf8, float scaleX);
  
  void BindTexture();
  
  // Draw the text on the window. 
  // (x, y) coords are -1..1
  // TODO Why char*, why not string
  void Print(float x, float y, const char* textUtf8, float scaleX = 1.0f);

  // Draw text in world space as a series of billboards.
  // The billboards can always be standing up, or facing the camera.
  void PrintWorld(const Vec3f& v, 
    float size, const char* text, bool up = false, bool depthTest = false);

protected:
  std::string m_name;
  RCPtr<TextureSequence> m_textureSequence;
  float m_size;
  // Char widths: OK to be a vector for fixed-size-grid font.
  // Int index so allows >255 characters.
  std::vector<float> m_charWidths;
  // The first character in the texture sequence - usually 0 for a 256-
  //  character font, or 32 if the font starts at space.
  int m_startChar;
};

typedef RCPtr<Font> PFont;

// Convenience function for word-wrapping.
// Splits text into lines so that no line exceeds width when drawn with 
// the given font.
std::vector<std::string> WrapFontText(Font*, const char*, float width);

Resource* FontLoader(const std::string& fontName);

}

#endif

