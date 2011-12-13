#ifndef USEFUL_H_INCLUDED
#define USEFUL_H_INCLUDED

#include <Vec3.h>
#include <iostream>
#include <string>

namespace Amju
{
std::ostream& operator<<(std::ostream& os, const Vec3f& v);

std::string ToString(const Vec3f& v);
}

#endif

