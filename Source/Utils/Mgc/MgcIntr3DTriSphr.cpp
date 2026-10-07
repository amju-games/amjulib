// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026

#include "precomp.h" // first include
#include <AmjuFirst.h>
#include "MgcDist3DVecTri.h"

#include "MgcIntr3DTriSphr.h"
#include <AmjuFinal.h>

using namespace Mgc;



//----------------------------------------------------------------------------

bool Mgc::TestIntersection (const Triangle3& rkT, const Sphere& rkS)

{

    Real fSqrDist = SqrDistance(rkS.Center(),rkT);

    Real fRSqr = rkS.Radius()*rkS.Radius();

    return fSqrDist < fRSqr;

}

//----------------------------------------------------------------------------



