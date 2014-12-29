#include <iostream>
#include <Game.h>
#include <Timer.h>
#include <AmjuGL.h>
#include <GuiText.h>
#include <GuiListBox.h>
#include <StringUtils.h>
#include <ObjMesh.h>
#include <StereoDraw.h>
#include "GSBase.h"
#include "OvrHeadTracker.h"
#include "StateList.h"
#include "Tweakable.h"

namespace Amju
{
static const Vec3f ORIG_VIEW_DIR(0, 0, -1);
static const Vec3f ORIG_UP_DIR(0, 1, 0);

static ChooserDialog chooser;

static const std::string KEYS = "[P]ause [S]kip [R]eset ";

// Callback when choice made
static void OnChooserFinished(Dialog* dlg)
{
  chooser.Choose();
}

void ChooserDialog::Choose()
{
  GuiElement* elem = m_gui->GetElementByName("choose-list-box");
  Assert(elem);
  GuiListBox* listbox = dynamic_cast<GuiListBox*>(elem);
  Assert(listbox);
  GuiList* list = listbox->GetList();

  int i = list->GetSelectedItem();
 
  std::cout << "Selected item: " << i << "\n"; 
  GSBase* gs = GetState(i);
  TheGame::Instance()->SetCurrentState(gs);
}

void ChooserDialog::Add(const std::string& choice)
{
  GuiElement* elem = m_gui->GetElementByName("choose-list-box");
  Assert(elem);
  GuiListBox* listbox = dynamic_cast<GuiListBox*>(elem);
  Assert(listbox);
  GuiList* list = listbox->GetList();

  GuiText* text = new GuiText;
  text->SetText(choice);
  list->AddItem(text); 
}

void ChooserDialog::Populate()
{
  const StateList& statelist = GetStateList();
  for (auto it = statelist.begin(); it != statelist.end(); ++it)
  { 
    Add(it->first);
  }
}

void DrawCurrentState()
{
  GameState* s = TheGame::Instance()->GetState();
  GSBase* b = dynamic_cast<GSBase*>(s);
  
  if (b)
  {
    b->DrawScene();
  }
}

GSBase::GSBase() : m_time(0), m_maxTime(5.0f), m_paused(false)
{
}

void GSBase::CreateTweakMenu()
{
  m_dlg = dynamic_cast<GuiDialog*>(LoadGui("gui-dialog.txt").GetPtr());
  Assert(m_dlg);
  m_dlg->SetTitle(m_name);
}

void GSBase::OnActive() 
{
  m_time = 0;

  SetUpTweakMenu();

  chooser.SetGuiFilename("gui-choose-dialog.txt");
  chooser.SetTitle("Choose scene");
  chooser.Populate();
  chooser.SetFinishCallback(OnChooserFinished);
 
  m_camera.m_pos = Vec3f(0, 5, 10); 
  m_camera.m_dir = ORIG_VIEW_DIR;
  m_camera.m_up = ORIG_UP_DIR;
}

bool GSBase::OnKeyEvent(const KeyEvent& ke)
{
  if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR && ke.key == 'c')
  {
    std::cout << "Choose scene menu\n";

    DoModalDialog(&chooser);
  }
  else if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR && ke.key == 'r')
  {
    std::cout << "Reset!\n";
    OnActive();
  }
  else if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR && ke.key == 'p')
  {
    m_paused = !m_paused;
  }
  else if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR && ke.key == 's')
  {
    m_time = m_maxTime;
  }
  return false;
}
 
void GSBase::Draw2d()
{
  static GuiText s;
  s.SetJust(GuiText::AMJU_JUST_LEFT);
  s.SetFgCol(Colour(0.5f, 0.5f, 0.5f, 1));
  s.SetBgCol(Colour(0, 0, 0, 1));
//  s.SetDrawBg(true);
  s.SetLocalPos(Vec2f(-1, 0.9f));
  s.SetSize(Vec2f(2, 0.1f));
  s.SetText(m_name + " " + KEYS);
  s.Draw();

  if (m_dlg)
  {
    m_dlg->Draw();
  }
}

static int depth = 0;
void GSBase::Draw()
{
  Assert(depth == 0);
  depth++;

  static StereoDraw sd; // TODO one per state?
  sd.SetCamera(m_camera);
  sd.SetIsStereo(true);
  sd.SetDrawFunc(DrawCurrentState);
  sd.Draw();

  depth--;
}

void GSBase::DrawHelp()
{
  m_camera.Draw();

/*
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  const float FOVY = 60.0f;
  const float NEAR = 1.0f;
  const float FAR = 3000.0f;
  float aspect = 1.3f;
  AmjuGL::SetPerspectiveProjection(FOVY, aspect, NEAR, FAR);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();

  AmjuGL::LookAt(0, 5, 10,  0, 0, 0,  0, 1, 0);
*/
}

void GSBase::Update()
{
  static OvrHeadTracker ht;
  static bool first = true;
  if (first)
  {
    first = false;
    ht.Init();
  }
  Quaternion q;
  if (ht.Update(&q))
  {
    m_camera.m_dir = q.RotateVec(ORIG_VIEW_DIR);
    m_camera.m_up = q.RotateVec(ORIG_UP_DIR);
  }

  if (m_paused)
  {
    return;
  }

  float dt = TheTimer::Instance()->GetDt();
  m_time += dt;

  /* Now we have a menu we don't need to do this
     But it could be good for a "play all" option
  if (m_time >= m_maxTime)
  {
    m_time = 0;
    if (!m_nextState)
    {
      std::cout << "Oops, no next state set\n";
      Assert(0);
    }
    TheGame::Instance()->SetCurrentState(m_nextState);
  }
  */
}

void DrawBunnyScene()
{
  static ObjMesh* bunny = 
    (ObjMesh*)TheResourceManager::Instance()->GetRes("bunny_with_normals.obj");

  AmjuGL::UseShader(0);

  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D); // not textured
  AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);
  Vec3f pos(1, 1, 1);
  AmjuGL::DrawLighting(
    AmjuGL::LightColour(0, 0, 0),
    AmjuGL::LightColour(0.2f, 0.2f, 0.2f), // Ambient light colour
    AmjuGL::LightColour(1, 1, 1), // Diffuse light colour
    AmjuGL::LightColour(1, 1, 1),
    AmjuGL::Vec3(pos.x, pos.y, pos.z)); // Light direction
 
/* 
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  const float FOVY = 60.0f;
  const float NEAR = 6.0f;
  const float FAR = 20.0f;
  float aspect = 1.3f;
  AmjuGL::SetPerspectiveProjection(FOVY, aspect, NEAR, FAR);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();
  AmjuGL::LookAt(3, 5, 12,  3, 0, 0,  0, 1, 0);
*/

  float d = 3.0f;

  AmjuGL::SetColour(Colour(0.2, 0.2, 1, 1));
  bunny->Draw();

  AmjuGL::SetColour(Colour(1, 0, 0, 1));
  AmjuGL::PushMatrix();
  AmjuGL::Translate(d, 0, d);
  bunny->Draw();

  AmjuGL::SetColour(Colour(1, 0, 1, 1));
  AmjuGL::Translate(d, 0, d);
  bunny->Draw();
  AmjuGL::PopMatrix();

  AmjuGL::SetColour(Colour(0, 1, 0, 1));
  AmjuGL::PushMatrix();
  AmjuGL::Translate(-d, 0, -d);
  bunny->Draw();
  AmjuGL::SetColour(Colour(1, 1, 0, 1));
  AmjuGL::Translate(-d, 0, -d);
  bunny->Draw();
  AmjuGL::PopMatrix();
}
}
