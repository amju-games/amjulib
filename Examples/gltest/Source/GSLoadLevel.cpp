#include <AmjuGL.h>
#include <Game.h>
#include <GameObjectFactory.h>
#include "GSLoadLevel.h"
#include "MySceneGraph.h"

namespace Amju
{
GSLoadLevel::GSLoadLevel()
{
  m_loadedOk = false;
  m_numLoaded = 0;
  m_numObjects = 0;
  m_maxBarX = 0;
}

std::string GSLoadLevel::GetLastLoadedLevel() const
{
  return m_levelFilename;
}

bool GSLoadLevel::LoadOneObject()
{
  std::string s;
  if (!m_file->GetDataLine(&s))
  {
    m_file->ReportError("Expected game object type");
    return false;
  }

//std::cout << "Got game object type: '" << s << "'\n";

  auto gof = TheGameObjectFactory::Instance();

  RCPtr<GameObject> go = gof->Create(s);
  if (!go)
  {
    m_file->ReportError("Can't create game object: " + s);

    std::vector<std::string> strs = gof->GetTypeNames();
    for (auto t : strs)
      std::cout << "Factory has this type: " << t << "\n";

    return false;
  }

  if (!go->Load(m_file))
  {
    m_file->ReportError("Failed to load game object: " + s);
    return false;
  }

  TheGame::Instance()->AddGameObject(go);

  return true;
}

void GSLoadLevel::Update()
{
  GSBase::Update();

  // Load object
  LoadOneObject();

  m_numLoaded++;

  // Update progress bar
  GuiElement* bar = m_gui->GetElementByName("bar");
  Vec2f s = bar->GetSize();
  float barSize = (float)m_numLoaded / (float)m_numObjects;
  if (barSize > 1.0f)
  {
    barSize = 1.0f;
  }
  s.x = barSize * m_maxBarX;
  bar->SetSize(s);

  // Go back to prev state when done; set loaded flag.
  if (m_numLoaded >= m_numObjects)
  {
    GoBack();
  }
}

void GSLoadLevel::DrawScene2d()
{
  // Draw progress bar GUI
  m_gui->Draw();
}

void GSLoadLevel::DrawScene()
{
  // Noting to draw
}

void GSLoadLevel::CreateTweakMenu()
{
  //GSBase::CreateTweakMenu();
  // No tweak menu
}

void GSLoadLevel::OnActive()
{
  GSBase::OnActive();

  m_loadedOk = false;
  m_numLoaded = 0;

  // Load gui
  m_gui = LoadGui("gui-load-level.txt");
  Assert(m_gui);
  GuiElement* bar = m_gui->GetElementByName("bar");
  m_maxBarX = bar->GetSize().x;

  // Clear game objects
  TheGame::Instance()->ClearGameObjects();

  // Create a root for the game scene graph
  ResetSceneGraph(); // convenience func in MySceneGraph

  // Open file
  bool HAS_VERSION_INFO = true;
  bool NOT_BINARY = false;
  bool useRoot = false;
 
  // On Mac and PC, level files are OS files so we can edit them
#if defined(WIN32) || defined(MACOSX)
  File::Impl impl = File::STD;
#else
  File::Impl impl = File::GLUE;
#endif

  // Number of game objects
  m_file = new File(HAS_VERSION_INFO, impl);

  if (!m_file->OpenRead(m_levelFilename, NOT_BINARY, useRoot))
  {
    Assert(0);
    return;
  }

  if (!m_file->GetInteger(&m_numObjects))
  {
    m_file->ReportError("Expected number of objects");
    Assert(0);
    return;
  }
}

bool GSLoadLevel::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSLoadLevel::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
