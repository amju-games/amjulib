// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#ifndef MGCCONT2DPOINTINPOLYGON_H

#define MGCCONT2DPOINTINPOLYGON_H



// Given a polygon as an order list of vertices (x[i],y[i]) for

// 0 <= i < N and a test point (xt,yt), return 'true' if (xt,yt) is in

// the polygon and 'false' if it is not.



#include "MagicFMLibType.h"

#include "MgcVector2.h"



namespace Mgc {



MAGICFM bool PointInPolygon (int iQuantity, const Vector2* akPoint,

    const Vector2& rkTest);



} // namespace Mgc



#endif

