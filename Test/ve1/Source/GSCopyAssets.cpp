#include <AmjuGL.h>
#include <Directory.h>
#include <Game.h>
#include <AmjuTime.h>
#include <Pause.h>
#include "GSCopyAssets.h"
#include "GSTitle.h"
#include "GSObjMesh.h" // Todo temp test

#if defined(WIN32) && defined(_DEBUG)
//#define WIN32_TEST_COPY_ASSETS
#endif

//#define FILECOPY_DEBUG

namespace Amju
{
std::string GetDataDir()
{
#ifdef WIN32_TEST_COPY_ASSETS
  // When developing, we don't want to use the real process dir.
  // TODO How do we know if this is a 32-bit or 64-bit machine ? 
  std::string dataDir = "c:/program files (x86)/my game";  // on 64-bit machine
  //std::string dataDir = "c:/program files/my game";  // on 32-bit machine
#else
  std::string dataDir = GetProcessDir();
#endif

#ifdef WIN32
  dataDir += "/Data/";
#endif

#ifdef MACOSX
  dataDir += "/../Resources/Data/";
#endif

//std::cout << "GetDataDir returning: " << dataDir << "\n";

  return dataDir;
}

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
std::cout << "Copying files to Save Dir as required...\n";

    static std::string dataDir = GetDataDir();
    static std::string saveDir = File::GetRoot();

std::cout << "Data Dir: " << dataDir << "\nSave Dir: " << saveDir << "\n";

    RecursiveCopy(dataDir, saveDir); // because characters are in subdirs

    // All copied - go to next state 
    // TODO Once logo displayed etc
// TODO TEMP TEST
    //TheGame::Instance()->SetCurrentState(TheGSTitle::Instance());
    TheGame::Instance()->SetCurrentState(TheGSObjMesh::Instance());
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
