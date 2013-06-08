#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <StringUtils.h>
#include <Directory.h>
#include <GlueFile.h>
#include <Game.h>
#include <AmjuTime.h>
#include <Pause.h>
#include <CursorManager.h>
#include "GSCopyAssets.h"
#include "GSLogo.h"
#include "GSTitle.h"
#include <AmjuFinal.h>

#if defined(WIN32) && defined(_DEBUG)
//#define WIN32_TEST_COPY_ASSETS
#endif

//#define FILECOPY_DEBUG

#ifdef MACOSX
static const char* GLUE_FILE_NAME = "data-Mac.glue";
#endif

#ifdef IPHONE
static const char* GLUE_FILE_NAME = "data-iphone.glue";
#endif

#ifdef WIN32
static const char* GLUE_FILE_NAME = "data-win.glue";
#endif

namespace Amju
{
// Check if file exists in Save Dir. If not, copies it from Data dir.
bool CopyFileIfMissing(const std::string& filename, const std::string& srcDir, const std::string& destDir)
{
  bool doCopy = true;
  std::string dest = destDir + "/" + filename;

  if (FileExists(dest))
  {
//std::cout << "File already exists: " << dest << "\n";

    Time destTime = GetFileModifiedTime(dest);

    std::string src = srcDir + "/" + filename;
    Time srcTime = GetFileModifiedTime(src);

    if (!(destTime < srcTime))
    {
      doCopy = false;
    }
  }

  if (doCopy)
  {
#ifdef FILECOPY_DEBUG
std::cout << "Copying file " << filename << " as dest is older than src, or doesn't exist.\n";
#endif

    if (FileCopy(srcDir, destDir, filename))
    {
#ifdef FILECOPY_DEBUG
      std::cout << "..FileCopy was OK!\n";
#endif
    }
    else
    {
      // Always show for diagnosing crashes etc
      std::cout << "FileCopy FAILED: srcDir: " << srcDir << " destDir: " << destDir << " file: " << filename << "\n";
    }
  }
  else
  {
#ifdef FILECOPY_DEBUG2
std::cout << "No need to copy file " << filename << ", it's up to date.\n";
#endif
  }

/*
  if (FileExists(dest))
  {
    std::cout << "File already exists: " << s << "\n";
  }
  else
  {
    std::cout << "Copying file " << filename << "\n";
    if (FileCopy(srcDir, destDir, filename))
    {
      std::cout << "..FileCopy was OK!\n";
    }
    else
    {
      std::cout << "..FileCopy FAILED!\n";
    }
  }
*/

  return true;
}

bool DoCopyingForDir(const std::string& srcDir, const std::string& destDir)
{
  DirEnts des;
  Dir(srcDir, &des, false);
 
  // Check all files, copying if necessary.
  for (unsigned int i = 0; i < des.size(); i++)
  {
//std::cout << "In " << srcDir << ": " << files[i] << "\n";
    DirEnt& de = des[i];
    if (!de.m_isDir)
    {
      CopyFileIfMissing(de.m_name, srcDir, destDir);
    }
  }

  return true;
}

bool CopyFromGlueFile(const std::string& srcGlueFilePath, const std::string& destDir)
{
  std::string oldRoot = File::GetRoot();
  File::SetRoot("", "/");
  GlueFile gf;
  bool loaded = gf.OpenGlueFile(srcGlueFilePath, false);
  File::SetRoot(oldRoot, "/");

  if (!loaded)
  {
    std::cout << "Failed to open glue file " << srcGlueFilePath << "\n";
    Assert(0);
    return false;
  }
  Time glueTime = GetFileModifiedTime(srcGlueFilePath);

  Strings strs;
  gf.Dir(&strs);
  int size = strs.size();
  for (int i = 0; i < size; i++)
  {
    const std::string& subfile = strs[i];
#ifdef FILECOPY_DEBUG
    std::cout << "Copying file " << subfile << " from glue file...\n";
#endif

    std::string outFileName = destDir + subfile;
    // Check if we should copy file - we should if it does not exist, or
    //  timestamp of glue file is more recent than the file.
    bool doCopy = true;

    if (FileExists(outFileName))
    {
#ifdef FILECOPY_DEBUG
std::cout << "File already exists: " << outFileName << "\n";
#endif
    Time destTime = GetFileModifiedTime(outFileName);

    if (!(destTime < glueTime))
    {
#ifdef FILECOPY_DEBUG
std::cout << "  Don't copy, it's more recent.\n";
#endif
      doCopy = false; // more recent copy already there
    }
  }


    unsigned int seekbase = 0;
    if (!gf.GetSeekBase(subfile, &seekbase))
    {
      std::cout << "Failed to find subfile " << subfile << " in glue file, but was listed in Dir!!\n";
      Assert(0);
      return false;
    }
    uint32 size = gf.GetSize(subfile);
    unsigned char* buf = new unsigned char[size + 1];
    gf.GetBinary(seekbase, size, buf);

    File outFile(false); // no version info
    std::string outFileDir = GetFilePath(outFileName);
    if (!MkDir(outFileDir))
    {
      std::cout << "MkDir failed: " << outFileDir << "\n";
      Assert(0);
      return false;
    }
    if (!outFile.OpenWrite(outFileName, 0, true, false, true))
    {
      std::cout << "Failed to open file " << outFileName << " for writing.\n";
      Assert(0);
      return false;
    }
    if (!outFile.WriteBinary((char*)buf, size))
    {
      std::cout << "Failed to write binary data to " << outFileName << ", size: " << size << "\n";
      Assert(0);
      return false;
    }
    delete [] buf; // whoops
  }
  return true;
}

void RecursiveCopy(const std::string& srcDir, const std::string& destDir)
{
#ifdef FILECOPY_DEBUG
std::cout << "Recursively copying " << srcDir << " to " << destDir << "\n";
std::cout << "Making dir " << destDir << "\n";
#endif

  MkDir(destDir);

  // List contents of src dir
  DirEnts des;
  Dir(srcDir, &des, false);
 
  for (unsigned int i = 0; i < des.size(); i++)
  {
    DirEnt& de = des[i];

#ifdef FILECOPY_DEBUG
std::cout << "In " << srcDir << ": " << " found: " << de.m_name << (de.m_isDir ? " (DIR)" : "") << "\n";
#endif

    if (de.m_name[0] == '.') // name starts with a dot ? 
    {
#ifdef FILECOPY_DEBUG
std::cout << "...skipping this dir.\n";
#endif

      continue;
    }

    if (de.m_isDir)
    {
#ifdef FILECOPY_DEBUG
std::cout << "So should recusively copy...\n";
#endif

      std::string dir = "/" + de.m_name + "/";
      RecursiveCopy(srcDir + dir, destDir + dir);
    }
    else
    {
      CopyFileIfMissing(de.m_name, srcDir, destDir);
    }
  }
}

GSCopyAssets::GSCopyAssets()
{
}

void GSCopyAssets::Update()
{
  // Counter so we draw window before copying
  static int done = 0;
  done++;
  if (done == 2)
  {
//#ifndef IPHONE
    std::cout << "Copying files to Save Dir as required...\n";
      
    static std::string dataDir = GetDataDir();

    // This is OK as we set File root to the Save Dir in LoadConfig()
    static std::string saveDir = File::GetRoot();

    std::string glueFilePath = dataDir + GLUE_FILE_NAME; 

std::cout << "Data Dir: " << dataDir << "\nSave Dir: " << saveDir << "\n";
std::cout << "Glue file + path: " << glueFilePath << "\n";

    //RecursiveCopy(dataDir, saveDir); // because characters are in subdirs
    CopyFromGlueFile(glueFilePath, saveDir);
//#endif // IPHONE
      
    // All done, now we can start loading stuff
    TheCursorManager::Instance()->Load(Vec2f(0.025f, -0.08f)); // Hotspot, yuck

    // All copied - go to next state 
#ifdef _DEBUG
  TheGame::Instance()->SetCurrentState(TheGSTitle::Instance());
#else
    TheGame::Instance()->SetCurrentState(TheGSLogo::Instance()); 
#endif
  }
}

void GSCopyAssets::Draw()
{
}

void GSCopyAssets::Draw2d()
{
}

void GSCopyAssets::OnActive()
{
  // Before loading anything, copy the files necessary to show logo or 'please wait' indicator.
  AmjuGL::SetClearColour(Colour(0, 0, 0, 1));
}

} // namespace
