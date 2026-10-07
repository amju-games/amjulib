// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#ifndef MGCSPHERE_H

#define MGCSPHERE_H



#include "MgcVector3.h"



namespace Mgc {





class MAGICFM Sphere

{

public:

    Sphere ();



    Vector3& Center ();

    const Vector3& Center () const;



    Real& Radius ();

    const Real& Radius () const;



protected:

    Vector3 m_kCenter;

    Real m_fRadius;

};



#include "MgcSphere.inl"



} // namespace Mgc



#endif



