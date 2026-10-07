// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#if !defined(FILE_IMPL_STD_H_INCLUDED)
#define FILE_IMPL_STD_H_INCLUDED

#include "FileImpl.h"
#include <fstream>

namespace Amju
{
// Implements File operations using standard file functions.
class FileImplStd : public FileImpl
{
public:
  FileImplStd();
  virtual ~FileImplStd();

  virtual bool OpenRead(
    const std::string& path, 
    const std::string& filename, 
    bool isBinary);

  virtual bool OpenWrite(
    const std::string& path, 
    const std::string& filename, 
    bool isBinary, 
    bool truncate);

  virtual bool Close();

  virtual bool GetLine(std::string* pResult);
  virtual unsigned int GetBinary(unsigned int bytes, unsigned char* pBuffer);
  virtual void BinarySeek(unsigned int pos);
  virtual unsigned int GetBinaryFileSize();
  virtual bool WriteBinary(const char*, int numBytes);

protected:
  // This is the real file to read from
  std::fstream m_file; 

};
}

#endif
