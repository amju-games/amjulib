#include <iostream>
#include <AmjuGLWindowInfo.h>
#include <Game.h>
#include <Font.h>
#include <Directory.h>
#include <File.h>
#include <FileImplGlue.h>
#include <GlueFileMem.h>
#include "GSVr.h"

#ifdef AMJU_IOS
#define GLUE_FILE "data-iphone.glue"
#endif

namespace Amju
{
// Create global variable window info 
Amju::AmjuGLWindowInfo w(640, 480, false, "Amju VR Test");

void ReportError(const std::string& s)
{
  std::cout << s << "\n";
}

void StartUpBeforeCreateWindow()
{
#if defined(AMJU_IOS)
  std::string dataDir = GetDataDir();
  std::cout << "Data Dir: " << dataDir << "\n";
  File::SetRoot(dataDir, "/");

  GlueFileMem* gfm = new GlueFileMem;
  if (FileImplGlue::OpenGlueFile(GLUE_FILE, gfm))
  {
    std::cout << "Opened glue file " << GLUE_FILE << "\n";
  }
  else
  {
    ReportError("Failed to open data glue file");
  }
#endif
}

void StartUpAfterCreateWindow()
{
  TheGame::Instance()->SetCurrentState(TheGSVr::Instance());

  //TheResourceManager::Instance()->AddLoader("font", FontLoader);
  //Font* font = (Font*)TheResourceManager::Instance()->GetRes("font2d/arial-font.font");
  //Assert(font);
  //TheGame::Instance()->SetFrameTimeFont(font);
}
}

