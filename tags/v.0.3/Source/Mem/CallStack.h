/*
Amju Games source code (c) Copyright Jason Colman 2000-2008
*/

#ifndef AMJU_CALL_STACK_H_INCLUDED
#define AMJU_CALL_STACK_H_INCLUDED

#include <vector>
#include <string>

namespace Amju
{
// Stack of function names. Used by overloaded operator new() to log where
//  allocations happen. Could also use in Assert.
class CallStack
{
public:
  static CallStack* Instance();

  void Push(const char* functionName);
  void Pop();

  typedef std::vector<std::string> FunctionNames;
  const FunctionNames& GetFunctionNames();

private:
  FunctionNames m_names;

private:
  CallStack() {}
  CallStack(const CallStack&);
};

// Create at the start of each function to push the current function name
//  onto the stack.
struct CallStackLogger
{
  CallStackLogger(const char* functionName);
  ~CallStackLogger();
};

// Handy macro - add to top of each function
// Could potentially have more stuff added, e.g. profiling
#ifdef YES_CALLSTACK

#ifdef MACOSX
#define AMJU_CALL_STACK Amju::CallStackLogger myCallStackLogger(__PRETTY_FUNCTION__);
#endif // MACOSX 
	
#ifdef IPHONE
#define AMJU_CALL_STACK
#endif
	
#ifdef WIN32
// MSVC6 does not have __FUNCTION__ :-(
#if defined(_MSC_VER) && _MSC_VER < 1300
#define __FUNCTION__ "???"
#endif
#define AMJU_CALL_STACK Amju::CallStackLogger myCallStackLogger(__FUNCTION__);
#endif // WIN32

#ifdef GEKKO
#define AMJU_CALL_STACK Amju::CallStackLogger myCallStackLogger(__PRETTY_FUNCTION__);
#endif // GEKKO

#else // YES_CALLSTACK
#define AMJU_CALL_STACK
#endif // YES_CALLSTACK
}

#endif

