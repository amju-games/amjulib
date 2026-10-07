// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026

#pragma once

namespace Amju
{
class AABB;
struct Capsule;

bool Intersects(const AABB& aabb, const Capsule& cap);
}

