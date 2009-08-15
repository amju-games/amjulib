// Magic Software, Inc.

// http://www.magic-software.com

// Copyright (c) 2000, 2001.  All Rights Reserved

//

// Source code from Magic Software is supplied under the terms of a license

// agreement and may not be copied or disclosed except in accordance with the

// terms of that agreement.  The various license agreements may be found at

// the Magic Software web site.  This file is subject to the license

//

// FREE SOURCE CODE

// http://www.magic-software.com/License/free.pdf



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

