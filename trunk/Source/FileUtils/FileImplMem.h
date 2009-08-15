/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
*/

#ifndef FILE_IMPL_MEM_H_INCLUDED
#define FILE_IMPL_MEM_H_INCLUDED

#include "FileImpl.h"
#include "BinaryData.h"

namespace Amju
{
// Implements a file as bytes in memory
class FileImplMem : public FileImpl
{
public:
  FileImplMem();
  virtual ~FileImplMem();

  virtual bool OpenRead(
    const std::string& path,
    const std::string& filename,
    bool isBinary);

  virtual bool OpenWrite(
    const std::string& path,
    const std::string& filename,
    bool isBinary,
    bool truncate);

  virtual bool GetLine(std::string* pResult);
  virtual unsigned int GetBinary(unsigned int bytes, unsigned char* pBuffer);
  virtual void BinarySeek(unsigned int pos);
  virtual unsigned int GetBinaryFileSize();
  virtual bool WriteBinary(const char*, int numBytes);

  // Use e.g. vector<unsigned char> so binary files will work
  const BinaryData& GetData() const;
  void SetData(const BinaryData& data);
  // Convenience, allow setting from string
  void SetData(const std::string& textData);

protected:
  // This is the memory appended to with WriteBinary.
  BinaryData m_data; 
  unsigned int m_index; 
  bool m_isBinary;
};
}

#endif

