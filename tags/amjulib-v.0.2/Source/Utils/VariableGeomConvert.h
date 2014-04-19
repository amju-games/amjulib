#pragma once

#include <Vec2.h>
#include <Vec3.h>
#include <Matrix.h>
#include "Variable.h"

namespace Amju
{
Vec2f ToVec2f(const Variable&);
Vec3f ToVec3f(const Variable&);
Matrix ToMatrix(const Variable&);

Variable ToVariable(const Vec2f&);
Variable ToVariable(const Vec3f&);
Variable ToVariable(const Matrix&);
}

