#include "EventPollerImplSDL.h"
#include <SDL.h> 
#include <Screen.h>

namespace Amju
{
void SetKeyType(const SDL_KeyboardEvent& ske, KeyEvent* pKe)
{
  pKe->keyDown = (ske.state == SDL_PRESSED);
  switch (ske.keysym.sym)
  {
  case SDLK_RETURN:
    pKe->keyType = AMJU_KEY_ENTER;
    break;
  case SDLK_SPACE:
    pKe->keyType = AMJU_KEY_SPACE;
    break;
  case SDLK_UP:
    pKe->keyType = AMJU_KEY_UP;
    break;
  case SDLK_DOWN:
    pKe->keyType = AMJU_KEY_DOWN;
    break;
  case SDLK_LEFT:
    pKe->keyType = AMJU_KEY_LEFT;
    break;
  case SDLK_RIGHT:
    pKe->keyType = AMJU_KEY_RIGHT;
    break;
  case SDLK_ESCAPE:
    pKe->keyType = AMJU_KEY_ESC;
    break;
  case SDLK_BACKSPACE:
    pKe->keyType = AMJU_KEY_BACKSPACE;
    break;
  case SDLK_DELETE:
    pKe->keyType = AMJU_KEY_DELETE;
    break;
  default:
    pKe->keyType = AMJU_KEY_CHAR;
    // Must do this to get e.g. '!' rather than '1'.
    // sym is just the 'lower case' symbol on the key.
    pKe->key = ske.keysym.unicode & 0x7f;
    break;
  }
}

void EventPollerImplSDL::Update(Listeners* pListeners)
{
  // Init joysticks - TODO check for extra joysticks plugged in
  static bool first = true;
  if (first)
  {
    first = false;

    SDL_EnableUNICODE(1); // For getting chars from KB

    int numJs = SDL_NumJoysticks();
    for (int i = 0; i < numJs; i++)
    {
      SDL_JoystickOpen(i);  
    }
    SDL_JoystickEventState(SDL_ENABLE);
  }

  SDL_Event e;
  while (SDL_PollEvent(&e))
  {
    KeyEvent ke;
    bool isKeyEvent = false;

    // Remember x and y axis values for each joystick
    static const int MAX_NUM_JOYSTICKS = 4;
    static JoyAxisEvent je[MAX_NUM_JOYSTICKS];
    int joyNum = 0;
    bool isJoyEvent = false;

    CursorEvent ce;
    bool isCursorEvent = false;

    ButtonEvent be;
    bool isButtonEvent = false;

    MouseButtonEvent mbe;
    bool isMouseButtonEvent = false;

    bool isQuit = false;

    switch (e.type)
    {
    case SDL_QUIT:
      isQuit = true;
      break;
    case SDL_ACTIVEEVENT:			/* Application loses/gains visibility */
      break;
    case SDL_KEYDOWN:			/* Keys pressed */
    case SDL_KEYUP:			/* Keys released */
      {
        SDL_KeyboardEvent ske = e.key;
        if (ske.keysym.sym < SDLK_NUMLOCK) // i.e. is not a modifier
        {
          SetKeyType(ske, &ke);
          isKeyEvent = true;
        }
        break;
      }

    case SDL_MOUSEMOTION:			/* Mouse moved */
      {
        SDL_MouseMotionEvent sme = e.motion;
        ce.controller = 0;
        // Convert screen coords to -1..1 NDCs
        ce.x = ((float)sme.x / (float)Screen::X()) * 2.0f - 1.0f;
        ce.y = (1.0f - (float)sme.y / (float)Screen::Y()) * 2.0f - 1.0f;
        isCursorEvent = true;
        break;
      }

    case SDL_MOUSEBUTTONDOWN:		/* Mouse button pressed */
    case SDL_MOUSEBUTTONUP:		/* Mouse button released */
      {
        SDL_MouseButtonEvent sme = e.button;
        mbe.x = ((float)sme.x / (float)Screen::X()) * 2.0f - 1.0f;
        mbe.y = (1.0f - (float)sme.y / (float)Screen::Y()) * 2.0f - 1.0f;
        
        if ((Uint32)sme.button == 1)
        {
          mbe.button = AMJU_BUTTON_MOUSE_LEFT;
        }
        else if ((Uint32)sme.button == 2)
        {
          mbe.button = AMJU_BUTTON_MOUSE_MIDDLE;
        }
        else if ((Uint32)sme.button == 3)
        {
          mbe.button = AMJU_BUTTON_MOUSE_RIGHT;
        }
        else
        {
          // Don't know why, but sometimes we get here
          //Assert(0);
          break;
        }
        
        mbe.isDown = (sme.state == SDL_PRESSED);
        isMouseButtonEvent = true;
        break;
      }

    case SDL_JOYAXISMOTION:		/* Joystick axis motion */
      {
        SDL_JoyAxisEvent sje = e.jaxis;
        joyNum = sje.which;
        je[joyNum].controller = joyNum;
        if (sje.axis == 0)
        {
          je[joyNum].x = (float)(sje.value) * (1.0f/32768.0f);
          isJoyEvent = true;
        }
        else if (sje.axis == 1)
        {
          je[joyNum].y = (float)(sje.value) * (1.0f/32768.0f);
          isJoyEvent = true;
        }
        break;
      }

    case SDL_JOYBALLMOTION:		/* Joystick trackball motion */
    case SDL_JOYHATMOTION:		/* Joystick hat position change */
    case SDL_JOYBUTTONDOWN:		/* Joystick button pressed */
    case SDL_JOYBUTTONUP:			/* Joystick button released */
    case SDL_SYSWMEVENT:			/* System specific event */
    case SDL_VIDEORESIZE:			/* User resized video mode */
    case SDL_VIDEOEXPOSE:			/* Screen needs to be redrawn */
      break;
    }

    int eaten = AMJU_MAX_PRIORITY + 1;
    for (Listeners::iterator it = pListeners->begin(); it != pListeners->end(); ++it)
    {
      if (it->first > eaten)
      {
        break;
      }

      EventListener* e = it->second;
      Assert(e);

      if (isQuit && e->OnQuitEvent() ||
          isKeyEvent && e->OnKeyEvent(ke) ||
          isCursorEvent && e->OnCursorEvent(ce) ||
          isJoyEvent && e->OnJoyAxisEvent(je[joyNum]) ||
          isButtonEvent && e->OnButtonEvent(be) ||
          isMouseButtonEvent && e->OnMouseButtonEvent(mbe))
      {
        eaten = it->first;
      }
    }
  }
}
}
