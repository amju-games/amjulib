// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#ifndef MGCSEGMENT3_H

#define MGCSEGMENT3_H



#include "MgcVector3.h"



namespace Mgc {





class MAGICFM Segment3

{

public:

    // Segment is S(t) = P+t*D for 0 <= t <= 1.  D is not necessarily unit

    // length.  The end points are P and P+D.

    Segment3 ();



    Vector3& Origin ();

    const Vector3& Origin () const;



    Vector3& Direction ();

    const Vector3& Direction () const;



protected:

    Vector3 m_kOrigin;  // P

    Vector3 m_kDirection;  // D

};



#include "MgcSegment3.inl"



} // namespace Mgc



#endif



