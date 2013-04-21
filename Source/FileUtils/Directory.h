/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Directory.h,v $
Revision 1.5  2008/03/18 09:25:45  jay
Move Filter() function to StringUtils

Revision 1.4  2007/02/02 14:00:39  Administrator
Mingw GCC fix

Revision 1.3  2006/10/28 22:32:00  Administrator
Add delete file function

Revision 1.2  2006/09/08 21:49:09  jay
Added FileExists()

Revision 1.1  2004/09/08 15:43:18  jay
Added to repository
  
*/

#if !defined(AMJU_DIRECTORY_H_INCLUDED)
#define AMJU_DIRECTORY_H_INCLUDED

#include <string>
#include <vector>

#ifdef IPHONE
#include "iOSGetDir.h"
#endif


namespace Amju
{
class Time;

// Lists all files in the given directory.
// File::Root is NOT prepended to directory.
// If useGlue is true, and a Glue File is ready, the list of files in
// the glue file is returned. In this case the directory is not used.
// Returns true if successful.
struct DirEnt
{
  DirEnt() : m_isDir(false) {}
  DirEnt(const std::string& name, bool isDir) : m_name(name), m_isDir(isDir) {}

  std::string m_name;
  bool m_isDir;
};
typedef std::vector<DirEnt> DirEnts;

bool Dir(
  const std::string& directory, 
  DirEnts* pResult, 
  bool useGlue = true);


// Get directory for saving preferences, etc.
// This is in the file system, not the glue file.
// For Mac OS X, we use /Users/Shared/<application name>.
// For Windows, we try to get something similar. If this fails (Win 98/NT)
// we use the File::Root directory. The reason is that users do not expect
// a new high-level  directory to be created on their disk, and don't like it.
std::string GetSaveDir(const std::string& appName);

// Get directory where we expect to find data.
// For Windows: it's <process dir>/Data
// For Mac: <bundle dir>/Contents/Resources/Data
std::string GetDataDir();
  
// Get user desktop directory.
// This is for saving files that we want the user to be able to
// find easily.
std::string GetDesktopDir();

// Get the directory where the process lives.
// For Mac Bundles, it is the directory where the executable lives INSIDE THE APP bundle (*.app).
// For other executables, it is the directory where this executable lives.
std::string GetProcessDir();

// Make a directory in OS filesystem - glue file not used.
bool MkDir(const std::string& dir);

bool FileExists(const std::string& filename);

// Delete a file FROM THE OS FILESYSTEM: not a glue file
enum DeleteResult { AMJU_FILE_DOES_NOT_EXIST, AMJU_DELETED_OK, AMJU_DELETE_FAILED };
DeleteResult AmjuDeleteFile(const std::string& filename);

// Copy an OS file from srcDir to destDir. I.e. like 'cp srcDir/filename destDir/'
// srcDir and destDir do NOT have File::Root prepended.
bool FileCopy(const std::string& srcDir, const std::string& destDir, const std::string& filename);

// Get last time OS file was modified
Time GetFileModifiedTime(const std::string& filename); 
}

#endif

