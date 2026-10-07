// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#ifndef MGCINTR3DTRISPHR_H

#define MGCINTR3DTRISPHR_H



#include "MgcSphere.h"

#include "MgcTriangle3.h"



namespace Mgc {



// Determine if triangle transversely intersects sphere.  Return value is

// 'true' if and only if they intersect.  The Function does not indicate an

// intersection if one or more vertices are the only points of intersection

// or if the triangle is tangent to the sphere.



MAGICFM bool TestIntersection (const Triangle3& rkT, const Sphere& rkS);



} // namespace Mgc



#endif



