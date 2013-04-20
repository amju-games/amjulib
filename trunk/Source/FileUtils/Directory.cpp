/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Directory.cpp,v $
Revision 1.17  2008/05/18 16:04:23  jay
MSVC update

Revision 1.16  2008/05/08 10:58:03  jay
New memory management code

Revision 1.15  2008/03/18 09:25:45  jay
Move Filter() function to StringUtils

Revision 1.14  2007/12/28 22:53:58  jay
Fixes for makelevelbin and makeleafbin following AmjuGL refactor

Revision 1.13  2007/12/12 16:51:43  staff
Windows: VC8: fixes for deprecated mkdir and unlink

Revision 1.12  2007/10/22 21:04:32  jay
Added some debug error msgs for when mkdir fails

Revision 1.11  2007/10/21 22:54:56  jay
mkdir error msgs

Revision 1.10  2007/02/02 14:00:39  Administrator
Mingw GCC fix

Revision 1.9  2007/01/23 23:42:26  Administrator
Windows fix: use common application data dir

Revision 1.8  2007/01/08 21:45:12  Administrator
Windows Save Dir: ony use Data dir in release mode, so we don't get
rubbish in the Data folder which could get glued into data.glue

Revision 1.7  2007/01/05 22:52:04  jay
Win: just use Data dir as Save dir

Revision 1.6  2006/10/28 22:31:59  Administrator
Add delete file function

Revision 1.5  2006/09/15 08:37:39  Administrator
Win compile fix

Revision 1.4  2006/09/08 21:49:09  jay
Added FileExists()

Revision 1.3  2006/06/27 18:11:56  jay
Debug cout on chmod

Revision 1.2  2006/01/16 13:32:57  jay
Mac save dir is now in /Users/Shared, not user-specific

Revision 1.1  2004/09/08 15:43:18  jay
Added to repository
  
*/

#include <AmjuFirst.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <algorithm>
#include <iostream>
#include <AmjuTime.h>
#include "Directory.h"
#include "GlueFile.h"
#include "FileImplGlue.h"
#include "StringUtils.h"
#include "File.h"

#if defined(WIN32)
#include <io.h>
#ifdef MSVC
#include "Direct.h"
#endif
#include <windows.h>
#include <shlobj.h>

#if !defined CSIDL_COMMON_APPDATA
#define CSIDL_COMMON_APPDATA 35
#endif

#else
#include <dirent.h>
#include <errno.h>
#include <libgen.h>
#include <string.h>
#endif

#ifdef MACOSX
#include <Carbon/Carbon.h>
#endif

#ifdef IPHONE
#include "iOSGetDir.h"
#endif

#include <AmjuFinal.h>

//#define DIRECTORY_DEBUG

