/*
Amju Games source code (c) Copyright Jason Colman 2010
*/

#include <AmjuFirst.h>
#include "FileImplGlue.h"
#include <AmjuAssert.h>
#include <AmjuFinal.h>

namespace Amju
{
void ReportError(const std::string&);

SharedPtr<GlueFile> FileImplGlue::s_pGlueFile;
bool FileImplGlue::s_isReady = false;

FileImplGlue::FileImplGlue()
{
  AMJU_CALL_STACK;

  Assert(s_pGlueFile.GetPtr());

  m_base = 0;
  m_pos = 0;
}

FileImplGlue::~FileImplGlue()
{
  AMJU_CALL_STACK;

  Assert(s_pGlueFile.GetPtr());
}

bool FileImplGlue::OpenWrite(
    const std::string& path,
    const std::string& filename,
    bool isBinary,
    bool truncate)
{
  AMJU_CALL_STACK;

  // TODO can't write to a Glue file
  Assert(0);
  return false;
}

bool FileImplGlue::WriteBinary(const char*, int numBytes)
{
  AMJU_CALL_STACK;

  // TODO can't write to a Glue file
  Assert(0);
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
  if (!s_pGlueFile->GetSeekBase(filename, &m_base))
  {
    std::string s = "No such file: " + filename + " in glue file: " 
      + s_pGlueFile->GetFilename();
    ReportError(s);
    return false;
  }

  s_pGlueFile->SetPos(m_base);
  m_pos = s_pGlueFile->GetPos();

  m_subfilename = filename;
  return true;
}

bool FileImplGlue::GetLine(std::string* pResult)
{
  AMJU_CALL_STACK;

  s_pGlueFile->SetPos(m_pos);

  unsigned int pos = m_pos - m_base; // position within subfile
  unsigned int size = s_pGlueFile->GetSize(m_subfilename);
  if (pos >= size)
  {
    // EOF reached
    return false;
  }

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
  m_pos = s_pGlueFile->GetPos();
  return true;
}

unsigned int FileImplGlue::GetBinary(unsigned int bytes, unsigned char* pBuffer)
{
  s_pGlueFile->SetPos(m_pos);
  unsigned int r =  s_pGlueFile->GetBinary(bytes, pBuffer);
  m_pos = s_pGlueFile->GetPos();
  return r;
}

void FileImplGlue::BinarySeek(unsigned int pos)
{
  AMJU_CALL_STACK;

  s_pGlueFile->SetPos(m_base + pos);
  m_pos = s_pGlueFile->GetPos();
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
