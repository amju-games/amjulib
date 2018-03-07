// Amjulib - cross platform game engine
// (c) Copyright Jason Colman 2000-2017

#pragma once

// * BmFont *
// Packed bitmap font, created using BMFont tool.

#include <unordered_map>
#include <StringUtils.h>
#include "Font.h"

namespace Amju
{
// * BmFontTextureSequence *
// For fonts packed sensibly into an atlas, rather than in a uniform grid (i.e. using BMFont).
class BmFontTextureSequence : public TextureSequence
{
public:
  // Load texture sequence from file
  bool Load(File*) override;

  // Load BMFont description from file
  bool LoadBmFont(const std::string& filename);

  float GetCharWidth(int element) const;

  struct BmChar
  {
    enum class Channel
    {
      BLUE = 1,
      GREEN = 2,
      RED = 4,
      ALPHA = 8,
      ALL = 15
    };

    float u0 = 0;
    float v0 = 0;
    float u1 = 0;
    float v1 = 0;
    float x = 0;
    float y = 0;
    float w = 0; // quad width
    float h = 0;
    float xadvance = 0; // char width, for making text strings
    int page = 0; // which texture: not used for now
    Channel channel = Channel::ALPHA;

    BmChar() = default;
  };

protected:
  void GetElementInAtlas(
    int ch, // the element index, e.g. a character code, from ' ' to 'z' etc. Can be >255 for unicode
    float& x, float& y, // position offset (would be zero if all elements are equal size)
    float& w, float& h, // size of glyph in relation to entire atlas
    float& u0, float& v0, // top left UV coord
    float& u1, float& v1 // bottom right UV coord
  ) override;

  bool LoadChars(File* f, int numChars);

  // Parse 'page' line, which has texture filename. Path is the path
  //  from file root to where the texture file lives.
  bool ParsePageLine(const Strings& strs, const std::string& path);

  bool ParseInfoLine(const Strings& strs);

private:
  // We expect a sparse array for, e.g. Chinese characters.
  std::unordered_map<int, BmChar> m_elements;
};

// * BmFont *
// Display text using a packed font atlas as created using BM Font tool.
class BmFont : public Font
{
public:
  BmFont(const std::string& name);

  bool Load(const std::string& filename);
  bool Load(File*) override;
  float GetCharacterWidth(int c) override;
};
}

