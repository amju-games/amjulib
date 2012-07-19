/*
Amju Games source code (c) Copyright Jason Colman 2000-2008
*/

// NB Bad idea to use #include "AmjuFirst.h", #include "AmjuFinal.h", or
//  AMJU_CALL_STACK here.

#ifdef WIN32
#pragma warning(disable: 4786)
#endif // WIN32

#include "AmjuMem.h"

#if defined(USE_AMJU_DEBUG_NEW) && defined(MACOSX)

#ifdef new
#undef new
#endif

#ifdef WIN32
#include <malloc.h>
#endif // WIN32

// #include <cstddef> ??

#include <map>
#include <fstream>
#include <assert.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "AmjuFinal.h"

#ifdef new
#undef new
#endif

// Overload new and delete.
// TODO Allocate from a fixed size pool


// Can't put this in namespace Amju, it's never called!
//namespace Amju


// Store all allocations, so we can report on leaked memory, show biggest allocs, etc.
struct Allocation
{
  Allocation(
    size_t size, 
    void* p, 
    unsigned int id, 
    const char* file, 
    int line,
    const Amju::CallStack::FunctionNames& callstack) : 
    m_size(size), m_p(p), m_id(id), m_file(file), m_line(line), m_callstack(callstack) {}

  // size of allocated block
  size_t m_size; 

  // pointer to block
  void* m_p; 

  // unique ID for this allocation
  unsigned int m_id; 

  // File/line number of allocation
  std::string m_file;
  int m_line;

  // Call stack
  Amju::CallStack::FunctionNames m_callstack;

  // TODO name of type allocated, etc

  bool operator<(const Allocation& rhs) const
  {
    if (m_size < rhs.m_size)
    {
      return false; // Order by descending size
    }
    return true;
  }
};

// Set when we are ready to start recording allocations - which is immediately
//  -- set to false when the AllocMgr has been destroyed - after this we can't
//  log any more allocations.
static bool ready = true;

void DestroyAllocMgr();

// We need to stop trying to manage allocations once this object goes away.
typedef std::map<void*, Allocation> AllocMap;
struct AllocMgr
{
  static AllocMgr* Instance()
  {
    if (!s_instance)
    {
      static int count = 0;
      ++count;
#ifndef WIN32
      std::cout << "** Creating AllocMgr, number of times created: " << count << "\n";
#endif
      s_instance = new AllocMgr;

      // Register with atexit so we get destroyed, hopefully after everything else
      atexit(&DestroyAllocMgr);

      ready = true;
    }
    return s_instance;
  }

  ~AllocMgr()
  {
    // TODO only report if flag set
    ReportMem();

    // When this object is destroyed we will no longer be able to
    // use the alloc map. 
    ready = false;
  }

  AllocMap allocMap;

private:
  AllocMgr() {}
public:
  static AllocMgr* s_instance;
};
  
AllocMgr* AllocMgr::s_instance = 0;

// Definition
void DestroyAllocMgr()
{
  delete AllocMgr::s_instance;
  AllocMgr::s_instance = 0;
}

static unsigned int currentAlloc = 0;

// If non-zero, break when this allocation is reached.
static unsigned int breakId = 0; 

// If non-zero, break on allocations of this size
static size_t breakSize = 0;

// If non-zero, break if the total of currently allocated memory is greater
//  than this.
static unsigned int limit = 0;

// Recursion depth count:
// Using a map in operator new can cause new to be called again 
//  -- only add to the map if the recursion depth is zero!
static unsigned int depth = 0;

void IncRecursionDepth()
{
  ++depth;
}

void DecRecursionDepth()
{
  --depth;
}

void SetBreakSize(size_t size)
{
  breakSize = size;
}

void SetBreakID(unsigned int id)
{
  if (currentAlloc > id)
  {
    std::cout << "*** Setting break ID to " << id << " but current alloc is " << currentAlloc << "\n";
  }
  breakId = id;
}

void SetLimit(unsigned int lim)
{
  limit = lim;
}

static int totalMemAllocated = 0;
static int maxMemAllocated = 0;

struct Breaker
{
void Break()
{
  // Set gdb break point here - assert kills process
  std::cout << "Break!\n";
  assert(0);
}
};

Breaker myBreaker;

