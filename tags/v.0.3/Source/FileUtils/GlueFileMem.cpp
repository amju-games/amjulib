/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GlueFileMem.cpp,v $
Revision 1.5  2008/05/08 10:58:04  jay
New memory management code

Revision 1.4  2006/06/27 18:16:39  jay
Report filename in error msgs, as there may be multiple glue files

Revision 1.3  2006/06/17 13:40:05  jay
Debug print

Revision 1.2  2006/02/17 18:59:08  jay
Added new GetBinary() overload. This returns an _object_ containing a
pointer to a buffer. The buffer can be allocated by the object, which then
owns the buffer; or, the pointer can point to memory allocated by
something else.

Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
  
*/

#include "AmjuFirst.h"
#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include <string.h> // memcpy
#include "GlueFileMem.h"
#include "FileSize.h"
#include "AmjuAssert.h"
#include "AmjuFinal.h"

using namespace std;
namespace Amju
{
GlueFileMem::GlueFileMem()
{
  AMJU_CALL_STACK;

  m_pMemFile = 0;
  m_filePos = 0;
}

GlueFileMem::~GlueFileMem()
{
  AMJU_CALL_STACK;

  delete [] m_pMemFile;
}

bool GlueFileMem::OpenGlueFile(const std::string gluefilename, bool read)
{
  AMJU_CALL_STACK;

#ifdef _DEBUG
std::cout << "Reading Glue file into memory: " << gluefilename.c_str() << "\n";
#endif

  Assert(read);
  if (!read)
  {
#ifdef _DEBUG
    std::cout << "Glue file mem: read-only flag is " << (read ? "true" : "false") << std::endl;
#endif
    return false;
  }

  if (!GlueFile::SimpleOpen(gluefilename.c_str(), read))
  {
    return false;
  }

  // Get the size of the file, and allocate a big enough buffer.
  // Then read the whole file into the buffer.
  m_fileSize = FileSize(gluefilename.c_str());

  Assert(m_fileSize > 0);

  m_pMemFile = new unsigned char[m_fileSize];
  if (!m_pMemFile)
  {
#ifdef _DEBUG
    std::cout << "Failed to allocate memory: " << m_fileSize << " bytes" << std::endl;
#endif
    return false;
  }

  m_gluefile.seekg(0);
  m_gluefile.read((char*)m_pMemFile, m_fileSize);

  bool reachedEnd = ((unsigned long)m_gluefile.tellg() == m_fileSize);
  if (reachedEnd)
  {
#ifdef _DEBUG
    cout << "Read all of glue file: " << gluefilename.c_str() <<  endl;
#endif
  }
  else
  {
    cout << "GLUEFILE: " << gluefilename.c_str()
      << " Only read " << (unsigned long)m_gluefile.tellg() << " bytes!" << endl;
  }
  
  return ReadTable();
}

void GlueFileMem::SetPos(uint32 pos)
{
  AMJU_CALL_STACK;

  m_filePos = pos;
}

uint32 GlueFileMem::GetPos()
{
  AMJU_CALL_STACK;

  return m_filePos;
}

uint32 GlueFileMem::GetBinary(uint32 numbytes, unsigned char* pBuffer)
{
  AMJU_CALL_STACK;

  Assert(m_filePos + numbytes <= m_fileSize);

  // dest, src, count
  memcpy(pBuffer, m_pMemFile + m_filePos, numbytes);
  m_filePos += numbytes;
  return numbytes;
}

uint32 GlueFileMem::GetBinary(uint32 seekPos, uint32 numbytes, unsigned char* pBuffer)
{
  AMJU_CALL_STACK;

  // ** NB Unlike the base class implementation, this _is_ MT safe for read-only
  // files.

  // dest, src, count
  memcpy(pBuffer, m_pMemFile + seekPos, numbytes);
  return numbytes;
}

GlueFileBinaryData GlueFileMem::GetBinary(uint32 seekPos, uint32 numbytes)
{
  AMJU_CALL_STACK;

  GlueFileBinaryData data;
  data.SetBuffer(m_pMemFile + seekPos); 
  return data;  
}
}
