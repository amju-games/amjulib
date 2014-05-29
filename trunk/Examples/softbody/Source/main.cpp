#ifndef AMJU_IOS

#define AMJU_USE_OPENGL
#define AMJU_USE_GLUT

#include <main.h>
#include <AmjuGLWindowInfo.h>
#include <Game.h>
#include <ResourceManager.h>
#include <ObjMesh.h>
#include <Font.h>
#include "GSSoftBody.h"

namespace Amju
{
// Create global variable window info 
Amju::AmjuGLWindowInfo w(640, 480, false, "Soft body test");

void StartUpBeforeCreateWindow()
{
#if defined(AMJU_IOS)
  const char* GLUE_FILE = "data-iphone.glue";

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
  ResourceManager* rm = TheResourceManager::Instance();
  rm->AddLoader("obj", TextObjLoader);

  Font* font = (Font*)TheResourceManager::Instance()->GetRes("font2d/arial-font.font");
  Assert(font);
  Game* game = TheGame::Instance();
  game->SetFrameTimeFont(font);

  game->SetCurrentState(TheGSSoftBody::Instance());
}
}

#endif // IPHONE

