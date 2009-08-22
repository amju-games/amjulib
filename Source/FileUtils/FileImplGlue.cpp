/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FileImplGlue.cpp,v $
Revision 1.4  2008/05/08 10:58:03  jay
New memory management code

Revision 1.3  2006/12/01 23:28:03  jay
New FileImplMem, unfortunately causing many small changes

Revision 1.2  2006/06/27 18:13:09  jay
Useful error msg when file not in glue file

Revision 1.1  2004/09/08 15:43:18  jay
Added to repository
  
*/

#include "AmjuFirst.h"
#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "FileImplGlue.h"
#include "AmjuAssert.h"
#include "AmjuFinal.h"

namespace Amju
{
void ReportError(const std::string&);

SharedPtr<GlueFile> FileImplGlue::s_pGlueFile;
bool FileImplGlue::s_isReady = false;

FileImplGlue::FileImplGlue()
{
  AMJU_CALL_STACK;

  Assert(s_pGlueFile.GetPtr());
  // Store the old file position
  m_oldPos = s_pGlueFile->GetPos();
}

FileImplGlue::~FileImplGlue()
{
  AMJU_CALL_STACK;

  Assert(s_pGlueFile.GetPtr());
  // Restore the stored file pos
  s_pGlueFile->SetPos(m_oldPos);
}

bool FileImplGlue::OpenWrite(
    const std::string& path,
    const std::string& filename,
    bool isBinary,
    bool truncate)
{
  AMJU_CALL_STACK;

  // TODO can't write to a Glue file
  return false;
}

bool FileImplGlue::WriteBinary(const char*, int numBytes)
{
  AMJU_CALL_STACK;

  // TODO can't write to a Glue file
  return false;
}

GlueFile* FileImplGlue::GetGlueFile()
{
  AMJU_CALL_STACK;

  return s_pGlueFile.GetPtr();
}

bool FileImplGlue::OpenRead(const std::string& path, const std::string& filename, bool isBinary)
{
  AMJU_CALL_STACK;

  // Ignore the path.
  // Get the start pos of the sub-file we want and seek to there.
  uint32 base = 0;
  if (!s_pGlueFile->GetSeekBase(filename, &base))
  {
    std::string s = "No such file: " + filename + " in glue file: " 
      + s_pGlueFile->GetFilename();
    ReportError(s);
    return false;
  }
  s_pGlueFile->SetPos(base);
  m_subfilename = filename;
  return true;
}

bool FileImplGlue::GetLine(std::string* pResult)
{
  AMJU_CALL_STACK;

  // Only called if the current sub-file is text.
  // Get characters until we hit end-of-line char.
  // Strip off any MS-DOS ^M char preceding it.
  // TODO Optimize this!
  *pResult = std::string();
  unsigned char c;
  while (GetBinary(1, &c) == 1)
  {
    if (c == 10)
    {
      return true;
    }
    if (c != 13)
    {
      (*pResult) += c;
    }
  }
  return true;
}

unsigned int FileImplGlue::GetBinary(unsigned int bytes, unsigned char* pBuffer)
{
  return s_pGlueFile->GetBinary(bytes, pBuffer);
}

void FileImplGlue::BinarySeek(unsigned int pos)
{
  AMJU_CALL_STACK;

  uint32 base = 0;
  if (!s_pGlueFile->GetSeekBase(m_subfilename, &base))
  {
    return;
  }
  s_pGlueFile->SetPos(base + pos);
}

unsigned int FileImplGlue::GetBinaryFileSize()
{
  return s_pGlueFile->GetSize(m_subfilename);
}

bool FileImplGlue::OpenGlueFile(const std::string& gluefilename, GlueFile* pGf)
{
  AMJU_CALL_STACK;

  // Set up the GlueFile which every implementation will access.
  if (pGf)
  {
    s_pGlueFile = pGf;
  }
  else
  {
    s_pGlueFile = new GlueFile;
  }
  bool b = s_pGlueFile->OpenGlueFile(gluefilename, true); // true means read-only
  s_isReady = b;
  return b;
}
}
