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

namespace Amju
{
// Lists all files in the given directory.
// If useGlue is true, and a Glue File is ready, the list of files in
// the glue file is returned. In this case the directory is not used.
// Returns true if successful.
bool Dir(
  const std::string& directory, 
  std::vector<std::string>* pResult, 
  bool useGlue = true);


// Get directory for saving preferences, etc.
// This is in the file system, not the glue file.
// For Mac OS X, we use /Users/Shared/<application name>.
// For Windows, we try to get something similar. If this fails (Win 98/NT)
// we use the File::Root directory. The reason is that users do not expect
// a new high-level  directory to be created on their disk, and don't like it.
std::string GetSaveDir();

// Get user desktop directory.
// This is for saving fles that we want the user to be able to
// find easily.
std::string GetDesktopDir();

// Make a directory in OS filesystem - glue file not used.
bool MkDir(const std::string& dir);

bool FileExists(const std::string& filename);

// Delete a file FROM THE OS FILESYSTEM: not a glue file
enum DeleteResult { AMJU_FILE_DOES_NOT_EXIST, AMJU_DELETED_OK, AMJU_DELETE_FAILED };
DeleteResult AmjuDeleteFile(const std::string& filename);
}

#endif

