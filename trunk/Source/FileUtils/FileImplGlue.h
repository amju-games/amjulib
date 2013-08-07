/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FileImplGlue.h,v $
Revision 1.2  2006/12/01 23:28:03  jay
New FileImplMem, unfortunately causing many small changes

Revision 1.1  2004/09/08 15:43:18  jay
Added to repository
  
*/

#ifndef AMJU_FILE_IMPL_GLUE_H_INCLUDED
#define AMJU_FILE_IMPL_GLUE_H_INCLUDED

#include "FileImpl.h"
#include "GlueFile.h"
#include <string>

namespace Amju
{
// Implements the FileImpl interface, using a GlueFile (archive consisting of
// many sub-files).
// To use this class you must first make the GlueFile! Duh!
/*
There can be many instances of FileImplGlue, as each File creates a new
impl. But they all read from the same GlueFile. So each FileImplGlue
refers to one Singleton GlueFile. Each FileImplGlue stores its position in 
the main Glue file.
This lets us have multiple open sub-files, but is totally NOT THREAD SAFE.
I.e. file access using FileImplGlue CANNOT be concurrent!
*/
class FileImplGlue : public FileImpl
{
public:
  FileImplGlue();
  virtual ~FileImplGlue();

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

  // Before using the functions above, the archive must be opened.
  // Optionally pass in the Glue File object (created on HEAP). If zero,
  // a new Glue File is created. This is a way of letting the client
  // specify the glue file subtype used.
  static bool OpenGlueFile(const std::string& gluefilename, GlueFile* pGf = 0);

  // Returns true if a Glue file has been opened.
  static bool IsReady() { return s_isReady; }

  // Get the single Glue File used by all objects of this type.
  static GlueFile* GetGlueFile();

protected:
  // Name of the open sub-file in the GlueFile.
  std::string m_subfilename;

  // GlueFile isn't a Singleton - it's plausible to have more than one - but 
  // each FileImplGlue uses the same one.
  static RCPtr<GlueFile> s_pGlueFile;

  // Store the current file position of the GlueFile.
  // This is so we can have multiple FileImplGlues, pointing to different
  // places in the same Glue file.
  uint32 m_pos;

  // Base pos for this subfile in the glue file
  uint32 m_base; 
 
  // Set when a glue file is successfully opened.
  static bool s_isReady;
};
}

#endif

