#include <iostream>
#include <AmjuGL.h>
#include <CursorManager.h>
#include <DegRad.h>
#include <EventPoller.h>
#include <Game.h>
#include <GuiButton.h>
#include <GuiText.h>
#include <GuiListBox.h>
#include <ObjMesh.h>
#include <Quaternion.h>
#include <StringUtils.h>
#include <StereoDraw.h>
#include <Timer.h>
#include "GSBase.h"
#include "StateList.h"
#include "Tweakable.h"

#if defined(WIN32) || defined(MACOSX)
#include "MouseHeadTracker.h"
#include "OvrHeadTracker.h"
#endif


namespace Amju
{
// Set to true if we detect OVR. Also we want to set to true if in Cardboard mode,
//  or we just want to test.
static bool isStereo = true;

static Vec3f ORIG_VIEW_DIR(0, 0, -1);
static Vec3f ORIG_UP_DIR(0, 1, 0);

#if defined(WIN32) || defined(MACOSX)
static RCPtr<MouseHeadTracker> mht = new MouseHeadTracker; // on heap because it's an event listener
#endif
    
static ChooserDialog chooser;

static const std::string KEYS = "[P]ause [S]kip [R]eset ";

static void OnChoose(GuiElement*)
{
  GameState* s = TheGame::Instance()->GetState();
  GSBase* b = dynamic_cast<GSBase*>(s);
  if (b)
  {
    b->OnChooseButton();
  }
}

static void OnTweak(GuiElement*)
{
  GameState* s = TheGame::Instance()->GetState();
  GSBase* b = dynamic_cast<GSBase*>(s);
  if (b)
  {
    b->OnTweakButton();
  }
}

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
    AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);
    b->DrawScene();

    AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
    AmjuGL::SetIdentity();
    AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
    AmjuGL::SetIdentity();
    AmjuGL::UseShader(nullptr);
    AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);
    AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
    AmjuGL::Enable(AmjuGL::AMJU_BLEND);

    b->DrawScene2d();
  }
}

GSBase::GSBase() : m_time(0), m_maxTime(5.0f), m_paused(false)
{
  m_mouseLook = true;
  m_showTweak = false;
  m_showChoose = false;
}

void GSBase::CreateTweakMenu()
{
  m_tweaker = dynamic_cast<GuiDialog*>(LoadGui("gui-dialog.txt").GetPtr());
  Assert(m_tweaker);
  m_tweaker->SetTitle(m_name);
}

void GSBase::OnTweakButton()
{
  m_showTweak = !m_showTweak;
}

void GSBase::OnChooseButton()
{
  m_showChoose = !m_showChoose;
}

void GSBase::OnDeactive()
{
  TheEventPoller::Instance()->RemoveListener(m_menuButtons);
  m_menuButtons = nullptr;

  TheEventPoller::Instance()->RemoveListener(m_chooser);
  m_chooser = nullptr;
}

