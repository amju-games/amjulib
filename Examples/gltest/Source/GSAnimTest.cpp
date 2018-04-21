#include <AmjuGL.h>
#include <AmjuGL-OpenGL.h>
#include "AnimPlayer.h"
#include "IndexedMesh.h"
#include "Skeleton.h"
#include "Skin.h"
#include "File.h"
#include "StringUtils.h"
#include "AnimManager.h"
#include "GSAnimTest.h"

namespace Amju
{
static const std::string ASSETS = "Characters/Dog/";

/*
static const char* MESH_FILE = "kirk-mesh-uv.txt";
static const char* SKIN_FILE = "kirk-skin.txt";
static const char* SKEL_FILE = "kirk-skel.txt";
static const char* ANIM_FILE = "kirk-anim3.txt";
*/

static const char* MESH_FILE = "dog-mesh.txt";
static const char* SKIN_FILE = "dog-skin.txt";
static const char* SKEL_FILE = "dog-skel.txt";
static const char* ANIM_FILE = "dog-anim.txt";

/*
static const char* MESH_FILE = "amjula-mesh.txt";
static const char* SKIN_FILE = "amjula-skin.txt";
static const char* SKEL_FILE = "amjula-skel.txt";
static const char* ANIM_FILE = "amjula-anim.txt";
*/

static AnimPlayer animPlayer;
static float yRot = 0;
static float xRot = 0;
static float xPos = 0;
static float yPos = 0;
static bool showSkel = true;
static bool showMesh = true;
static bool xray = false;
static float dt = 0.001f; // Mac default dt 
static bool paused = false;
static bool lighting = true;
static bool textured = true;
static bool stepping = false; // if true, step to next keyframe (1/24 sec)
static float steptime = 0;
static bool interpolate = true;

GSAnimTest::GSAnimTest()
{
  //m_nextState=...
}

void GSAnimTest::Update()
{
  GSBase::Update();

}

void draw()
{
  static PMesh mesh = 0;
  static bool initialised = false;

  if (!initialised)
  {
std::cout << "Creating mesh...\n";

    TheAnimManager::Instance()->SetUpAnimPlayer(&animPlayer, ASSETS + MESH_FILE, ASSETS + SKIN_FILE, ASSETS + SKEL_FILE);

    // Load anim - needs skel file for bone names
    if (!TheAnimManager::Instance()->LoadAnim(ASSETS + ANIM_FILE, ASSETS + SKEL_FILE))
    {
      std::cout << "Failed to load anim " << ANIM_FILE << "\n";
      exit(1);
    }

    PAnim pAnim = TheAnimManager::Instance()->GetAnim(ANIM_FILE);

    // If no anim, skeleton be will be static
    // Blendtime is ignored for first anim
    animPlayer.SetAnim(pAnim, 0);

    initialised = true;
  }

  if (!paused)
  {
    animPlayer.Update(dt, interpolate);
  }
  else if (stepping)
  {
    steptime += dt;
    if (steptime >= (1.0f/24.0f))
    {
      stepping = false;
      steptime = 0;
    }
    else
    {
      animPlayer.Update(dt, interpolate);
    }
  }
  
//  AmjuGL::LookAt(0, 10.0f, 20.0f, 0, 0,0, 0, 1.0f, 0);

  AmjuGL::Translate(xPos, yPos, 0);

  AmjuGL::RotateX(xRot);
  AmjuGL::RotateY(yRot);

  // Draw origin
/*
  glDisable(GL_TEXTURE_2D);
  glPushAttrib(GL_LIGHTING_BIT);
  glDisable(GL_LIGHTING);
  glLineWidth(1);
  glBegin(GL_LINES);
  glVertex3f(1, 0, 0);
  glVertex3f(-1, 0, 0);
  glVertex3f(0, 0, 1);
  glVertex3f(0, 0, -1);
  glEnd();
  glPopAttrib();
  glEnable(GL_TEXTURE_2D);
*/
  if (showSkel)
  {
    animPlayer.DrawSkel();
  }

  if (showMesh)
  {
    if (xray)
    {
      /*
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
      */
    }
    if (lighting)
    {
      //glEnable(GL_LIGHTING);
    }
    else
    {
      //glDisable(GL_LIGHTING);
    }
    if (textured)
    {
      //glEnable(GL_TEXTURE_2D);
    }
    else
    {
      //glDisable(GL_TEXTURE_2D);
    }
    animPlayer.Draw();
    //glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  }

  //glDisable(GL_DEPTH_TEST);
  //glDisable(GL_TEXTURE_2D);

  float t = animPlayer.GetTime();
  int frame = t * 24.0f;

//  std::string s = "Time: " + ToString(t) + " (" + ToString(frame) + "/24)";
//  PrintText(s, -0.95f, 0.95f);
//  PrintText("[M]Mesh [S]Skel [X]Xray [T]Texture [L]Lighting [P]Play/pause [1]Slower [2]Faster", -0.95f, 0.9f);
//  PrintText("[Q]Step to next 1/24th sec [0]Reset [I]Interpolate", -0.95f, 0.85f);

//  glEnable(GL_TEXTURE_2D);
//  glEnable(GL_DEPTH_TEST);


}

void GSAnimTest::DrawScene()
{
  DrawHelp();

  draw();
}

void GSAnimTest::OnActive()
{
}

bool GSAnimTest::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSAnimTest::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
