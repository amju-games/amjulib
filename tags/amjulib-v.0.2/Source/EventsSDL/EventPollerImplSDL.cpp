#include <AmjuFirst.h>
#include "EventPollerImplSDL.h"
#include <SDL.h> 
#include <Screen.h>
#include <AmjuFinal.h>

namespace Amju
{
void SetKeyType(const SDL_KeyboardEvent& ske, KeyEvent* pKe)
{
  // Modifiers
  if (ske.keysym.mod & KMOD_SHIFT) pKe->modifier |= AMJU_KEY_MOD_SHIFT;
  if (ske.keysym.mod & KMOD_CTRL) pKe->modifier |= AMJU_KEY_MOD_CTRL;
  if (ske.keysym.mod & KMOD_ALT) pKe->modifier |= AMJU_KEY_MOD_ALT;

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
  case SDLK_HOME:
    pKe->keyType = AMJU_KEY_HOME;
    break;
  case SDLK_END:
    pKe->keyType = AMJU_KEY_END;
    break;
  default:
    pKe->keyType = AMJU_KEY_CHAR;
    // Must do this to get e.g. '!' rather than '1'.
    // sym is just the 'lower case' symbol on the key.
    if (ske.keysym.unicode != 0)
    {
      pKe->key = ske.keysym.unicode & 0x7f;
    }
    else
    {
      pKe->key = ske.keysym.sym; // no unicode code.. 
    }
    break;
  }
}

void EventPollerImplSDL::Update(Listeners* pListeners)
{
  EventPollerImpl::Update(pListeners);

  // Init joysticks etc
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
      QueueEvent(new QuitEvent);
      break;

    case SDL_KEYDOWN:			/* Keys pressed */
    case SDL_KEYUP:			/* Keys released */
      {
        SDL_KeyboardEvent ske = e.key;
        if (ske.keysym.sym < SDLK_NUMLOCK) // i.e. is not a modifier
        {
          SetKeyType(ske, &ke);

#ifdef _DEBUG
static const char* TYPE[] = 
{
  "char",        //AMJU_KEY_CHAR,  // printable character
  "up arrow",    //AMJU_KEY_UP,    // up arrow
  "down arrow",  //AMJU_KEY_DOWN,
  "let arrow",   //AMJU_KEY_LEFT,
  "right arrow", //AMJU_KEY_RIGHT,
  "enter",       //AMJU_KEY_ENTER,
  "space",       //AMJU_KEY_SPACE,
  "esc",         //AMJU_KEY_ESC,
  "backspace",   //AMJU_KEY_BACKSPACE,
  "delete",      //AMJU_KEY_DELETE,
};
std::cout << "Key event: type: " << TYPE[ke.keyType] << " key: " << ke.key << 
  (ke.keyDown ? " DOWN" : " UP") << "\n";
#endif

          isKeyEvent = true;
          NotifyListenersWithPriority(&ke, pListeners);
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

        NotifyListenersWithPriority(&ce, pListeners);

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

        NotifyListenersWithPriority(&mbe, pListeners);

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

          NotifyListenersWithPriority(&je[joyNum], pListeners);
        }
        else if (sje.axis == 1)
        {
          je[joyNum].y = (float)(sje.value) * (-1.0f/32768.0f);
          isJoyEvent = true;

          NotifyListenersWithPriority(&je[joyNum], pListeners);
        }
        break;
      }

    case SDL_JOYBUTTONDOWN:		/* Joystick button pressed */
    case SDL_JOYBUTTONUP:			/* Joystick button released */
      {
        SDL_JoyButtonEvent jbe = e.jbutton;
        joyNum = jbe.which;
        be.controller = joyNum;
        be.isDown = (jbe.state == SDL_PRESSED);
        // TODO Array to map SDL button num to Amju event enum
        be.button = (Button)jbe.button;
        isButtonEvent = (jbe.button <= 3); // only handle buttons 0-3 right now
          
        NotifyListenersWithPriority(&be, pListeners);
      }
      break;
    } // switch
    /*
    int eaten = AMJU_MAX_PRIORITY + 1;
    int count = 0;
    for (Listeners::iterator it = pListeners->begin(); it != pListeners->end(); ++it)
    {
      int priority = it->first;
      if (priority > eaten)
      {
        break;
      }

      EventListener* ev = it->second;
      Assert(ev);

      if (isQuit && ev->OnQuitEvent() ||
          isKeyEvent && ev->OnKeyEvent(ke) ||
          isCursorEvent && ev->OnCursorEvent(ce) ||
          isJoyEvent && ev->OnJoyAxisEvent(je[joyNum]) ||
          isButtonEvent && ev->OnButtonEvent(be) ||
          isMouseButtonEvent && ev->OnMouseButtonEvent(mbe))
      {
        eaten = it->first;
      }

      count++;
    }
    */

  } // while more events
}
}
