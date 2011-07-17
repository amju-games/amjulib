#ifdef MACOSX
#include <AmjuFirst.h>
#include <iostream>

#include <AmjuGL-OpenGL.h>
#include <Screen.h>
#include <Game.h>
#include <SoundManager.h>
#include <BassSoundPlayer.h>
#include <AmjuGLWindowInfo.h>
#include <EventPollerImplGeneric.h>
#include <File.h>
#include <ResourceManager.h>
#include <Font.h>
#include "GSBelt.h"
#include "GSMain.h"
#include "GSObb2Test.h"
#include "GSRigidBody.h"
#include "GSLighting.h"
#include "GSShaderWave.h"

#ifdef AMJU_USE_GLUT
#include <GLUT/glut.h>
#endif

#include <AmjuFinal.h>

static const int WINDOW_W = 640;
static const int WINDOW_H = 480;

using namespace Amju;

namespace Amju
{
// TODO Make this a GLUT version in AmjuGLOpenGL
bool AmjuGLOpenGL::CreateWindow(AmjuGLWindowInfo*)
{
  return true;
}
}

#ifdef AMJU_USE_GLUT
void draw()
{
  TheGame::Instance()->RunOneLoop();

  glutSwapBuffers(); // ?
  glutPostRedisplay();
}

void QueueEvent(Amju::Event* e)
{
  ((Amju::EventPollerImplGeneric*)Amju::TheEventPoller::Instance()->GetImpl())->QueueEvent(e);
}

void key(char k, bool down)
{
  KeyEvent* ke = new KeyEvent;  
  ke->keyType = AMJU_KEY_CHAR;
  ke->key = k;
  ke->keyDown = true;
  QueueEvent(ke);
}

void keydown(unsigned char k, int, int)
{
  key(k, true);
}

void keyup(unsigned char k, int, int)
{
  key(k, false);
}

void special(int key, bool down)
{
  KeyEvent* ke = new KeyEvent;  
  switch (key)
  {
  case GLUT_KEY_LEFT:
    ke->keyType = AMJU_KEY_LEFT;
    break;

  case GLUT_KEY_RIGHT:
    ke->keyType = AMJU_KEY_RIGHT;
    break;

  case GLUT_KEY_UP:
    ke->keyType = AMJU_KEY_UP;
    break;

  case GLUT_KEY_DOWN:
    ke->keyType = AMJU_KEY_DOWN;
    break;

  }
  ke->keyDown = down;
  QueueEvent(ke);
}

void specialkeydown(int key, int, int)
{
  special(key, true);
}

void specialkeyup(int key, int, int)
{
  special(key, false);
}

void mousebutton(int button, int state, int x, int y)
{
  MouseButtonEvent* mbe = new MouseButtonEvent;
  switch (button)
  {
  case GLUT_LEFT_BUTTON:
    mbe->button = AMJU_BUTTON_MOUSE_LEFT;
    break;

  case GLUT_RIGHT_BUTTON:
    mbe->button = AMJU_BUTTON_MOUSE_RIGHT;
    break;

  case GLUT_MIDDLE_BUTTON:
    mbe->button = AMJU_BUTTON_MOUSE_MIDDLE;
    break;
  }
  mbe->isDown = (state == GLUT_DOWN);
  mbe->x = (float)x / (float)Amju::Screen::X(); 
  mbe->y = 1.0f - (float)y / (float)Amju::Screen::Y(); 
  QueueEvent(mbe);
}

void mousemove(int x, int y)
{
  CursorEvent* ce = new CursorEvent;
  ce->controller = 0;
  ce->x = (float)x / (float)Amju::Screen::X() * 2.0f - 1.0f; 
  ce->y = 1.0f - (float)y / (float)Amju::Screen::Y() * 2.0f; 
  QueueEvent(ce);
}
#endif

int main(int argc, char **argv)
{
#ifdef AMJU_USE_GLUT
  // TODO Have a CreateWindow for GLUT.. but events are tangled up with creating windows :-(
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(WINDOW_W, WINDOW_H);
  glutCreateWindow("Hello I R GLUT window too");
  glutDisplayFunc(draw);
  glutKeyboardFunc(keydown);
  glutKeyboardUpFunc(keyup);
  glutSpecialFunc(specialkeydown);
  glutSpecialUpFunc(specialkeyup);
  glutMouseFunc(mousebutton);
  glutMotionFunc(mousemove);
  glutPassiveMotionFunc(mousemove);
#endif
  
  TheGame::Instance()->SetCurrentState(TheGSShaderWave::Instance());

  TheEventPoller::Instance()->SetImpl(new EventPollerImplGeneric); 

  AmjuGL::SetImpl(new AmjuGLOpenGL);

  Amju::AmjuGLWindowInfo w(WINDOW_W, WINDOW_H, false);
  Amju::AmjuGL::CreateWindow(&w);
  Amju::AmjuGL::Init();

////  TheSoundManager::Instance()->SetImpl(new BassSoundPlayer); 

  TheResourceManager::Instance()->AddLoader("font", FontLoader);

#ifdef AMJU_USE_GLUT
  // Can't do this, glutMainLoop is in charge
  //TheGame::Instance()->Run();

  glutMainLoop();
#endif

  return 0;
}
 
#endif // MACOSX
