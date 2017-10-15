/*
Amju Games source code (c) Copyright Jason Colman 2004
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

  std::ios_base::openmode mode = ios::in;
  if (isBinary)
  {
    mode |= ios::binary;
  }

#ifdef WIN32
  m_file.open((path + filename).c_str(), mode);
#else
  m_file.open((path + filename).c_str(), mode);
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

  std::ios_base::openmode mode = ios::out;
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

#if defined(MINGW) || defined (MACOSX) || defined(AMJU_IOS) || defined(GEKKO)
  m_file.open((path + filename).c_str(), mode);
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

  std::getline(m_file, *pResult);

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
