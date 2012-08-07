#include "GSCopyAssets.h"
#include <AmjuGL.h>
#include <Directory.h>
#include <Game.h>
#include <AmjuTime.h>
#include "GSTitle.h"

#if defined(WIN32) && defined(_DEBUG)
//#define WIN32_TEST_COPY_ASSETS
#endif

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

std::cout << "GetDataDir returning: " << dataDir << "\n";

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
//std::cout << "File: " << filename << " destTime: " << destTime.ToString() << " srcTime: " << srcTime.ToString() << "\n";
std::cout << "Copying file " << filename << " as dest is older than src, or doesn't exist.\n";
    if (FileCopy(srcDir, destDir, filename))
    {
      std::cout << "..FileCopy was OK!\n";
    }
    else
    {
      std::cout << "..FileCopy FAILED!\n";
    }
  }
  else
  {
//std::cout << "No need to copy file " << filename << ", it's up to date.\n";
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
  std::vector<std::string> files;
  Dir(srcDir, &files, false);
 
  // Check all files, copying if necessary.
  for (unsigned int i = 0; i < files.size(); i++)
  {
//std::cout << "In " << srcDir << ": " << files[i] << "\n";

    CopyFileIfMissing(files[i], srcDir, destDir);
  }

  return true;
}

GSCopyAssets::GSCopyAssets()
{
}

void GSCopyAssets::Update()
{
  GSGui::Update();

  static bool done = false;
  if (!done)
  {
    done = true;

std::cout << "Copying files to Save Dir as required...\n";

    static std::string dataDir = GetDataDir();
    static std::string saveDir = File::GetRoot();

std::cout << "Data Dir: " << dataDir << "\nSave Dir: " << saveDir << "\n";

    MkDir(saveDir);
    // Create subdirs for font2d and font3d - do this before copying, to avoid problem.
    // TODO Fix Dir() so it distinguishes between files and dirs!!
    MkDir(saveDir + "/font2d");
    MkDir(saveDir + "/font3d");
    MkDir(saveDir + "/Sound");

    DoCopyingForDir(dataDir, saveDir);

    // Copy assets from data dir to save dir for font2d and font3d.
    DoCopyingForDir(dataDir + "/font2d/", saveDir + "/font2d/");
    DoCopyingForDir(dataDir + "/font3d/", saveDir + "/font3d/");
    DoCopyingForDir(dataDir + "/Sound/", saveDir + "/Sound/");

    // All copied - go to next state 
    // TODO Once logo displayed etc
    TheGame::Instance()->SetCurrentState(TheGSTitle::Instance());
  }
}

void GSCopyAssets::Draw()
{
//  GSGui::Draw();

}

void GSCopyAssets::Draw2d()
{
//  GSGui::Draw2d();
}

void GSCopyAssets::OnActive()
{
  // Before loading anything, copy the files necessary to show logo or 'please wait' indicator.

}

} // namespace
