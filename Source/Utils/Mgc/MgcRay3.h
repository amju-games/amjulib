// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#ifndef MGCRAY3_H

#define MGCRAY3_H



#include "MgcVector3.h"



namespace Mgc {





class MAGICFM Ray3

{

public:

    // Ray is R(t) = P+t*D for t >= 0.  D is not necessarily unit length.

    Ray3 ();



    Vector3& Origin ();

    const Vector3& Origin () const;



    Vector3& Direction ();

    const Vector3& Direction () const;



protected:

    Vector3 m_kOrigin;  // P

    Vector3 m_kDirection;  // D

};



#include "MgcRay3.inl"



} // namespace Mgc



#endif



