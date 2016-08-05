/*
Amju Games source code (c) Copyright Jason Colman 2005
*/

#include <AmjuFirst.h>
#include <algorithm>
#include <iostream>
#include "Font.h"
#include "File.h"
#include "StringUtils.h"
#include "AmjuAssert.h"
#include <AmjuFinal.h>

//#define FONT_DEBUG

namespace Amju
{
static const float CHAR_SIZE = 0.2f;
static const float HORIZONTAL_SPACING = CHAR_SIZE; // ??
static const int NUM_CHAR_WIDTHS = 256;
  
Resource* FontLoader(const std::string& fontName)
{
  // Font name is e.g. "cheri.font"
  // The ".font" is so the ResourceManager knows what loader to use - it doesn't
  //  have to be the file extension.
  // TODO Overload GetRes so you can specify the loader ?
  std::string fontFilename = GetFileNoExt(fontName) + ".txt";
  Font* font = new Font(StripPath(fontFilename));
  File file;
  if (!file.OpenRead(fontFilename))
  {
    Assert(0);
    return 0;
  }
  if (!font->Load(&file))
  {
    Assert(0);
    return 0;
  }
  return font;
}

Font::Font(const std::string& name) : m_name(name)
{
  AMJU_CALL_STACK;
  m_charWidths.resize(NUM_CHAR_WIDTHS);
}

Font::~Font()
{
  AMJU_CALL_STACK;
}

bool Font::Load(File* pf)
{
  AMJU_CALL_STACK;

  // Get the Point Size. This is not really a point size because it depends 
  // on the dimensions of the window!
  if (!pf->GetFloat(&m_size))
  {
    pf->ReportError("Expected font point size.");
    return false;
  }
  // Get the bitmap and alpha filenames
  std::string bm;
  if (!pf->GetDataLine(&bm))
  {
    pf->ReportError("Expected font bitmap name.");
    return false;
  }
  // Get no of characters in X and Y directions.
  int xchars = 0;
  int ychars = 0;
  if (!pf->GetInteger(&xchars))
  {
    pf->ReportError("Expected font x chars");
    return false;
  }
  if (!pf->GetInteger(&ychars))
  {
    pf->ReportError("Expected font y chars");
    return false;
  }

  bool b = m_textureSequence.Load(
    bm, xchars, ychars, CHAR_SIZE, CHAR_SIZE);
  if (!b)
  {
    pf->ReportError("Failed to load texture sequence for font");
    return false;
  }
  m_textureSequence.GetTexture()->SetFilter(AmjuGL::AMJU_TEXTURE_NICE);

  // Get start character - usually something like 0 or 32.
  if (!pf->GetInteger(&m_startChar))
  {
    pf->ReportError("Expected font start character");
    return false;
  }

  // Get filename for character widths
  std::string filename;
  if (!pf->GetDataLine(&filename)) 
  {
    pf->ReportError("Expected font widths filename");
    return false;
  }

  // load char widths
  File f(false); // false => don't expect version info
  if (!f.OpenRead(filename))
  {
    f.ReportError("Couldn't open font widths file");
    return false;
  }
  // Widths are in *.INI format:
  // [<section>]
  // <char>=<width>
  // <char>=<width>
  // ... etc.
  std::string line;
  f.GetDataLine(&line); // get [<section>]
  // Now get lines of format <char code>=<width>
  // Width is font bitmap width/num chars
  const float MAX_WIDTH = (float)m_textureSequence.GetTextureWidth() / 
    (float)xchars;

  // We expect 256 character widths.
  // NB We must not loop until the file ends - this will not work with
  // GLUE files.
  for (int i = 0; i < 256; i++)
  {
    f.GetDataLine(&line);

    std::string::size_type eq = line.find('=');
    std::string strCode = line.substr(0, eq);
    std::string strWidth = line.substr(eq + 1);
    int code = atoi(strCode.c_str());
    float width = (float)atoi(strWidth.c_str());
    width /= MAX_WIDTH;
    width *= HORIZONTAL_SPACING;

    m_charWidths[code] = width;
  }

#ifdef FONT_DEBUG
std::cout << "FONT: Successfully loaded " << m_name.c_str() << "\n";
#endif

  return true;
}

float Font::GetSize() const
{
  AMJU_CALL_STACK;

  return m_size;
}

void Font::SetSize(float s)
{
  AMJU_CALL_STACK;

  m_size = s;
}

float Font::GetCharacterWidth(char c)
{
  AMJU_CALL_STACK;

  float f = m_charWidths[c];
  f *= m_size;
  return f;
}

float Font::GetTextWidth(const std::string& s)
{
  AMJU_CALL_STACK;

  float f = 0;
  int chars = s.size();
  for (int i = 0; i < chars; i++)
  {
    f += GetCharacterWidth(s[i]);
  }
  return f;
}

void Font::BindTexture()
{
  m_textureSequence.Bind();
}

TriList* Font::MakeTriList(float x, float y, const char* text, float scaleX)
{
  AMJU_CALL_STACK;

#ifdef FONT_DEBUG
std::cout << "Font::MakeTriList: x: " << x << " y: " << y << " \"" << text << "\"\n";
#endif

  Assert(text);
  
  AmjuGL::Tris tris;
  
  if (*text == 0)
  {
    // Empty string
    return Amju::MakeTriList(tris);
  }
  
  float oldSizeX = m_textureSequence.GetSizeX();
  float sizeY = m_textureSequence.GetSizeY();
  
  m_textureSequence.SetSize(oldSizeX * scaleX, sizeY);  // TODO TEMP TEST
  
  float xOff = x;
  float yOff = y;
  int i = 0;
  while (unsigned char c = text[i++])
  {
    AmjuGL::Tri t[2];
    m_textureSequence.MakeTris(c - (char)m_startChar, m_size, t, xOff, yOff);
    tris.push_back(t[0]);
    tris.push_back(t[1]);
    xOff += GetCharacterWidth(c) * scaleX;
  }
  m_textureSequence.SetSize(oldSizeX, sizeY);
  return Amju::MakeTriList(tris);
}
  
struct FontWidthFinder
{
  FontWidthFinder(Font* pFont) : m_pFont(pFont) {}
  float operator() (const std::string& s) { return m_pFont->GetTextWidth(s); }
  Font* m_pFont;
};

std::vector<std::string> WrapFontText(Font* pFont, const char* text, float width)
{
  return WordWrap(text, width, FontWidthFinder(pFont));
}
}


