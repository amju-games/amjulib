#include <AmjuFirst.h>
#include <StringUtils.h>
#include "Colour.h"
#include "AmjuGL.h"
#include "AmjuAssert.h"
#include <iostream> // TEST
#include <stdio.h>
#include <AmjuFinal.h>

namespace Amju
{
Colour::Colour() : m_r(1.0f), m_g(1.0f), m_b(1.0f), m_a(1.0f)
{
}

Colour::Colour(float r, float g, float b, float a) :
  m_r(r), m_g(g), m_b(b), m_a(a)
{ 
}

void Colour::Set(float r, float g, float b, float a)
{
  m_r = r;
  m_g = g;
  m_b = b;
  m_a = a;
}

Colour& Colour::operator*=(const Colour& c)
{
  m_r *= c.m_r;
  m_g *= c.m_g;
  m_b *= c.m_b;
  m_a *= c.m_a;
  return *this;
}

void Colour::Use()
{
  AmjuGL::SetColour(m_r, m_g, m_b, m_a);
}

Colour operator*(const Colour& c1, const Colour& c2)
{
  Colour c = c1;
  c *= c2;
  return c;
}

static const int MAX_COLOUR_STACK = 64;
static Colour s_colourStack[MAX_COLOUR_STACK];
static int s_current = 0;
static Colour s_colour;

void PushColour()
{
  Assert(s_current < MAX_COLOUR_STACK);
  s_colourStack[s_current] = s_colour;
  ++s_current;
}

void PopColour()
{
  Assert(s_current > 0);
  --s_current;
  s_colour = s_colourStack[s_current];
  s_colour.Use();
}

void MultColour(const Colour& c)
{
  s_colour *= c;
  s_colour.Use();
}

Colour FromHexString(const std::string& s) 
{
  int r[4] = { 0xff, 0xff, 0xff, 0xff };
  int size = s.size();
  Assert(size == 6 || size == 8); 
  for (int i = size - 2, j = size / 2 - 1; i >= 0; i -= 2, j--)
  {
#ifdef WIN32
    sscanf_s(s.substr(i, 2).c_str(), "%X", &r[j]);
#else
    sscanf(s.substr(i, 2).c_str(), "%X", &r[j]);
#endif
  } 
  return Colour((float)r[0]/255.0f, (float)r[1]/255.0f, (float)r[2]/255.0f, (float)r[3]/255.0f);
}

std::string ToHexString(const Colour& col, bool includeAlpha)
{
  // e.g. red (1, 0, 0) => ff0000
  // with alpha: (1, 0, 0, 1) => ff0000ff

  unsigned int i = (((int)col.m_r) * 255);
  Assert(i <= 255);
  i = (i << 8) | (((int)col.m_g) * 255);
  Assert(i <= 65535);
  i = (i << 8) | (((int)col.m_b) * 255);
  Assert(i <= 16777215);

  if (includeAlpha)
  {
    i = (i << 8) | (((int)col.m_a) * 255);
  }  

  return ToHexString(i);
}

}
