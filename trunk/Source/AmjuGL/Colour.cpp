#include "Colour.h"
#include "AmjuGL.h"
#include "AmjuAssert.h"

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
}
