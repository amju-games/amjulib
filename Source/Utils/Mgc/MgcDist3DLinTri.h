// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#ifndef MGCDIST3DLINTRI_H

#define MGCDIST3DLINTRI_H



#include "MgcLine3.h"

#include "MgcRay3.h"

#include "MgcSegment3.h"

#include "MgcTriangle3.h"



namespace Mgc {



// squared distance measurements



MAGICFM Real SqrDistance (const Line3& rkLine, const Triangle3& rkTri,

    Real* pfLinP = NULL, Real* pfTriP0 = NULL, Real* pfTriP1 = NULL);



MAGICFM Real SqrDistance (const Ray3& rkRay, const Triangle3& rkTri,

    Real* pfRayP = NULL, Real* pfTriP0 = NULL, Real* pfTriP1 = NULL);



MAGICFM Real SqrDistance (const Segment3& rkSeg, const Triangle3& rkTri,

    Real* pfSegP = NULL, Real* pfTriP0 = NULL, Real* pfTriP1 = NULL);





// distance measurements



MAGICFM Real Distance (const Line3& rkLine, const Triangle3& rkTri,

    Real* pfLinP = NULL, Real* pfTriP0 = NULL, Real* pfTriP1 = NULL);



MAGICFM Real Distance (const Ray3& rkRay, const Triangle3& rkTri,

    Real* pfRayP = NULL, Real* pfTriP0 = NULL, Real* pfTriP1 = NULL);



MAGICFM Real Distance (const Segment3& rkSeg, const Triangle3& rkTri,

    Real* pfSegP = NULL, Real* pfTriP0 = NULL, Real* pfTriP1 = NULL);



} // namespace Mgc



#endif



