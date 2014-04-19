/*
Amju Games source code (c) Copyright Jason Colman 2000-2008
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "AmjuFirst.h"
#include "CallStack.h"
#include "AmjuFinal.h"

// NB Bad idea to use AMJU_CALL_STACK here - we will get infinite recursion.

// TODO TEMP TEST
#ifdef WIN32
#define DO_CALLSTACK 1
#endif // WIN32

#ifdef GEKKO
#define DO_CALLSTACK 1
#endif // GEKKO

#ifdef MACOSX
#ifdef USE_AMJU_DEBUG_NEW
#define DO_CALLSTACK 1
#endif // USE_AMJU_DEBUG_NEW
#endif // MACOSX

namespace Amju
{
CallStack* CallStack::Instance()
{
  static CallStack c;
  return &c;
}

void CallStack::Push(const char* functionName)
{
#ifdef DO_CALLSTACK
  // Inc. recursion depth so allocations are not logged
//  RecursionDepthIncrementer rdi;

  m_names.push_back(functionName);
#endif
}

void CallStack::Pop()
{
#ifdef DO_CALLSTACK
  // Inc. recursion depth so allocations are not logged
//  RecursionDepthIncrementer rdi;

  m_names.pop_back();
#endif
}

const CallStack::FunctionNames& CallStack::GetFunctionNames()
{
  return m_names;
}

CallStackLogger::CallStackLogger(const char* functionName)
{
#ifdef DO_CALLSTACK
  // Inc. recursion depth so allocations are not logged
//  RecursionDepthIncrementer rdi;

  CallStack::Instance()->Push(functionName);
#endif
}

CallStackLogger::~CallStackLogger()
{
#ifdef DO_CALLSTACK
  // Inc. recursion depth so allocations are not logged
//  RecursionDepthIncrementer rdi;

  CallStack::Instance()->Pop();
#endif
}
}


