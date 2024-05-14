#ifndef AMJU_COLOUR_H
#define AMJU_COLOUR_H

#include <optional>
#include <string>

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

bool operator==(const Colour& rhs, const Colour& lhs);
bool operator!=(const Colour& rhs, const Colour& lhs);

Colour operator*(const Colour& c1, const Colour& c2);

Colour Interpolate(const Colour& c1, const Colour& c2, float t);

// Colour stack
void PushColour();
void PopColour();
void MultColour(const Colour& c);

std::optional<Colour> FromHexString(const std::string&);

std::string ToHexString(const Colour& c, bool includeAlpha = false);
}

#endif
