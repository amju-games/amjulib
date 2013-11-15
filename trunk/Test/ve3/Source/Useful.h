#ifndef USEFUL_H_INCLUDED
#define USEFUL_H_INCLUDED

#include <iostream>
#include <string>
#include <Vec3.h>
#include <Vec2.h>
#include <GameObject.h>

namespace Amju
{
// Useful functions for output, etc

// Vec2
std::string ToString(const Vec2f& v);
Vec2f ToVec2(const std::string& s);

// Vec3
std::ostream& operator<<(std::ostream& os, const Vec3f& v);
std::string ToString(const Vec3f& v);
Vec3f ToVec3(const std::string& s);

// Game Object
std::ostream& operator<<(std::ostream& os, const GameObject& go);
std::string ToString(const GameObject& go);

}

#endif

