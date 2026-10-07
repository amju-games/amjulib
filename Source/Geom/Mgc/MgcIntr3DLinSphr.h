// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#ifndef MGCINTR3DLINSPHR_H

#define MGCINTR3DLINSPHR_H



#include "MgcLine3.h"

#include "MgcRay3.h"

#include "MgcSegment3.h"

#include "MgcSphere.h"



namespace Mgc {



// return value is 'true' if and only if objects intersect



MAGICFM bool TestIntersection (const Segment3& rkSegment,

    const Sphere& rkSphere);

MAGICFM bool TestIntersection (const Ray3& rkRay, const Sphere& rkSphere);

MAGICFM bool TestIntersection (const Line3& rkLine, const Sphere& rkSphere);



MAGICFM bool FindIntersection (const Segment3& rkSegment,

    const Sphere& rkSphere, int& riQuantity, Vector3 akPoint[2]);

MAGICFM bool FindIntersection (const Ray3& rkRay, const Sphere& rkSphere,

    int& riQuantity, Vector3 akPoint[2]);

MAGICFM bool FindIntersection (const Line3& rkLine, const Sphere& rkSphere,

    int& riQuantity, Vector3 akPoint[2]);



} // namespace Mgc



#endif