void* myNew(size_t size, const char* file, int line)
{
  // We have to allow allocations of size 0.
  // To make sure this works OK with malloc, change size to 1 in this case.
  // TODO Also print warning ?
  if (size == 0)
  {
    size = 1;
  }

  void* p = malloc(size);

  if (ready && depth == 0)
  {
    ++depth;

    currentAlloc++;

    if (breakId == currentAlloc)
    {
      // Break - use assert by default, or TODO allow another function to be set
      std::cout << "Mem: debug break on ID: " << breakId << "\n";
      myBreaker.Break(); 
    }

    if (breakSize && breakSize == size)
    {
      // Break on allocation of this size
      // TODO Allow another function to be called now
      std::cout << "Mem: debug break on size: " << size << "\n";
      myBreaker.Break(); 
    }

    totalMemAllocated += size;
    if (totalMemAllocated > maxMemAllocated)
    {
      maxMemAllocated = totalMemAllocated;
    }

    // Check that total is under a limit if set
    if (limit && totalMemAllocated > limit)
    {
      // Break - TODO allow another function ?
      std::cout << "Mem: over limit! Limit: " << limit << " total now: " << totalMemAllocated << "\n";
      myBreaker.Break(); 
    }

    // Avoid [] so we don't need default ctor
    std::pair<void*, Allocation> kv = 
      std::make_pair(p, 
      Allocation(size, p, currentAlloc, file, line, 
      Amju::CallStack::Instance()->GetFunctionNames()));

    AllocMgr::Instance()->allocMap.insert(kv);

    --depth;
  }

  return p;
}

// Overload operator new giving the file/line number of allocation
void* operator new(size_t size, const char* file, int line)
{
  return myNew(size, file, line);
}

void* operator new[](size_t size, const char* file, int line)
{
  return myNew(size, file, line);
}

void* operator new(size_t size)
{
  return myNew(size, "", -1);
}

void* operator new[](size_t size)
{
  return myNew(size, "", -1);
}

void operator delete(void* p)
{
  if (p)
  {
    if (ready) // && depth == 0)
    {
      AllocMap::iterator it = AllocMgr::Instance()->allocMap.find(p);

      if (it != AllocMgr::Instance()->allocMap.end())
      {
        totalMemAllocated -= it->second.m_size;

        AllocMgr::Instance()->allocMap.erase(it);
      }
    }

    free(p);
  }
}

void operator delete[](void* p)
{
  operator delete(p);
}

void ReportMem()
{
  ++depth;

  std::cout << "Writing memory report...\n";

  // Sort outstanding allocations by size
  typedef std::vector<Allocation> AllocVec;
  AllocVec vec; 

  for (AllocMap::const_iterator jt = AllocMgr::Instance()->allocMap.begin();
       jt != AllocMgr::Instance()->allocMap.end(); 
       ++jt)
  {
    vec.push_back(jt->second);
  }

#ifndef WIN32
  // Seems to hang on windows
  std::sort(vec.begin(), vec.end());
#endif

#ifdef USE_FSTREAM
  std::ofstream myFile;
  myFile.open("memreport.txt");
  if (myFile.bad() || !myFile.is_open())
  {
    assert(0);
  }

  myFile << "Total memory outstanding: " << totalMemAllocated << "\n";
  myFile << "Highest total: " << maxMemAllocated << "\n";
  myFile << "Number of allocations: " 
         << AllocMgr::Instance()->allocMap.size() << "\n";
  
  myFile << "ID\tSize\tFile::Line\tCall Stack\n";

#else

  FILE* file = fopen("memreport.txt", "w");
  if (!file)
  {
    assert(0);
  }

  fprintf(file, "Total memory outstanding: %d\nHighest total: %d\nNumber of allocations: %d\nID\tSize\tFile::Line\tCall Stack\n", 
    totalMemAllocated, maxMemAllocated, AllocMgr::Instance()->allocMap.size());

#endif

  for (AllocVec::iterator it = vec.begin(); it != vec.end(); ++it)
  {
    const Allocation& a = *it;

#ifdef USE_FSTREAM
    myFile 
      << a.m_id << "\t" 
      << a.m_size << "\t";

      if (a.m_line > 0)
      {
        myFile
          << a.m_file 
          << ":"
          << a.m_line << "\t";
      }
      else
      {
        myFile << "<unknown file:line>\t";
      }

      // Write call stack
      for (int i = 0; i < a.m_callstack.size(); i++)
      {
        myFile << a.m_callstack[i].c_str() << " ";
      }
      
      myFile << "\n";
#else

    fprintf(file, "%d\t%d\t", a.m_id, a.m_size);

    if (a.m_line > 0)
    {
      fprintf(file, "%s:%d\t", a.m_file.c_str(), a.m_line);
    }
    else
    {
      fprintf(file, "<unknown file:line>\t");
    }

    // Write call stack
    for (int i = 0; i < a.m_callstack.size(); i++)
    {
      fprintf(file, "%s ",  a.m_callstack[i].c_str());
    }
    
    fprintf(file, "\n");

#endif

  }

  --depth;
}

#else

// Dummy functions
void SetBreakID(unsigned int ID) {}
#ifdef GEKKO
void SetBreakSize(std::size_t size) {}
#else
void SetBreakSize(size_t size) {}
#endif

#endif // USE_AMJU_DEBUG_NEW
