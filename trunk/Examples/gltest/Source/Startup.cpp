#include <iostream>
#include <AmjuGLWindowInfo.h>
#include <CursorManager.h>
#include <Game.h>
#include <Font.h>
#include <Directory.h>
#include <File.h>
#include <FileImplGlue.h>
#include <GlueFileMem.h>
#include <ObjMesh.h>
#include "GSLighting.h"
#include "GSTexture.h"
#include "GSShaderWave.h"
#include "GSShadow.h"
#include "GSEnvMap.h"
#include "GSObb2Test.h"
#include "GSDepthOfField.h"
#include "GSMandel.h"
#include "GSTerrain.h"
#include "GSPatchwork.h"
#include "GSInitialise.h"

#ifdef AMJU_IOS
#define GLUE_FILE "data-iphone.glue"
#endif

namespace Amju
{
// Create global variable window info 
Amju::AmjuGLWindowInfo w(640, 480, false, "AmjuGL Test");

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
    
#if defined(XCODE) && defined(MACOSX)
  // TODO XCode only
  std::string dataDir = GetDataDir();
  dataDir += "../Assets/";
  dataDir = CleanPath(dataDir);
  std::cout << "Data Dir: " << dataDir << "\n";
  File::SetRoot(dataDir, "/");
#endif
}

void StartUpAfterCreateWindow()
{
  TheGame::Instance()->SetCurrentState(TheGSInitialise::Instance());
    
//  TheGame::Instance()->SetCurrentState(TheGSPatchwork::Instance());
//  TheGame::Instance()->SetCurrentState(TheGSBarrel::Instance());
//  TheGame::Instance()->SetCurrentState(TheGSTerrain::Instance());
//  TheGame::Instance()->SetCurrentState(TheGSMandel::Instance());
//  TheGame::Instance()->SetCurrentState(TheGSShaderWave::Instance());
//  TheGame::Instance()->SetCurrentState(TheGSLighting::Instance());

  ResourceManager* rm = TheResourceManager::Instance();
  rm->AddLoader("obj", TextObjLoader);
  rm->AddLoader("font", FontLoader);

  Font* font = (Font*)TheResourceManager::Instance()->GetRes("font2d/arial-font.font");
  Assert(font);
  TheGame::Instance()->SetFrameTimeFont(font);

  TheCursorManager::Instance()->Load(Vec2f(0.025f, -0.08f));

  TheGame::Instance()->Update();
}
}

