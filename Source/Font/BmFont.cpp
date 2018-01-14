// Amjulib - cross platform game engine
// (c) Copyright Jason Colman 2000-2017

#include <File.h>
#include <ReportError.h>
#include <StringUtils.h>
#include "BmFont.h"

namespace Amju
{
// Load BMFont description from file
bool BmFontTextureSequence::LoadBmFont(const std::string& filename)
{
  File f(false); // false => don't expect version info
  if (!f.OpenRead(filename))
  {
    f.ReportError("Couldn't open BM font file");
    return false;
  }

  return Load(&f);
}

namespace
{
int ParseCharCountLine(const Strings& strs)
{
  // Found count line
  Strings strs2 = Split(strs[1], '=');
  if (strs2.size() == 2)
  {
    return ToInt(strs2[1]);
  }
  else
  {
    Assert(0);
    return 0;
  }
}

// Parse a 'char' line in a BMFont description file, populating a BmChar.
bool ParseCharLine(
  File* f, 
  const Strings& strs, 
  BmFontTextureSequence::BmChar& bmc, 
  int& element,
  float texW, float texH)
{
  int numPairs = strs.size();
  for (int j = 1; j < numPairs; j++)
  {
    // Each element in strs should be in form <a>=<b>. Split into key and value.
    Strings strs2 = Split(strs[j], '=');
    if (strs2.empty())
    {
      f->ReportError("Unexpected format for line");
      return false;
    }
    if (strs2.size() > 2)
    {
      f->ReportError("Unexpected format for " + strs2[0]);
      return false;
    }
    if (strs2[0] == "id")
    {
      element = ToInt(strs2[1]);
    }
    else if (strs2[0] == "x") // U coord
    {
      bmc.u0 = ToFloat(strs2[1]) / texW;
    }
    else if (strs2[0] == "y") // V coord
    {
      bmc.v0 = ToFloat(strs2[1]) / texH;
    }
    else if (strs2[0] == "width")
    {
      float w = ToFloat(strs2[1]);
      bmc.w = w;
      bmc.u1 = bmc.u0 + w / texW;
    }
    else if (strs2[0] == "height")
    {
      float h = ToFloat(strs2[1]);
      bmc.h = h;
      bmc.v1 = bmc.v0 + h / texH;
    }
    else if (strs2[0] == "xoffset")
    {
      bmc.x = ToFloat(strs2[1]);
    }
    else if (strs2[0] == "yoffset")
    {
      bmc.y = ToFloat(strs2[1]);
    }
    else if (strs2[0] == "xadvance")
    {
      bmc.xadvance = ToFloat(strs2[1]);
    }
  }
  
  if (element < 0)
  {
    f->ReportError("No 'id' or bad value?");
    return false;
  }
  return true;
}
} // anon namespace

bool BmFontTextureSequence::Load(File* f)
{
  int numChars = 0;
  std::string line;
  while (f->GetDataLine(&line))
  {
    Strings strs = Split(line, ' ');
    if (!strs.empty() && strs[0] == "chars")
    {
      numChars = ParseCharCountLine(strs);
      if (numChars == 0)
      {
        ReportError("Unexpected format, expected 'chars count=<n>");
        return false;
      }
      break;
    }
  }

  return LoadChars(f, numChars);
}

bool BmFontTextureSequence::LoadChars(File* f, int numChars)
{
  Assert(m_pTexture);
  float texW = static_cast<float>(m_pTexture->GetWidth());
  float texH = static_cast<float>(m_pTexture->GetHeight());

  for (int i = 0; i < numChars; i++)
  {
    std::string line;
    if (!f->GetDataLine(&line))
    {
      f->ReportError("Too few 'char...' lines");
      return false;
    }
    Strings strs = Split(line, ' ');
    if (strs.empty())
    {
      continue;
    }
    if (strs[0] != "char")
    {
      f->ReportError("Unexpected format for 'char...' line");
      return false;
    }
    int element = -1;
    BmChar bmc;
    if (!ParseCharLine(f, strs, bmc, element, texW, texH))
    {
      return false;
    }
    m_elements[element] = bmc;
  }

  return true;
}

void BmFontTextureSequence::GetElementInAtlas(
  int element, // the element index, e.g. a character code, from ' ' to 'z' etc. Can be >255 for unicode
  float& x, float& y, // position offset (would be zero if all elements are equal size)
  float& w, float& h, // size of glyph in relation to entire atlas
  float& u0, float& v0, // top left UV coord
  float& u1, float& v1 // bottom right UV coord
)
{
  auto it = m_elements.find(element);
  if (it == m_elements.end())
  {
    ReportError("Bad BMFont element: " + ToString(element));
    Assert(0);
    return;
  }
  const BmChar& bmc = it->second;

  // These consts are to get the BMFont to look the same as a grid-style font.

  const float SCALE = 0.004f; // Hopefully not related to font size, but might be

  const float GLYPH_FATNESS = 1.5f; // not related to particular font or size, right?

  // This is to compensate for right-shift in bitmap fonts, TODO remove and fix
  //  visual bugs introduced as a result, e.g. note stems not lining up
  const float X_OFFSET = 0.07f; 

  // Hopefully not related to font size, but could be. I think it's because we change
  //  where the base line is, from top to bottom of glyph, or something like that.
  const float Y_OFFSET = 1.005f;

  w = bmc.w * SCALE * GLYPH_FATNESS;
  h = bmc.h * SCALE;
  x = bmc.x * SCALE + X_OFFSET;
  y = Y_OFFSET -h - bmc.y * SCALE;
  u0 = bmc.u0;
  v0 = bmc.v0;
  u1 = bmc.u1;
  v1 = bmc.v1;
}

}
