#include "GSCopyAssets.h"
#include <AmjuGL.h>
#include <Directory.h>
#include <Game.h>
#include "GSTitle.h"

namespace Amju
{
std::string GetDataDir()
{
  std::string dataDir = GetProcessDir();

#ifdef WIN32
  dataDir += "/Data";
#endif

#ifdef MACOSX
  dataDir += "/My Game.app/Contents/Resources/Data/";
#endif

std::cout << "GetDataDir returning: " << dataDir << "\n";

  return dataDir;
}

// Check if file exists in Save Dir. If not, copies it from Data dir.
bool CopyFileIfMissing(const std::string& filename, const std::string& srcDir, const std::string& destDir)
{
  std::string s = destDir + "/" + filename;
  if (FileExists(s))
  {
    std::cout << "File already exists: " << s << "\n";
  }
  else
  {
    std::cout << "Copying file " << filename << "\n";
    FileCopy(srcDir, destDir, filename); 
  }

  return true;
}

bool DoCopyingForDir(const std::string& srcDir, const std::string& destDir)
{
  std::vector<std::string> files;
  Dir(srcDir, &files, false);
 
  // Check all files, copying if necessary.
  for (unsigned int i = 0; i < files.size(); i++)
  {
//std::cout << files[i] << "\n";

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

    DoCopyingForDir(dataDir, saveDir);

    // Create subdirs for font2d and font3d
    MkDir(saveDir + "/font2d");
    MkDir(saveDir + "/font3d");

    // Copy assets from data dir to save dir for font2d and font3d.
    DoCopyingForDir(dataDir + "/font2d/", saveDir + "/font2d/");
    DoCopyingForDir(dataDir + "/font3d/", saveDir + "/font3d/");

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

//  GSGui::OnActive();
//  m_gui = 
  //LoadGui("TODO");
//  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
}

} // namespace
