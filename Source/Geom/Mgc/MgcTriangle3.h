// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#ifndef MGCTRIANGLE3_H

#define MGCTRIANGLE3_H



#include "MgcVector3.h"



namespace Mgc {





class MAGICFM Triangle3

{

public:

    // Triangle points are tri(s,t) = b+s*e0+t*e1 where 0 <= s <= 1,

    // 0 <= t <= 1, and 0 <= s+t <= 1.



    Triangle3 ();



    Vector3& Origin ();

    const Vector3& Origin () const;



    Vector3& Edge0 ();

    const Vector3& Edge0 () const;



    Vector3& Edge1 ();

    const Vector3& Edge1 () const;



protected:

    Vector3 m_kOrigin;

    Vector3 m_kEdge0;

    Vector3 m_kEdge1;

};



#include "MgcTriangle3.inl"



} // namespace Mgc



#endif



