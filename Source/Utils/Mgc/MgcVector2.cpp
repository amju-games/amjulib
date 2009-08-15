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
#include <cfloat>

#include <cmath>

#include "MgcVector2.h"
#include "AmjuFinal.h"

using namespace Mgc;



const Vector2 Vector2::ZERO(0.0f,0.0f);

const Vector2 Vector2::UNIT_X(1.0f,0.0f);

const Vector2 Vector2::UNIT_Y(0.0f,1.0f);



//----------------------------------------------------------------------------

Vector2::Vector2 (float fX, float fY)

{

    x = fX;

    y = fY;

}

//----------------------------------------------------------------------------

Vector2::Vector2 (float afCoordinate[2])

{

    x = afCoordinate[0];

    y = afCoordinate[1];

}

//----------------------------------------------------------------------------

Vector2::Vector2 (const Vector2& rkVector)

{

    x = rkVector.x;

    y = rkVector.y;

}

//----------------------------------------------------------------------------

Vector2& Vector2::operator= (const Vector2& rkVector)

{
  AMJU_CALL_STACK;


    x = rkVector.x;

    y = rkVector.y;

    return *this;

}

//----------------------------------------------------------------------------

bool Vector2::operator== (const Vector2& rkVector) const

{
  AMJU_CALL_STACK;


    return ( x == rkVector.x && y == rkVector.y );

}

//----------------------------------------------------------------------------

bool Vector2::operator!= (const Vector2& rkVector) const

{
  AMJU_CALL_STACK;


    return ( x != rkVector.x || y != rkVector.y );

}

//----------------------------------------------------------------------------

Vector2 Vector2::operator+ (const Vector2& rkVector) const

{
  AMJU_CALL_STACK;


    return Vector2(x+rkVector.x,y+rkVector.y);

}

//----------------------------------------------------------------------------

Vector2 Vector2::operator- (const Vector2& rkVector) const

{
  AMJU_CALL_STACK;


    return Vector2(x-rkVector.x,y-rkVector.y);

}

//----------------------------------------------------------------------------

Vector2 Vector2::operator* (float fScalar) const

{
  AMJU_CALL_STACK;


    return Vector2(fScalar*x,fScalar*y);

}

//----------------------------------------------------------------------------

Vector2 Vector2::operator- () const

{
  AMJU_CALL_STACK;


    return Vector2(-x,-y);

}

//----------------------------------------------------------------------------

Vector2 Mgc::operator* (float fScalar, const Vector2& rkVector)

{
  AMJU_CALL_STACK;


    return Vector2(fScalar*rkVector.x,fScalar*rkVector.y);

}

//----------------------------------------------------------------------------

Vector2 Vector2::operator/ (float fScalar) const

{
  AMJU_CALL_STACK;


    Vector2 kQuot;



    if ( fScalar != 0.0f )

    {

        float fInvScalar = 1.0f/fScalar;

        kQuot.x = fInvScalar*x;

        kQuot.y = fInvScalar*y;

        return kQuot;

    }

    else

    {

        return Vector2(FLT_MAX,FLT_MAX);

    }

}

//----------------------------------------------------------------------------

Vector2& Vector2::operator+= (const Vector2& rkVector)

{
  AMJU_CALL_STACK;


    x += rkVector.x;

    y += rkVector.y;

    return *this;

}

//----------------------------------------------------------------------------

Vector2& Vector2::operator-= (const Vector2& rkVector)

{
  AMJU_CALL_STACK;


    x -= rkVector.x;

    y -= rkVector.y;

    return *this;

}

//----------------------------------------------------------------------------

Vector2& Vector2::operator*= (float fScalar)

{
  AMJU_CALL_STACK;


    x *= fScalar;

    y *= fScalar;

    return *this;

}

//----------------------------------------------------------------------------

Vector2& Vector2::operator/= (float fScalar)

{
  AMJU_CALL_STACK;


    if ( fScalar != 0.0f )

    {

        float fInvScalar = 1.0f/fScalar;

        x *= fInvScalar;

        y *= fInvScalar;

    }

    else

    {

        x = FLT_MAX;

        y = FLT_MAX;

    }



    return *this;

}

//----------------------------------------------------------------------------

float Vector2::Dot (const Vector2& rkVector) const

{

    return x*rkVector.x + y*rkVector.y;

}

//----------------------------------------------------------------------------

float Vector2::Length () const

{

    return (float)sqrt(x*x +y*y); // j.c. not sqrtf()

}

//----------------------------------------------------------------------------

Vector2 Vector2::Cross () const

{

    return Vector2(y,-x);

}

//----------------------------------------------------------------------------

Vector2 Vector2::UnitCross () const

{

    Vector2 kCross(y,-x);

    kCross.Unitize();

    return kCross;

}

//----------------------------------------------------------------------------

float Vector2::Unitize (float fTolerance)

{

    float fLength = Length();



    if ( fLength > fTolerance )

    {

        float fInvLength = 1.0f/fLength;

        x *= fInvLength;

        y *= fInvLength;

    }

    else

    {

        fLength = 0.0f;

    }



    return fLength;

}

//----------------------------------------------------------------------------

void Vector2::Orthonormalize (Vector2 akVector[2])

{

    // If the input vectors are v0 and v1, then the Gram-Schmidt

    // orthonormalization produces vectors u0 and u1 as follows,

    //

    //   u0 = v0/|v0|

    //   u1 = (v1-(u0*v1)u0)/|v1-(u0*v1)u0|

    //

    // where |A| indicates length of vector A and A*B indicates dot

    // product of vectors A and B.



    // compute u0

    akVector[0].Unitize();



    // compute u1

    float fDot0 = akVector[0].Dot(akVector[1]); 

    akVector[1] -= fDot0*akVector[0];

    akVector[1].Unitize();

}

//----------------------------------------------------------------------------

