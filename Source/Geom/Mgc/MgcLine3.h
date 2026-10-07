// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#ifndef MGCLINE3_H

#define MGCLINE3_H



#include "MgcVector3.h"



namespace Mgc {





class MAGICFM Line3

{

public:

    // Line is L(t) = P+t*D for any real-valued t.  D is not necessarily

    // unit length.

    Line3 ();



    Vector3& Origin ();

    const Vector3& Origin () const;



    Vector3& Direction ();

    const Vector3& Direction () const;



protected:

    Vector3 m_kOrigin;  // P

    Vector3 m_kDirection;  // D

};



#include "MgcLine3.inl"



} // namespace Mgc



#endif



