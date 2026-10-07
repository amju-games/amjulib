// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#ifndef MGCRTLIB_H

#define MGCRTLIB_H



// A wrapper around some headers for run-time libraries.  I added this because

// CodeWarrior 6.1 for the Macintosh uses namespace std for functions exposed

// in cmath, cstring, etc.  Windows or Linux does not do this.  I do not know

// what the STL standard is for this.



#include <cassert>

#include <cctype>

#include <cfloat>

#include <cmath>

#include <cstddef>

#include <cstdio>

#include <cstdlib>

#include <cstring>



#ifdef __MACOS__

using namespace std;

#endif



#endif



