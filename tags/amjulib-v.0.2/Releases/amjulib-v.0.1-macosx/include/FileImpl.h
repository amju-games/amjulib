/*
Amju Games source code (c) Copyright Jason Colman 2004
*/

#ifndef FILE_IMPL_H_INCLUDED
#define FILE_IMPL_H_INCLUDED

#include <string>
#include <RCPtr.h>

namespace Amju
{
// Interface for classes implementing File operations.
class FileImpl : public RefCounted
{
public:
  virtual ~FileImpl();

  virtual bool OpenRead(
    const std::string& path, 
    const std::string& filename, 
    bool isBinary) = 0;

  virtual bool OpenWrite(
    const std::string& path, 
    const std::string& filename, 
    bool isBinary, 
    bool truncate) = 0;

  virtual bool Close() { return true; }

  virtual bool GetLine(std::string* pResult) = 0;
  virtual unsigned int GetBinary(unsigned int bytes, unsigned char* pBuffer) = 0;
  virtual void BinarySeek(unsigned int pos) = 0;
  virtual unsigned int GetBinaryFileSize() = 0;
  virtual bool WriteBinary(const char*, int numBytes) = 0;
};
}


#endif
