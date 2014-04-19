/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FileSize.cpp,v $
Revision 1.3  2008/05/08 10:58:04  jay
New memory management code

Revision 1.2  2006/09/15 08:37:40  jay
Fix comment

Revision 1.1  2004/09/08 15:43:18  jay
Added to repository
  
*/

#include <AmjuFirst.h>
#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include "FileSize.h"
#include "File.h"
#include <AmjuFinal.h>

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

  unsigned int s = buf.st_size;

  return s;
}
}


