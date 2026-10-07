// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#ifndef MGCDIST3DVECTRI_H

#define MGCDIST3DVECTRI_H



#include "MgcTriangle3.h"



namespace Mgc {



// squared distance measurements

MAGICFM Real SqrDistance (const Vector3& rkPoint, const Triangle3& rkTri,

    Real* pfSParam = NULL, Real* pfTParam = NULL);



// distance measurements

MAGICFM Real Distance (const Vector3& rkPoint, const Triangle3& rkTri,

    Real* pfSParam = NULL, Real* pfTParam = NULL);



} // namespace Mgc



#endif



