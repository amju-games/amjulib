/*
Amju Games source code (c) Copyright Jason Colman 2005
*/

#include "AmjuFirst.h"
#include <algorithm>
#include <iostream>
#include "Font.h"
#include "File.h"
#include "StringUtils.h"
#include "AmjuAssert.h"
#include "AmjuFinal.h"

namespace Amju
{
static const float CHAR_SIZE = 0.2f;
static const float HORIZONTAL_SPACING = CHAR_SIZE; // ??

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

    m_charWidths[(char)code] = width;
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

void Font::Print(float x, float y, const char* text)
{
  AMJU_CALL_STACK;

  if (!text)
  {
      return;
  }

  AmjuGL::PushAttrib(AmjuGL::AMJU_LIGHTING | AmjuGL::AMJU_DEPTH_TEST);

  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_TEST);
  AmjuGL::Enable(AmjuGL::AMJU_BLEND); 

  AmjuGL::PushMatrix();
  AmjuGL::Translate(x, y - CHAR_SIZE, 0);
  // ..so top of character is at y-coord, not the bottom

  m_textureSequence.Bind();

  int i = 0;
  while (unsigned char c = text[i])
  {
      i++;

      m_textureSequence.Draw(c - (char)m_startChar, m_size);

      float f = GetCharacterWidth(c);
      AmjuGL::Translate(f, 0.0f, 0.0f);
  }

  AmjuGL::PopMatrix();
  AmjuGL::PopAttrib();
}

void Font::PrintWorld(
  const Vec3f& v, 
  float size,
  const char* text, 
  bool up,
  bool depthTest)
{
  AMJU_CALL_STACK;

  if (!text)
  {
    return;
  }

  AmjuGL::PushAttrib(AmjuGL::AMJU_LIGHTING | AmjuGL::AMJU_DEPTH_TEST | AmjuGL::AMJU_BLEND);

  // TODO ?? glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  AmjuGL::Enable(AmjuGL::AMJU_BLEND);

  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  if (depthTest)
  {
    AmjuGL::Enable(AmjuGL::AMJU_DEPTH_TEST);
  }
  else
  { 
    AmjuGL::Disable(AmjuGL::AMJU_DEPTH_TEST);
  }

  AmjuGL::PushMatrix();
  AmjuGL::Translate(v.x, v.y, v.z);

  m_textureSequence.Bind();
//  AmjuGL::Disable(GL_TEXTURE_GEN_S);
//  AmjuGL::Disable(GL_TEXTURE_GEN_T);

  int i = 0;
  while (unsigned char c = text[i])
  {
    i++;

    m_textureSequence.DrawBillboard(c - (char)m_startChar, size, up);

    float f = GetCharacterWidth(c);
    f *= size * 8.0f; // TODO TEMP TEST
    // ??? TODO
    AmjuGL::Translate(f, 0.0f, 0.0f);
  }

  AmjuGL::PopMatrix();
  AmjuGL::PopAttrib();
}

/*
bool FontManager::Init()
{
  AMJU_CALL_STACK;

  // Get font info filename. This file contains info for all fonts.
  std::string fontInfo = "fonts.txt"; //Engine::Instance()->GetConfigValue("fonts");
  File f;
  if (!f.OpenRead(fontInfo))
  {
    return false;
  }
  int numFonts = 0;
  if (!f.GetInteger(&numFonts))
  {
    f.ReportError("Expected number of fonts.");
    return false;
  }
  SharedPtr<Font> pDefault;
  for (int i = 0; i < numFonts; i++)
  {
    std::string fontName;
    if (!f.GetDataLine(&fontName))
    {
      f.ReportError("Expected font name.");
      return false;
    }

    SharedPtr<Font> pFont = new Font(fontName);
    if (!pFont->Load(&f))
    {
      f.ReportError("Failed to load font info.");
      return false;
    }
    
    m_fonts[fontName] = pFont;

    if (i == 0)
    {
      pDefault = m_fonts.begin()->second;
    }
  }
  if (m_fonts.empty())
  {
std::cout << "FONT MANAGER: no fonts found!\n";
    return false;
  }

  return true;
}

Font* FontManager::GetFont(const std::string& fontName)
{
  AMJU_CALL_STACK;

  return m_fonts[fontName].GetPtr();
}
*/

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


