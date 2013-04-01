/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FileImplStd.cpp,v $
Revision 1.12  2008/05/18 16:04:26  jay
MSVC update

Revision 1.11  2008/05/08 10:58:04  jay
New memory management code

Revision 1.10  2007/12/13 10:14:39  jay
TODO Mac get size of an open file

Revision 1.9  2007/11/28 23:21:09  jay
Add Assert, commented out for now though

Revision 1.8  2007/11/26 22:01:31  jay
Add implementation for GetBinaryFileSize

Revision 1.7  2007/02/10 14:55:13  jay
Return accurate number of bytes read

Revision 1.6  2006/12/30 16:21:27  jay
Debug output

Revision 1.5  2006/12/01 23:28:04  jay
New FileImplMem, unfortunately causing many small changes

Revision 1.4  2006/09/15 12:48:24  Administrator
Fixes to get PetZoo+SceneEditor to build on Windows/MSVC

Revision 1.3  2006/05/17 08:41:57  jay
Merge improvements from Pool: chmod +ugow; new tokenising function

Revision 1.1.10.2  2006/05/15 08:18:41  jay
Chmod files when opened for reading or writing so they can be written
to by everyone.

Revision 1.1.10.1  2006/01/20 13:45:13  jay
Gcc 4.0 fix: no need for conditional compile

Revision 1.1  2004/09/08 15:43:18  jay
Added to repository

*/

#include <AmjuFirst.h>
#ifdef WIN32
#pragma warning(disable: 4786)
#endif
#include <iostream>
#ifdef MACOSX
#include <sys/types.h>  // chmod
#include <sys/stat.h>   // chmod
#endif

#include "FileImplStd.h"
#include <AmjuFinal.h>

using namespace std;
namespace Amju
{
FileImplStd::FileImplStd()
{
}

FileImplStd::~FileImplStd()
{
  AMJU_CALL_STACK;

}

bool FileImplStd::OpenRead(const std::string& path, const std::string& filename, bool isBinary)
{
  AMJU_CALL_STACK;

  Assert(!m_file.is_open()); // already opened

  int mode = ios::in;
  if (isBinary)
  {
    mode |= ios::binary;
  }

#ifdef WIN32
  m_file.open((path + filename).c_str(), mode);
#else
  m_file.open((path + filename).c_str(), (std::_Ios_Openmode)mode);
#endif
  bool result = m_file.is_open();

#ifdef MACOSX
  if (result)
  {
    // We may be writing to a shared area so multiple users can access the
    // file.
    int CHMOD_MODE =
      S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IWOTH | S_IROTH;
    // Read + write access for all
    chmod((path + filename).c_str(), CHMOD_MODE); 
  }
#endif

  return result;
}

bool FileImplStd::OpenWrite(
  const std::string& path, 
  const std::string& filename, 
  bool isBinary, 
  bool truncate)
{
  AMJU_CALL_STACK;

  Assert(!m_file.is_open()); // already opened

  int mode = ios::out;
  if (truncate)
  {
    mode |= ios::trunc; 
  }
  else
  {
    mode |= ios::app;
  }

  if (isBinary)
  {
    mode |= ios::binary;
  }

#ifdef OPEN_WRITE_DEBUG
  std::cout << "opening " << std::string(path + filename).c_str() << " for writing.\n";
#endif

#if defined(MINGW) || defined (MACOSX) || defined(IPHONE) || defined(GEKKO)
  m_file.open((path + filename).c_str(), (std::_Ios_Openmode)mode);
#else
  m_file.open((path + filename).c_str(), mode);
#endif
  if (!m_file.is_open())
  {
    return false;
  }
#ifdef MACOSX
  // We may be writing to a shared area so multiple users can access the
  // file.
  int CHMOD_MODE =
    S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IWOTH | S_IROTH;
  // Read + write access for all
  chmod((path + filename).c_str(), CHMOD_MODE);

#ifdef CHMOD_DEBUG
  std::cout << "CHMOD: " << (path + filename).c_str() << "\n";
#endif

#endif

  return true;
}

bool FileImplStd::WriteBinary(const char* c, int numBytes)
{
  AMJU_CALL_STACK;

  m_file.write(c, numBytes);
  return true;
}

bool FileImplStd::GetLine(std::string* pResult)
{
  AMJU_CALL_STACK;

  // Really read from a file.
  if (m_file.eof())
  {
    return false;
  }

  // TODO beware! max line length is 1000!
  char buf[1000];
  int buflen = 1000;

  m_file.getline(buf, buflen);
  *pResult = buf;

  // Chop off the last character if it happens to be char(13). This is ok for windows
  // but not unix.
  if (pResult->size() > 0 && (*pResult)[pResult->size()-1] == char(13) )
  {
    *pResult = pResult->substr(0, pResult->size() - 1);
  }

  return true;
}

unsigned int FileImplStd::GetBinary(unsigned int bytes, unsigned char* pBuffer)
{
  if (m_file.eof())
  {
    return 0;
  }

  m_file.read((char*)pBuffer, bytes);

  // Return number of bytes read
  return (unsigned int)m_file.gcount();
}

void FileImplStd::BinarySeek(unsigned int pos)
{
  AMJU_CALL_STACK;

  m_file.seekg(pos);
}

unsigned int FileImplStd::GetBinaryFileSize()
{
#if defined(MACOSX) || defined(IPHONE)
  // TODO
  // Must use stat, but we don't know the filename ???
  // Using tellg is not accurate!
  return 0;
#endif

#ifdef WIN32
  // Only valid for binary files
  // Unfortunately no such flag
  //Assert(m_isBinary);

  if (!m_file.good()) return 0;

  m_file.seekg(0,ios::end);
  std::streamoff len = m_file.tellg();
  m_file.seekg(ios::beg);
  std::streamoff pos = m_file.tellg();

  return (unsigned int)(len - pos);
#endif

#ifdef GEKKO
  Assert(0);
  return 0;
#endif // GEKKO
}

bool FileImplStd::Close()
{
  Assert(m_file.is_open()); // should already be opened
  m_file.close();
  return true;
}
}
