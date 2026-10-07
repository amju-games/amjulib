// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026

#include "precomp.h" // first include
#include <AmjuFirst.h>
#include "MgcCont2DPointInPolygon.h"
#include <AmjuFinal.h>

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

