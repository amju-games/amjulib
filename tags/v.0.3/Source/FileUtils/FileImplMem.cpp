/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
*/

#include "AmjuFirst.h"
#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "FileImplMem.h"
#include "AmjuFinal.h"

namespace Amju
{
FileImplMem::FileImplMem()
{
  AMJU_CALL_STACK;

  m_index = 0;
  m_isBinary = false;
}

FileImplMem::~FileImplMem()
{
  AMJU_CALL_STACK;

}

bool FileImplMem::OpenRead(
    const std::string& path,
    const std::string& filename,
    bool isBinary)
{
  AMJU_CALL_STACK;

  m_index = 0;
  m_isBinary = isBinary;
  return true;
}

bool FileImplMem::OpenWrite(
    const std::string& path,
    const std::string& filename,
    bool isBinary,
    bool truncate)
{
  AMJU_CALL_STACK;

  m_index = 0;
  m_isBinary = isBinary;
  return true;
}

bool FileImplMem::GetLine(std::string* pResult)
{
  AMJU_CALL_STACK;

  (*pResult) = ""; // No clear() in MSVC!?

  if (m_index == m_data.size())
  {
    return false;
  }
  if (m_isBinary)
  {
    while (m_index < m_data.size())
    {
      char c = m_data[m_index];
      ++m_index;
      if (c == 0)
      {
        break;
      }
      *pResult += c;
    }
  }
  else
  {
    while (m_index < m_data.size())
    {
      char c = m_data[m_index];
      ++m_index;
      if (c == 10 || c == 13)
      {
        break;
      }
      *pResult += c;
    }
    // Skip next char if it is 13 (windows line feed char ?)
    if (m_index < m_data.size())
    {
      if (m_data[m_index] == 13 || m_data[m_index] == 10)
      {
        ++m_index;
      }
    } 
  }

#ifdef _DEBUG
std::cout << "FileImplMem line: \"" << pResult->c_str() << "\"\n";
#endif

  return true;
}

unsigned int FileImplMem::GetBinary(unsigned int maxbytes, unsigned char* pBuffer)
{
  unsigned int bytes = maxbytes;
  if (bytes > m_data.size() - m_index)
  {
    bytes = m_data.size() - m_index;
  } 
  for (unsigned int i = 0; i < bytes; i++)
  {
    *pBuffer++ = m_data[m_index];
	m_index++;
  } 
  return bytes;
}

void FileImplMem::BinarySeek(unsigned int pos)
{
  AMJU_CALL_STACK;

  m_index = pos;
}

unsigned int FileImplMem::GetBinaryFileSize()
{
  return m_data.size();
}

bool FileImplMem::WriteBinary(const char* c, int numBytes)
{
  AMJU_CALL_STACK;

  // Reallocate if necessary
  m_data.reserve(m_data.size() + numBytes);

  for (int i = 0; i < numBytes; i++)
  {
    m_data.push_back((unsigned char)c[i]);
  }

  m_index += numBytes;
  return true;
}

const BinaryData& FileImplMem::GetData() const
{
  AMJU_CALL_STACK;

  return m_data;
}

void FileImplMem::SetData(const std::string& s)
{
  AMJU_CALL_STACK;

  m_data.clear();
  m_data.reserve(s.size());
  unsigned int size = s.size();
  for (unsigned int i = 0; i < size; i++)
  {
    m_data.push_back(s[i]);
  }
}

void FileImplMem::SetData(const BinaryData& data)
{
  AMJU_CALL_STACK;

  m_data = data;
  m_index = 0;
}
}



