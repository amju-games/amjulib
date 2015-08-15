#pragma once

namespace Amju
{
// Replacement for glFrustum.
// Constructs matrix from params and mults current AmjuGL matrix
void MultFrustum(float left, float right, float bottom, float top, float zNear, float zFar);
}

