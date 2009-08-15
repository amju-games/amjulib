#ifndef AMJU_COLOUR_H
#define AMJU_COLOUR_H

namespace Amju
{
struct Colour
{
  Colour();
  Colour(float r, float g, float b, float a);
  void Set(float r, float g, float b, float a);
  Colour& operator*=(const Colour& c);
  void Use();

  float m_r, m_g, m_b, m_a;
};

Colour operator*(const Colour& c1, const Colour& c2);

// Colour stack
void PushColour();
void PopColour();
void MultColour(const Colour& c);
}

#endif
