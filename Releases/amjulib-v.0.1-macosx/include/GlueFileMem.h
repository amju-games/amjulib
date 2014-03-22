/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GlueFileMem.h,v $
Revision 1.2  2006/02/17 18:59:08  jay
Added new GetBinary() overload. This returns an _object_ containing a
pointer to a buffer. The buffer can be allocated by the object, which then
owns the buffer; or, the pointer can point to memory allocated by
something else.

Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
  
*/

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

