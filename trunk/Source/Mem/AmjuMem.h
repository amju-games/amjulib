/*
Amju Games source code (c) Copyright Jason Colman 2000-2008
*/

#ifndef AMJU_MEM_H_INCLUDED
#define AMJU_MEM_H_INCLUDED

#include <new>

#if defined(USE_AMJU_DEBUG_NEW) //&& defined(MACOSX)

// Call to generate report of outstanding memory allocations
void ReportMem();

// Call to cause a break when the specified allocation is to be executed
void SetBreakID(unsigned int ID); 

// Set a break when an allocation of the specified size is about to be made
void SetBreakSize(size_t size);

// Set max size of total allocations
void SetLimit(unsigned int limit);


// Overload operator new so you can give the file/line number of allocation
void* operator new(size_t size, const char* file, int line);

// Array new
void* operator new[](size_t size, const char* file, int line);

#ifdef WIN32
// MSVC: warns that there is no matching operator delete.. ?
void operator delete(void*, const char* file, int line);
void operator delete[](void*, const char* file, int line);
#endif


// Call the overloaded operator new giving file and line number
#define new new(__FILE__, __LINE__)


// Used to turn off allocation logging for call stack or other 
//  debug-only use of heap. Allocations are only logged if recursion depth 
//  is zero.
void IncRecursionDepth();
void DecRecursionDepth();

struct RecursionDepthIncrementer
{
  RecursionDepthIncrementer()
  {
    IncRecursionDepth();
  }

  ~RecursionDepthIncrementer()
  {
    DecRecursionDepth();
  }
};

#else

// Dummy versions of functions so we don't have to change anything else
void SetBreakID(unsigned int ID);
#ifdef GEKKO
// gcc says size_t no good now, use std::size_t..!?
void SetBreakSize(std::size_t size);
#else
void SetBreakSize(size_t size);
#endif

#endif //  USE_AMJU_DEBUG_NEW
#endif //  AMJU_MEM_H_INCLUDED







