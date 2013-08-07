/*
Amju Games source code (c) Copyright Jason Colman 2004
*/

#ifndef AMJU_FILE_INCLUDED_H
#define AMJU_FILE_INCLUDED_H

#include "FileImpl.h"
#include "Tokeniser.h"
#include <RCPtr.h>
#include <fstream>
#include <string>

namespace Amju
{
class File : public RefCounted
{
public:
  // Use to specify no version info to ctor.
  static const bool NO_VERSION;
  // Use to specify binary mode to OpenRead().
  static const bool BINARY;
  // Latest file format version number.
  static const int CURRENT_VERSION;
  // A line of dashes - use to separate sections.
  static const std::string SEPARATOR;
  
  // Create the file: normally the first integer in the file is the version
  // number. This can be suppressed, for reading different kinds of files.
  
  // Type of file to create
  enum Impl { STD, GLUE, MEM };
  File(bool hasVersionInfo = true, Impl impl = GLUE); 

  // Only for emergencies!
  // TODO Work out a better design so you can get to the data in mem files.
  FileImpl* GetImpl() { return m_pImpl.GetPtr(); }

  ~File();

  // Set the top-level directory. If this path doesn't end with sep,
  // (a slash or backslash), then this is added.
  // This root directory is the same for all files.
  // Initially there is no root, so the path is the working directory
  // for the executable.
  static void SetRoot(const std::string& root, const std::string& sep);
  static std::string GetRoot() { return s_root; }

  // Get current line number. Only valid for text files.
  int GetLine() { return m_currentLine; }
  // Get filename.
  std::string GetName() { return m_filename; }

  // Opens file, read only. Returns true if sucessful.
  // If useRoot is true, the root path is prepended to the filename.
  // You can turn this off for non-standard directories.
  bool OpenRead(
    const std::string& filename, 
    bool isBinary = false, 
    bool useRoot = true);
  
  // Open for writing. The file version number can be set here.
  bool OpenWrite(
    const std::string& filename, 
    int version = CURRENT_VERSION,
    bool isBinary = false,
    bool useRoot = true,
    bool truncate = true);
 
  bool IsOpen() const;
  
  bool Close();

  // Get size of file
  unsigned long GetFileSize();
 
  // Write a string - a line of text in text mode.
  bool Write(const std::string& text);
  
  // Write some binary data - file MUST be binary mode.
  bool WriteBinary(const char* c, unsigned int length);

  // Write an int - on a new line in text mode.
  bool WriteInteger(int i);
  
  // Write a float - on a new line in text mode.
  bool WriteFloat(float f);

  // Write a comment. Use this instead of Write(), so we can turn off comments
  // if required.
  bool WriteComment(const std::string& comment);
  
  // Error report - includes file name and line number, then uses
  // default error reporter.
  void ReportError(const std::string& error);

  // Text-based file reading
  // -----------------------
  // Returns the next line in the file, or the 1st line if the file
  // has just been Opened.
  // in binary mode, this gets characters until '\0' is found.
  bool GetLine(std::string* pResult);
  
  // GetDataLine gets the next line, ignoring comment lines.
  // (In binary mode, comments do not exist.)
  bool GetDataLine(std::string* pResult, bool trim = true);
 
  // Get the next non-comment line and treat as a localisable string.
  // Strings beginning @@@ are to be localised.
  // Strings of the format $$$<id> have been localised
  // Other strings are not localised.
  bool GetLocalisedString(std::string* pResult);
 
  // GetInteger gets the next data line and treats it as an integer,
  // or for binary mode, treats the next sizeof(int) bytes as an int.
  bool GetInteger(int* pResult);
  
  bool GetFloat(float* pResult);
  
  // More() Returns true if there are 1 or more lines left to read in the file.
  bool More() { return !m_bEOFReached; };
  
  // Get version. -1 means no version has been found.
  int GetVer() { return m_version; }
  
  // Token-based reading 
  // -------------------
  bool GetToken(std::string* pResult);
  bool FindToken(const std::string& token);
  bool GetIntToken(int* pResult);
  bool GetFloatToken(float* pResult);
  // Returns any remaining text in the current line, and clears m_tokenLine.
  // The next Token call from list above will need to get a new line. 
  std::string TokenGetLineRemainder();

  void SetTokeniser(Tokeniser*);

  // Binary files
  // ------------
  // Read <bytes> bytes from the file into the buffer owned by the caller.
  // Returns actual number of bytes written to buffer.
  // The file position read from can be set using BinarySeek() below.
  // If not explicitly set, the position starts at byte 0, and is set by
  // GetBinary() to the first byte after the current position not returned yet.
  unsigned int GetBinary(unsigned int bytes, unsigned char* pBuffer);

  // Set position from which the next call to GetBinary() will read bytes.
  void BinarySeek(unsigned int seekPos);

  // Returns true if file was opened in binary mode
  bool IsBinary() const { return m_isBinary; }

protected:
  // Read version 
  bool ReadVersion();

  bool m_isOpen;

  // Version number of file if the file format supports this.
  int m_version;

  // true if we are reading/writing a binary file
  bool m_isBinary;

  // The implementation
  RCPtr<FileImpl> m_pImpl;

  // Output file - for saving.
  std::ofstream m_os;
  
  // set true when Read() returns no characters
  bool m_bEOFReached;

  // Returns true if string is a comment.
  // Comment lines are empty or start with "//".
  bool IsComment(std::string str); 

  // Current line number, used for error reporting.
  int m_currentLine;
  // Filename, for error reporting.
  std::string m_filename;

  bool m_hasVersionInfo;

  // New tokenising functions.
  // -----------------------------
  // A Tokeniser chops a string into a token head and the remaining tail.

  // Current line to chop into tokens.
  std::string m_tokenLine;
  // Tokenising object. By delegating we can plug in different tokenising 
  // behaviour.
  Tokeniser* m_pTokeniser;
  // By default, we use this simple tokeniser.
  Tokeniser m_defaultTokeniser;

  // This path string is prepended to filenames/paths when they are opened.
  static std::string s_root;
};
}
#endif

