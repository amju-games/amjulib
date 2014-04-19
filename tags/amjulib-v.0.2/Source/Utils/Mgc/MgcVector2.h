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



#ifndef MGCVECTOR2_H

#define MGCVECTOR2_H



#include "MagicFMLibType.h"



namespace Mgc {





class MAGICFM Vector2

{

public:

    // construction

    Vector2 ();

    Vector2 (float fX, float fY);

    Vector2 (float afCoordinate[2]);

    Vector2 (const Vector2& rkVector);



    // coordinates

    float x, y;



    // access vector V as V[0] = V.x, V[1] = V.y

    //

    // WARNING.  These member functions rely on

    // (1) Vector2 not having virtual functions

    // (2) the data packed in a 2*sizeof(float) memory block

    float& operator[] (int i) const;

    operator float* ();



    // assignment and comparison

    Vector2& operator= (const Vector2& rkVector);

    bool operator== (const Vector2& rkVector) const;

    bool operator!= (const Vector2& rkVector) const;



    // arithmetic operations

    Vector2 operator+ (const Vector2& rkVector) const;

    Vector2 operator- (const Vector2& rkVector) const;

    Vector2 operator* (float fScalar) const;

    Vector2 operator/ (float fScalar) const;

    Vector2 operator- () const;

    MAGICFM friend Vector2 operator* (float fScalar, const Vector2& rkVector);



    // arithmetic updates

    Vector2& operator+= (const Vector2& rkVector);

    Vector2& operator-= (const Vector2& rkVector);

    Vector2& operator*= (float fScalar);

    Vector2& operator/= (float fScalar);



    // vector operations

    float Length () const;

    float SquaredLength () const;

    float Dot (const Vector2& rkVector) const;

    float Unitize (float fTolerance = 1e-06f);

    Vector2 Cross () const;  // returns (y,-x)

    Vector2 UnitCross () const;  // returns (y,-x)/sqrt(x*x+y*y)



    // Gram-Schmidt orthonormalization.

    static void Orthonormalize (Vector2 akVector[2]);



    // special points

    static const Vector2 ZERO;

    static const Vector2 UNIT_X;

    static const Vector2 UNIT_Y;

};



#include "MgcVector2.inl"



} // namespace Mgc



#endif

