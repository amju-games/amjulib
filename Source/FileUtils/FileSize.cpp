/*
Amju Games source code (c) Copyright Juliet Colman 2004
*/

#include "AmjuFirst.h"
#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include "FileSize.h"
#include "File.h"
#include "AmjuFinal.h"

namespace Amju
{
unsigned int FileSize(const char* filename)
{
  std::string fullpath = File::GetRoot() + filename;

  struct stat buf;
  if (stat(fullpath.c_str(), &buf) == -1)
  {
    // Error
    return 0;
  }

  auto s = buf.st_size;

  return s;
}
}


