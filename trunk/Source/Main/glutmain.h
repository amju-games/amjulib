#ifdef AMJU_USE_GLUT
#include <AmjuFirst.h>
#include <iostream>

#include <AmjuGL-OpenGL.h>
#include <Screen.h>
#include <Game.h>
#include <SoundManager.h>
#include <BassSoundPlayer.h>
#include <AmjuGLWindowInfo.h>
#include <EventPollerImplGeneric.h>

// TODO include glut.h for other platforms
#ifdef MACOSX
#include <GLUT/glut.h>
#endif
#ifdef WIN32
#include <gl/glut.h>
#endif

#include <AmjuFinal.h>

//#define KEY_CODE_DEBUG

using namespace Amju;

namespace Amju
{
extern AmjuGLWindowInfo w;

// TODO Make this a GLUT version in AmjuGLOpenGL
bool CreateWindowGLUT(AmjuGLWindowInfo*)
{
  return true;
}
}

void draw()
{
  TheGame::Instance()->RunOneLoop();

#ifndef WIN32
  glutSwapBuffers(); // ?
#endif

  glutPostRedisplay();
}

void QueueEvent(Amju::Event* e)
{
  ((Amju::EventPollerImplGeneric*)Amju::TheEventPoller::Instance()->GetImpl())->QueueEvent(e);
}

void key(char k, bool down)
{
  KeyEvent* ke = new KeyEvent;  
  ke->modifier = glutGetModifiers();
  ke->keyDown = down;

  if (k == 127) // backspace
  {
#ifdef WIN32
    ke->keyType = AMJU_KEY_DELETE;
#else
    ke->keyType = AMJU_KEY_BACKSPACE;
#endif
  }
  else if (k == 8) // delete
  {
#ifdef WIN32
    ke->keyType = AMJU_KEY_BACKSPACE;
#else
    ke->keyType = AMJU_KEY_DELETE;
#endif
  }
  else if (k == 13)
  {
    ke->keyType = AMJU_KEY_ENTER;
  }
  else if (k == 27) // esc
  {
    ke->keyType = AMJU_KEY_ESC;
  }
  else
  {
#ifdef KEY_CODE_DEBUG
std::cout << "Got key " << (down ? "DOWN" : "UP") << " event, char is : " << k << " (" << (int)k << ")\n";
#endif

    ke->keyType = AMJU_KEY_CHAR;
    ke->key = k;
  }

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

  Assert(GLUT_ACTIVE_SHIFT == AMJU_KEY_MOD_SHIFT);
  Assert(GLUT_ACTIVE_CTRL == AMJU_KEY_MOD_CTRL);
  Assert(GLUT_ACTIVE_ALT == AMJU_KEY_MOD_ALT);

  ke->modifier = glutGetModifiers();
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

  case GLUT_KEY_PAGE_UP:
    ke->keyType = AMJU_KEY_PAGE_UP;
    break;

  case GLUT_KEY_PAGE_DOWN:
    ke->keyType = AMJU_KEY_PAGE_DOWN;
    break;

  case GLUT_KEY_HOME:
    ke->keyType = AMJU_KEY_HOME;
    break;

  case GLUT_KEY_END:
    ke->keyType = AMJU_KEY_END;
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
  mbe->x = (float)x / (float)Amju::Screen::X() * 2.0f - 1.0f; 
  mbe->y = 1.0f - (float)y / (float)Amju::Screen::Y() * 2.0f; 
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

void joystick(unsigned int buttonMask, int x, int y, int z)
{
  // TODO ignore if same state as last time
  JoyAxisEvent* j = new JoyAxisEvent;
  j->controller = 0;
  j->x = (float)x / 1000.0f;
  j->y = (float)y / 1000.0f;
  // TODO buttons
  QueueEvent(j);
}

int main(int argc, char **argv)
{
  // TODO Have a CreateWindow for GLUT.. but events are tangled up with creating windows :-(
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);

  // w is global defined in game-specific code
  glutInitWindowSize(w.GetWidth(), w.GetHeight()); 

  glutCreateWindow("Hello"); // TODO App name
  glutDisplayFunc(draw);

  // DON'T auto-repeat - we will do it ourselves
  glutIgnoreKeyRepeat(1);

  glutKeyboardFunc(keydown);
  glutKeyboardUpFunc(keyup);
  glutSpecialFunc(specialkeydown);
  glutSpecialUpFunc(specialkeyup);
  glutMouseFunc(mousebutton);
  glutMotionFunc(mousemove);
  glutPassiveMotionFunc(mousemove);

  int js = glutDeviceGet(GLUT_HAS_JOYSTICK);
  if (js != 0)
  {
    std::cout << "GLUT says joystick available!\n";
  }
  glutJoystickFunc(joystick, 50);

  TheEventPoller::Instance()->SetImpl(new EventPollerImplGeneric); 

  AmjuGL::SetImpl(new AmjuGLOpenGL(CreateWindowGLUT));

  // Defined in game-specific code
  Amju::AmjuGL::CreateWindow(&w);
  Amju::AmjuGL::Init();

  // This game-specific function must be defined for each project
  StartUp();

  // Can't do this, glutMainLoop is in charge
  //TheGame::Instance()->Run();

  glutMainLoop();

  return 0;
}
 
#endif // AMJU_USE_GLUT