void GSBase::OnActive() 
{
  m_time = 0;
  m_showTweak = false;
  m_showChoose = false;

  m_menuButtons = LoadGui("gui-menu-buttons.txt");
  Assert(m_menuButtons);
  GuiButton* choose = (GuiButton*)m_menuButtons->GetElementByName("choose-button");
  choose->SetCommand(OnChoose);
  GuiButton* tweak = (GuiButton*)m_menuButtons->GetElementByName("tweak-button");
  tweak->SetCommand(OnTweak);

  SetUpTweakMenu();

  m_chooser = dynamic_cast<GuiDialog*>(LoadGui("gui-choose-dialog.txt").GetPtr());
  Assert(m_chooser);
//  chooser.SetGuiFilename("gui-choose-dialog.txt");
  m_chooser->SetTitle("Choose scene");
//  Populate(m_chooser);
  chooser.SetFinishCallback(OnChooserFinished);
 
  m_camera.m_pos = Vec3f(0, 5, 10); 
  ORIG_VIEW_DIR = Normalise(-m_camera.m_pos);
  m_camera.m_dir = ORIG_VIEW_DIR;
  //ORIG_UP_DIR = Normalise(CrossProduct(ORIG_VIEW_DIR, Vec3f(0, 1, 0)));
  m_camera.m_up = ORIG_UP_DIR;

#if defined(WIN32) || defined(MACOSX)
  mht->Reset();
#endif

  // Set up GUI text: name and description etc.
  for (int i = 0; i < 2; i++)
  {
    m_guiText[i].SetFgCol(Colour(1, 1, 1, 1));
    m_guiText[i].SetBgCol(Colour(0, 0, 0, 0.7f));
    m_guiText[i].SetDrawBg(true);
    m_guiText[i].SetLocalPos(Vec2f(-1, 0.9f - 0.1f * (float)i));
    m_guiText[i].SetJust(GuiText::AMJU_JUST_CENTRE);
  }
  m_guiText[0].SetSize(Vec2f(2, 0.1f));
  m_guiText[0].SetText(m_name + " " + KEYS);
  m_guiText[1].SetIsMulti(true);
  m_guiText[1].SetSize(Vec2f(2, 0.21f));
  m_guiText[1].SetFontSize(0.7f);
  m_guiText[1].SetText(m_description);
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
 
void GSBase::DrawScene2d()
{
  for (int i = 0; i < 2; i++)
  {
    m_guiText[i].Draw();
  }

  if (m_showTweak && m_tweaker)
  {
    m_tweaker->Draw();
  }

  if (m_showChoose && m_chooser)
  {
    m_chooser->Draw();
  }

  m_menuButtons->Draw();

  TheCursorManager::Instance()->Draw();
}

static int depth = 0;
void GSBase::Draw()
{
  Assert(depth == 0);
  depth++;

  static StereoDraw sd; // TODO one per state?
  sd.SetCamera(m_camera);
  sd.SetIsStereo(isStereo);
  sd.SetDrawFunc(DrawCurrentState);
  sd.Draw();

  depth--;
}

void GSBase::Draw2d()
{
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

bool GSBase::OnRotationEvent(const RotationEvent& re)
{
    // TODO this sucks - much better if we had another kind of RotationEvent
    //  containing a quaternion.
    
    static Quaternion q[3];
    switch (re.axis)
    {
    case  AMJU_AXIS_X:
        // Concat a rotation about the x axis
        q[0] = Quaternion(Vec3f(1, 0, 0), DegToRad(re.degs));
        break;
            
    case  AMJU_AXIS_Y:
        // Concat a rotation about the x axis
        q[1] = Quaternion(Vec3f(0, 1, 0), DegToRad(re.degs));
        break;

    case  AMJU_AXIS_Z:
        // Concat a rotation about the x axis
        q[2] = Quaternion(Vec3f(0, 0, 1), DegToRad(re.degs));
        break;
    }
    
    Quaternion combined = q[0] * q[1] * q[2];
    m_camera.m_dir = combined.RotateVec(ORIG_VIEW_DIR);
    m_camera.m_up = combined.RotateVec(ORIG_UP_DIR);

    return false; // not consumed?
}
    
void GSBase::Update()
{
#if defined(WIN32) || defined(MACOSX)
  static OvrHeadTracker ht;
  static bool first = true;
  if (first)
  {
    first = false;
    if (ht.Init())
    {
std::cout << "Found OVR, setting stereo on.\n";
      isStereo = true;
    }
    mht->Init();
  }
  Quaternion q;
  if (ht.Update(&q))
  {
    m_camera.m_dir = q.RotateVec(ORIG_VIEW_DIR);
    m_camera.m_up = q.RotateVec(ORIG_UP_DIR);
  }
  else if (m_mouseLook && mht->Update(&q))
  {
    m_camera.m_dir = q.RotateVec(ORIG_VIEW_DIR);
    m_camera.m_up = q.RotateVec(ORIG_UP_DIR);
  }
#endif // #if defined(WIN32) || defined(MACOSX)

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

  AmjuGL::SetColour(Colour(0.2f, 0.2f, 1, 1));
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

