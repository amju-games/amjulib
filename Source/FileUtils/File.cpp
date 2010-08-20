/*
Amju Games source code (c) Copyright Jason Colman 2004
*/

#include "AmjuFirst.h"
#include <iostream>

#ifdef MACOSX
#include <sys/types.h>  // chmod
#include <sys/stat.h>   // chmod
#endif

#ifdef GEKKO
// To initialise SD card file system 
#include <fat.h>
#include "ReportError.h"
#include "Pause.h"
#endif // GEKKO

#include "File.h"
#include "FileImplStd.h"
#include "FileImplGlue.h"
#include "FileImplMem.h"
#include "Endian.h"
#include "StringUtils.h"
#include "Localise.h"
#include "AmjuAssert.h"
#include "AmjuFinal.h"

using namespace std;

namespace Amju
{
void ReportError(const string& error);

const int File::CURRENT_VERSION = 11;
const std::string File::SEPARATOR = ""; // separate lines in file.
string File::s_root;
const bool File::NO_VERSION = false;
const bool File::BINARY = true;
  
File::File(
  bool hasVersionInfo /* = true */, 
  File::Impl impl
) : 
  m_hasVersionInfo(hasVersionInfo)
{
  AMJU_CALL_STACK;

  m_bEOFReached = false; 
  m_version = -1; // meaning no version info found yet.
  m_currentLine = 0;
  m_pTokeniser = &m_defaultTokeniser;
  m_isBinary = false;

  if (impl == MEM)
  {
    m_pImpl = new FileImplMem;
  }
  else if (impl == GLUE && FileImplGlue::IsReady())
  {
    m_pImpl = new FileImplGlue;
  }
  else 
  {
    m_pImpl = new FileImplStd;
  }
}

File::~File()
{
  AMJU_CALL_STACK;

}

void File::SetRoot(const std::string& root, const std::string& sep)
{
  AMJU_CALL_STACK;

#ifdef GEKKO
  // Probably a better place than FileImplStd ctor, as we must call 
  //  this early in main to open any files...
  static bool first = true;
  if (first)
  {
    first= false;

#ifdef _DEBUG
    std::cout << "Initialising file system...\n";
#endif

    if (!fatInitDefault()) 
    {
      Amju::ReportError("fatInitDefault failed!");
    }
    else
    {
#ifdef _DEBUG
      std::cout << "Successfully initialised file system!\n";
#endif
    }
  }
#endif // GEKKO

  s_root = root;
  if (root.empty())
  {
    return;
  }
  // Get last character. If it isn't a slash/backslash, append one.
  char lastch = s_root[s_root.size() - 1];
  string last(1, lastch); // construct string from 1 char
  if (last == sep)
  {
    return;
  }
  s_root += sep;
}

bool File::ReadVersion()
{
  AMJU_CALL_STACK;

  // If this file has been flagged as containing version info,
  // get the first integer, and store as the version.
  m_version = 0;
  if (m_hasVersionInfo)
  {
    // Get the version number.
    if (!GetInteger(&m_version))
    {
      return false;
    }
  }
  return true;
}

bool File::OpenRead(const std::string& filename, bool isBinary, bool useRoot) 
{
  AMJU_CALL_STACK;

  Assert(!filename.empty());

  m_filename = filename;
  m_isBinary = isBinary;

  std::string root;
  if (useRoot) 
  {
    root = s_root;
  }

  if (!m_pImpl->OpenRead(root, filename, isBinary))
  {
    ReportError("Failed to open file for reading: " + filename);
    m_bEOFReached = true; 
    return false;
  }

  return ReadVersion();
}

unsigned long File::GetFileSize()
{
  return m_pImpl->GetBinaryFileSize();
}

unsigned int File::GetBinary(unsigned int bytes, unsigned char* pBuffer)
{
  if (m_bEOFReached)
  {
    return 0;
  }

  return m_pImpl->GetBinary(bytes, pBuffer);
}

void File::BinarySeek(unsigned int seekPos)
{
  AMJU_CALL_STACK;

  m_pImpl->BinarySeek(seekPos);
}

void File::ReportError(const string& error)
{
  AMJU_CALL_STACK;

  // Add filename and line number to error message.
  string newError = GetName() + ", Line " + ToString(GetLine()) + ": " + error;
  Amju::ReportError(newError);
}

bool File::OpenWrite(
  const std::string& filename, 
  int version, 
  bool isBinary,
  bool useRoot,
  bool truncate)
{
  AMJU_CALL_STACK;

  m_filename = filename;
  m_isBinary = isBinary;
 
  std::string root;
  if (useRoot) 
  {
    root = s_root;
  }

  if (!m_pImpl->OpenWrite(root, filename, isBinary, truncate))
  {
    return false;
  }
 
  if (m_hasVersionInfo)
  {
    WriteComment("// File version");
    // Write version number
    WriteInteger(version);
    WriteComment(SEPARATOR);
  }
  return true;
}

bool File::Write(const std::string& text)
{
  AMJU_CALL_STACK;

  if (m_isBinary)
  {
    WriteBinary(text.c_str(), text.length());
    WriteBinary("\0", 1);

    //m_os.write(text.c_str(), text.length());
    //m_os.write("\0", 1);
  }
  else
  {
    WriteBinary(text.c_str(), text.length());
    WriteBinary("\n", 1); 

    //m_os << text.c_str() << endl;

    m_currentLine++;
  }
  return true;
}

bool File::WriteBinary(const char* c, unsigned int length)
{
  AMJU_CALL_STACK;

  if (length > 0)
  {
    return m_pImpl->WriteBinary(c, length);
  }
  return true;
}

bool File::WriteComment(const std::string& comment)
{
  AMJU_CALL_STACK;

  if (m_isBinary)
  {
    return true;
  }
  return Write(comment);
}

bool File::WriteInteger(int i)
{
  AMJU_CALL_STACK;

  if (m_isBinary)
  {
    // File reads use Endian, so we must do the same for writes.
    i = Endian(i);

    WriteBinary((const char*)(&i), sizeof(int));

    //m_os.write((const char*)(&i), sizeof(int));
  }
  else
  {
    std::string s = ToString(i) + "\n";
    WriteBinary(s.c_str(), s.length());

    //m_os << i << endl;

    m_currentLine++;
  }
  
  return true;
}

bool File::WriteFloat(float f)
{
  AMJU_CALL_STACK;

  if (m_isBinary)
  {
    f = Endian(f);
    
    WriteBinary((const char*)(&f), sizeof(f));
    
    //m_os.write((const char*)(&f), sizeof(float));
  }
  else
  {
    std::string s = ToString(f) + "\n";
    WriteBinary(s.c_str(), s.length());
    
    //m_os << f << endl;   

    m_currentLine++;
  }

  return true;
}

bool File::GetLine(string* pResult)
{
  AMJU_CALL_STACK;

  Assert(pResult);
  if (m_bEOFReached) 
  {
    return false;
  }

  if (m_isBinary)
  {
    // Get characters until '\0' is found.
    char c;
    while (1)
    {
      if (GetBinary(1, (unsigned char*)&c) == 0)
      {
        m_bEOFReached = true;
        return false;
      }
      if (c)
      {
        (*pResult) += c;
      }
      else
      {
        return true;
      }
    }
  }
  else
  {
    // Get a line from the current file (only valid for Text mode files).
    if (m_pImpl->GetLine(pResult))
    {
      m_currentLine++;
      return true;
    }
  }

  return false;
}

bool File::IsComment(std::string str)
{
  AMJU_CALL_STACK;

  if (str.empty() ) 
  {
    return true;
  }
  if (str.size() < 2)
  {
    return false;
  }

  return (str.substr(0, 2) == "//");
}

bool File::GetLocalisedString(std::string* pResult)
{
  AMJU_CALL_STACK;

  if (!GetDataLine(pResult))
  {
    return false;
  }

  Lookup(pResult);

  return true;
}

bool File::GetDataLine(string* pResult)
{
  AMJU_CALL_STACK;

  std::string str;
  do 
  {
    if (!GetLine(&str))
    {
      return false;
    }
    // Strip whitespace from beginning and end of the string.
    Trim(&str);

  } while (IsComment(str) && More());
    
  *pResult = str;
  return true;
}

bool File::GetInteger(int* pResult)
{
  AMJU_CALL_STACK;

  if (m_isBinary)
  {
    int bytes = GetBinary(sizeof(int), (unsigned char*)pResult);
    *pResult = Endian((int32)*pResult);
    return (bytes != 0);
  }

  // Text mode: get the next non-comment line, and atoi() it.
  std::string str;
  if (!GetDataLine(&str))
  {
    ReportError("Unexpected end of file, wanted integer.");
    return false;
  }

#if defined(_DEBUG) || defined(SCENE_EDITOR)
  // Check this is an integer - i.e. is a sequence of digits.
  int s = 0;
  int len = str.size();
  Assert(len > 0); // GetDataLine() should not return an empty line
  // Leading minus sign is ok, as long as it is followed by a number.
  if (str[0] == '-')
  {
    s = 1;
    if (len == 1) 
    {
      ReportError("'-' is not an integer.");
      return false;
    }
  }
  for (int i = s; i < len; i++)
  {
    if (!isdigit(str[i]))
    {
      string error = "'" + str + "' is not an integer.";
      ReportError(error);
      return false;
    }
  }
#endif

  *pResult = atoi(str.c_str());
  return true;
}

bool File::GetFloat(float* pResult)
{
  AMJU_CALL_STACK;

  if (m_isBinary)
  {
    int bytes = GetBinary(sizeof(float), (unsigned char*)pResult);
    *pResult = Endian(*pResult);
    return (bytes != 0);
  }

  std::string str;
  if (!GetDataLine(&str))
  {
    return false;
  }
  *pResult = (float)atof(str.c_str());
  return true;
}

bool File::GetToken(std::string* pResult)
{
  AMJU_CALL_STACK;

  // For importing text files, e.g. .ASC format
  if (m_tokenLine.empty())
  {
    if (!GetDataLine(&m_tokenLine))
    {
      // No more data lines left.
      return false;
    }
  }
  Assert(!m_tokenLine.empty());
  Assert(m_pTokeniser);
  
  // 'Tokenisation' is delegated to a tokeniser object. 
  return m_pTokeniser->Tokenise(&m_tokenLine, pResult);
}

std::string File::TokenGetLineRemainder()
{
  std::string s = m_tokenLine;
  m_tokenLine = "";
  return s;
}

bool File::FindToken(const std::string& token)
{
  AMJU_CALL_STACK;

  string s;
  while (GetToken(&s))
  {
    if (s == token)
    {
      return true;
    }
  }
  return false;
}

bool File::GetIntToken(int* pResult)
{
  AMJU_CALL_STACK;

  // Importing text file.. no binary version.
  string s;
  if (!GetToken(&s))
  {
    return false;
  }
  *pResult = atoi(s.c_str());
  return true;
}

bool File::GetFloatToken(float* pResult)
{
  AMJU_CALL_STACK;

  // Importing text file.. no binary version.
  string s;
  if (!GetToken(&s))
  {
    return false;
  }
  *pResult = (float)atof(s.c_str());
  return true;
}

void File::SetTokeniser(Tokeniser* p)
{
  AMJU_CALL_STACK;

  m_pTokeniser = p;
}

bool File::Close()
{
  AMJU_CALL_STACK;
  return m_pImpl->Close();
}
}


