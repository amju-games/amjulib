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



#include "AmjuFirst.h"
#include "MgcCont2DPointInPolygon.h"
#include "AmjuFinal.h"

using namespace Mgc;



//----------------------------------------------------------------------------

bool Mgc::PointInPolygon (int iQuantity, const Vector2* akPoint,

    const Vector2& rkTest)

{

    bool bInside = false;

    for (int i = 0, j = iQuantity-1; i < iQuantity; j = i++)

    {

        if (

            (akPoint[i].y <= rkTest.y

        &&  rkTest.y < akPoint[j].y

        &&  (akPoint[j].y - akPoint[i].y)*(rkTest.x - akPoint[i].x) <

                (akPoint[j].x - akPoint[i].x)*(rkTest.y - akPoint[i].y))



        ||

            (akPoint[j].y <= rkTest.y

        &&  rkTest.y < akPoint[i].y

        &&  (akPoint[j].y - akPoint[i].y)*(rkTest.x - akPoint[i].x) >

                (akPoint[j].x - akPoint[i].x)*(rkTest.y - akPoint[i].y))

        )

        {

            bInside = !bInside;

        }

    }



    return bInside;

}

//----------------------------------------------------------------------------