namespace Amju
{
bool Dir(
  const std::string& directory, 
  DirEnts* pResult, 
  bool useGlue /* = true */)
{
  AMJU_CALL_STACK;

  if (useGlue && FileImplGlue::IsReady())
  {
    // Return list of files in Glue File.
    Strings strs;
    if (!FileImplGlue::GetGlueFile()->Dir(&strs))
    {
      return false;
    }
    for (unsigned int i = 0; i < strs.size(); i++)
    {
      pResult->push_back(DirEnt(strs[i], false));
    }
    return true;
  }
 
  // Return list of files in directory.
#if defined(WIN32)
  _finddata_t fileinfo;
  std::string s = directory + "/*.*";
  long f = _findfirst(s.c_str(), &fileinfo);
  if (f == -1)
  {
    return true; // no files
  }

  do
  {
    std::string f = StripPath(fileinfo.name);
    bool isDir = ((fileinfo.attrib & _A_SUBDIR) != 0);
    DirEnt de(f, isDir);
    pResult->push_back(de);
  }
  while (_findnext(f, &fileinfo) == 0); // 0 => success
  return true;

#else
  DIR* dirp = opendir(directory.c_str());
  if (!dirp)
  {
    return false;
  }

  while (struct dirent* dp = readdir(dirp))
  {
    std::string f = dp->d_name;
    f = StripPath(f);

    struct stat buf;
    bool exists = (stat((directory + "/" + f).c_str(), &buf) != -1);
    if (!exists)
    {
      std::cout << "Can't stat file listed by readdir..!?!\n";
      Assert(0);
      continue;
    }
    bool isDir = ((buf.st_mode & S_IFDIR) != 0);
    DirEnt de(f, isDir);
    pResult->push_back(de);
  }
  closedir(dirp);
  return true;
#endif

  return true; 
}


/* Function with behaviour like `mkdir -p'  */
// From: http://niallohiggins.com/2009/01/08/mkpath-mkdir-p-alike-in-c-for-unix/
int
mkpath(const char *s, mode_t mode)
{
  char *q, *r = NULL, *path = NULL, *up = NULL;
  int rv = -1;
  if (strcmp(s, ".") == 0 || strcmp(s, "/") == 0)
    return (0);

  if ((path = strdup(s)) == NULL)
    exit(1);
    
  if ((q = strdup(s)) == NULL)
    exit(1);

  if ((r = dirname(q)) == NULL)
    goto out;
        
  if ((up = strdup(r)) == NULL)
    exit(1);

  if ((mkpath(up, mode) == -1) && (errno != EEXIST))
    goto out;

  if ((mkdir(path, mode) == -1) && (errno != EEXIST))
    rv = -1;
  else
    rv = 0;

out:
  if (up != NULL)
    free(up);
  free(q);
  free(path);
  return (rv);
}

bool MkDir(const std::string& dir)
{
  AMJU_CALL_STACK;

#ifdef DIRECTORY_DEBUG
  std::cout << "Creating directory " << dir.c_str() << "\n";
#endif

/*
#ifdef WIN32
  int r = _mkdir(dir.c_str()); // VC8: mkdir deprecated..
#else
  int r = mkdir(dir.c_str(), 0);

#ifndef GEKKO
  // Set permission: rwxr-xr-x
  chmod(dir.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
#endif  
  
#ifdef DIRECTORY_DEBUG
std::cout << "CHMOD: " << dir.c_str() << "\n";
#endif
#endif
*/

  // Handy function to give 'mkdir -p' effect 
  int r = mkpath(dir.c_str(), 0777);
 
/* 
#ifdef DIRECTORY_DEBUG
#ifdef MACOSX
  if (r != 0)
  {
    switch (r)
    {
    case EEXIST:
      std::cout << "Directory already exists.\n";
      break;
    case ENOSPC: // no space
      std::cout << "No space.\n";
      break;
    case EROFS:  // read only
      std::cout << "Read only.\n";
      break;
    case EACCES: // no permission
      std::cout << "No permission.\n";
      break;
    // TODO More error conditions 
    default:
      std::cout << "FAILED to create directory " << dir.c_str() 
        << " code: " << r << "\n"; 
    }
  }
#endif // MACOSX
#endif // DIRECTORY_DEBUG
*/

  return (r == 0);
}

#ifdef WIN32
bool ShGetFolderPath(uint32 pathType, std::string* pResult)
{
  AMJU_CALL_STACK;

  char path[MAX_PATH];
  // Function Sig for SHGetFolderPath
  typedef UINT (CALLBACK* FuncPtr)(HWND, int, HANDLE, DWORD, LPSTR);
  // Load library shell32.dll
  HINSTANCE hdll = LoadLibraryA("shell32.dll");
  if (hdll)
  {
#ifdef DIRECTORY_DEBUG
    std::cout << "Loaded shell32.dll\n";
#endif
    FuncPtr funcPtr = (FuncPtr)GetProcAddress(hdll, "SHGetFolderPathA");
    if (funcPtr)
    {
#ifdef DIRECTORY_DEBUG
      std::cout << "Got address for SHGetFolderPathA\n";
#endif
      HRESULT hr = (funcPtr)(0, pathType, 0, 0, path); 
      if (SUCCEEDED(hr))
      {
        *pResult = path;
        return true;
      }
    }
  }
  return false;
}
#endif

#ifdef MACOSX
// From https://developer.apple.com/carbon/tipsandtricks.html
Boolean AmIBundled()
{
  FSRef processRef;
  FSCatalogInfo processInfo;
  int isBundled;
  ProcessSerialNumber psn = {0, kCurrentProcess};

  GetProcessBundleLocation (&psn, &processRef);
  FSGetCatalogInfo (&processRef, kFSCatInfoNodeFlags, &processInfo, NULL, NULL, NULL);
  isBundled = processInfo.nodeFlags & kFSNodeIsDirectoryMask;

  return( isBundled );
}
#endif

std::string GetProcessDir()
{
#ifdef IPHONE
  std::cout << "Yikes, calling GetProcessDir\n";
  Assert(0);
#endif // IPHONE
  
#ifdef MACOSX
  // This works for exes which are not bundles!
  CFBundleRef mainBundle = CFBundleGetMainBundle();

  // Find the executable URL - this works even for non-bundles!
  CFURLRef fileUrl = CFBundleCopyExecutableURL(mainBundle);

  // Convert URL to path
  CFStringRef filePath = CFURLCopyFileSystemPath (fileUrl, kCFURLPOSIXPathStyle);

  // Convert (unicode ?) CFStringRef to a const char *
  static const int MAX_STR = 1024;
  char bytes[MAX_STR];

  std::string fileName;
  if (CFStringGetCString(filePath, bytes, MAX_STR, CFStringGetSystemEncoding()))
  {
    fileName = bytes;
  }
  else
  {
    // TODO
std::cout << "Unexpected: didn't get exe file name: " << bytes << "\n";
  }
  std::string root = Amju::GetFilePath(fileName); // strip exe name from end

  if (AmIBundled())
  {
#ifdef BUNDLE_CHECK_DEBUG
std::cout << "This really is an app bundle.\n";
#endif
    // This really is a bundle. So we want to go from <dir>/Bundle.app/Contents/MacOS/<exe> to <dir>
    //root += "/../../../";
    // This turned out to be a bad idea - better to append the ../../../ if required by client code.
  }
  else
  {
#ifdef BUNDLE_CHECK_DEBUG
std::cout << "This  is not an app bundle.\n";
#endif
  }

#ifdef PROCESS_DIR_DEBUG
std::cout << "GetProcessDir() result: " << root.c_str() << "\n";
#endif

  return root; 
#endif // MACOSX

#ifdef WIN32

  char fileName[MAX_PATH];

  if(!GetModuleFileNameA(0, fileName, MAX_PATH))
  {
    return "./";
  }
  std::string root = Amju::GetFilePath(fileName); // strip exe name from end

  return root;

#endif
}

std::string GetDataDir()
{
#ifdef WIN32
  std::string dataDir = GetProcessDir();
  dataDir += "/Data/";
  return dataDir;
#endif
  
#ifdef MACOSX
  std::string dataDir = GetProcessDir();
  dataDir += "/../Resources/Data/";
  return dataDir;
#endif
  
#ifdef IPHONE
  return iOSGetDataDir();
#endif
}
  
std::string GetDesktopDir()
{
#ifdef WIN32
  static bool first = true;
  static std::string s;
  if (first)
  {
    first = false;
    bool success = ShGetFolderPath(CSIDL_DESKTOPDIRECTORY, &s);
    if (!success)
    {
#ifdef DIRECTORY_DEBUG
      std::cout << "Failed to get SHGetFolderPathA\n";    
#endif
      s = "/";
    }
  }
  return s;
#endif

#ifdef MACOSX
  static std::string s;
  static bool first = true;
  if (first)
  {
    first = false;
 
    static const int PATH_SIZE=200;
    char path[PATH_SIZE];
    // Get the user's desktop directory.
    FSRef fsref;
    FSFindFolder(kOnAppropriateDisk, kCurrentUserFolderType, kDontCreateFolder, &fsref);
    FSRefMakePath(&fsref, (unsigned char*)path, PATH_SIZE);

    s = path;
    s += "/Desktop/";
 //   s += APPLICATION_NAME;
//    s += "/";
//    MkDir(s);
  }
  return s;
#endif

#ifdef GEKKO
  return "/";
#endif // GEKKO

#ifdef IPHONE
  std::cout << "Trying to get Desktop dir, help!\n";
  return "./";
#endif // IPHONE
}

std::string GetSaveDir(const std::string& appName)
{
  // Get directory for saving preferences, etc.
#ifdef GEKKO
  return File::GetRoot();
#endif

#ifdef IPHONE
  return iOSGetDataDir();
  //std::cout << "Save dir: " << File::GetRoot() << "\n";
  //return File::GetRoot();
#endif

  // Not in the application directory, so the app can be upgraded without losing
  // preferences etc.
  // NB This should be in a shared area, not specific to the user that 
  // installed the game.
#ifdef WIN32
#ifdef JUST_USE_DATA_DIR
  // Try this for now
  return File::GetRoot();
#else

  static bool first = true;
  static std::string s;
  if (first)
  {
    first = false;
    // NB *IS* specific to current user, hopefully will fix issues where
    //  we can't write to saved files
    bool success = ShGetFolderPath(CSIDL_LOCAL_APPDATA, &s);
    
    // This only works on Win2000+
    // TODO Try to load the function at run time. If this fails fall
    // back to a hack.

    if (success)
    {
      s += "\\";
      s += appName;
      s += "\\";
     
      MkDir(s);
    }
    else
    {
#ifdef DIRECTORY_DEBUG
      std::cout << "Failed to get SHGetFolderPathA\n";    
#endif
      // Use the Data directory. 
      s = File::GetRoot();
    }
  }
  return s;
#endif // !JUST_USE_DATA_DIR

#endif // WIN32

#ifdef MACOSX
  static std::string s;
  static bool first = true;
  if (first)
  {
    first = false;
 
    s = "/Users/Shared/";
    s += appName;
    s += "/";

    MkDir(s);
  }

  return s;
#endif
}

bool FileExists(const std::string& filename)
{
  AMJU_CALL_STACK;

  struct stat buf;
  bool exists = (stat(filename.c_str(), &buf) != -1);

#ifdef DIRECTORY_DEBUG
std::cout << "**FILE EXISTS: " << filename.c_str() << ": "
  << (exists ? "YES" : "NO" ) << "**\n";
#endif

  return exists; 
}

DeleteResult AmjuDeleteFile(const std::string& filename)
{
  AMJU_CALL_STACK;

  if (!FileExists(filename))
  {
    return AMJU_FILE_DOES_NOT_EXIST;
  }

#ifdef MSVC
  _unlink(filename.c_str()); // VC8: unlink deprecated..
#else
#ifndef GEKKO
  unlink(filename.c_str()); // VC8: unlink deprecated..
#endif // GEKKO
#endif

  if (FileExists(filename))
  {
    return AMJU_DELETE_FAILED;
  }
  return AMJU_DELETED_OK;
}

bool FileCopy(const std::string& srcDir, const std::string& destDir, const std::string& filename)
{
  std::string src = srcDir + "/" + filename;
  std::string dest = destDir + "/" + filename;

  // boost::filesystem::copy_file would be the thing to use here, if we were using Boost.

#if defined (WIN32)
  // Requires Windows XP
  return CopyFileA(src.c_str(), dest.c_str(), FALSE) == TRUE; // do copy even if dest already exists

/*
#elif defined (MACOSX)

  // Requires Mac OS X 10.5 
  copyfile_state_t s = copyfile_state_alloc();
  int ret = copyfile(src.c_str(), dest.c_str(), s, COPYFILE_ALL);
  copyfile_state_free(s);
*/

#else

  // Copy file even if dest already exists
  std::ifstream f1(src.c_str(), std::fstream::binary);
  std::ofstream f2(dest.c_str(), std::fstream::trunc | std::fstream::binary);
  f2 << f1.rdbuf(); 
  return true;

#endif
}

Time GetFileModifiedTime(const std::string& filename)
{
  struct stat buf;
  stat(filename.c_str(), &buf);
  return Time((unsigned int)buf.st_mtime);
}

}

