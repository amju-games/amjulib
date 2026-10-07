// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#if !defined(GLUE_FILE_MEM_H_INCLUDED)
#define GLUE_FILE_MEM_H_INCLUDED

#include "GlueFile.h"

namespace Amju
{
// A glue file which loads the whole file into memory when it is opened
// for reading.
// This is to allow concurrent access to the file from multiple threads.
class GlueFileMem : public GlueFile
{
public:
  GlueFileMem();
  virtual ~GlueFileMem();
  virtual bool OpenGlueFile(const std::string gluefilename, bool read);
  virtual uint32 GetPos();
  virtual void SetPos(uint32 pos);
  virtual uint32 GetBinary(uint32 numbytes, unsigned char* pBuffer);
  virtual uint32 GetBinary(uint32 seekPos, uint32 numbytes, unsigned char* pBuffer);
  virtual GlueFileBinaryData GetBinary(uint32 seekPos, uint32 numbytes);

protected:
  unsigned char* m_pMemFile;
  unsigned int m_fileSize; 
  unsigned int m_filePos;
};
}

#endif

