#ifndef USEFUL_H_INCLUDED
#define USEFUL_H_INCLUDED

#include <iostream>
#include <string>
#include <Vec3.h>
#include <GameObject.h>

namespace Amju
{
// Useful functions for output

// Vec3
std::ostream& operator<<(std::ostream& os, const Vec3f& v);
std::string ToString(const Vec3f& v);

// Game Object
std::ostream& operator<<(std::ostream& os, const GameObject& go);
std::string ToString(const GameObject& go);

}

#endif

