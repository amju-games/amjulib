// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#ifndef MGCDIST3DVECLIN_H

#define MGCDIST3DVECLIN_H



#include "MgcLine3.h"

#include "MgcRay3.h"

#include "MgcSegment3.h"



namespace Mgc {



// squared distance measurements



MAGICFM Real SqrDistance (const Vector3& rkPoint, const Line3& rkLine,

    Real* pfParam = NULL);



MAGICFM Real SqrDistance (const Vector3& rkPoint, const Ray3& rkRay,

    Real* pfParam = NULL);



MAGICFM Real SqrDistance (const Vector3& rkPoint, const Segment3& rkSegment,

    Real* pfParam = NULL);





// distance measurements



MAGICFM Real Distance (const Vector3& rkPoint, const Line3& rkLine,

    Real* pfParam = NULL);



MAGICFM Real Distance (const Vector3& rkPoint, const Ray3& rkRay,

    Real* pfParam = NULL);



MAGICFM Real Distance (const Vector3& rkPoint, const Segment3& rkSegment,

    Real* pfParam = NULL);



} // namespace Mgc



#endif



