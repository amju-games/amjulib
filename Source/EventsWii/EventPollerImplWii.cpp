#include <iostream>
#include "EventPollerImplWii.h"
#include <wiiuse/wpad.h>
#include "Pause.h"  

namespace Amju
{
// Store previous button states so we only generate an event when the
//  button is pressed or released
static const int NUM_BUTTONS = 13;
//static bool s_wasPressed[NUM_BUTTONS];
static const int WII_AMJU_BUTTON[NUM_BUTTONS][2] = 
{
  { WPAD_BUTTON_2,         AMJU_BUTTON_MAX   },
  { WPAD_BUTTON_1,         AMJU_BUTTON_MAX   },
  { WPAD_BUTTON_B,         AMJU_BUTTON_B     },
  { WPAD_BUTTON_A,         AMJU_BUTTON_A     },
  { WPAD_BUTTON_MINUS,     AMJU_BUTTON_MAX   },
  { WPAD_BUTTON_HOME,      AMJU_BUTTON_MAX   },
  { WPAD_BUTTON_LEFT,      AMJU_BUTTON_LEFT  },
  { WPAD_BUTTON_RIGHT,     AMJU_BUTTON_RIGHT },
  { WPAD_BUTTON_DOWN,      AMJU_BUTTON_DOWN  },
  { WPAD_BUTTON_UP,        AMJU_BUTTON_UP    },
  { WPAD_BUTTON_PLUS,      AMJU_BUTTON_MAX   },
  { WPAD_NUNCHUK_BUTTON_Z, AMJU_BUTTON_D     },
  { WPAD_NUNCHUK_BUTTON_C, AMJU_BUTTON_C     }
};

EventPollerImplWii::EventPollerImplWii()
{
  WPAD_Init();
  
  for (int i = 0; i < 4; i++)
  {
    WPAD_SetDataFormat(i, WPAD_FMT_BTNS_ACC_IR);
    WPAD_SetVRes(i, 640, 480);
  }
    
  //for (int i = 0; i < NUM_BUTTONS; i++)
  //{
  //  s_wasPressed[i] = false;
  //}
}

void EventPollerImplWii::Update(Listeners* pListeners)
{
  static float ir_x[WPAD_MAX_WIIMOTES] = { 0, 0, 0, 0, 0 };
  static float ir_y[WPAD_MAX_WIIMOTES] = { 0, 0, 0, 0, 0 };

  // TODO TEMP TEST
  // Remote 0 can quit with Home button
  if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) 
  {
    exit(0);
  }  

  WPAD_ScanPads();
  for (int ctr = 0; ctr < WPAD_MAX_WIIMOTES; ctr++)
  {    
    // Get IR info
    ir_t ir;
    WPAD_IR(ctr, &ir);

    // Rotation data
    orient_t orient;
    WPAD_Orientation(ctr, &orient);

    // Nunchuck/Balance board data
    expansion_t et;
    WPAD_Expansion(ctr, &et);

    static vec2b_t joyPos = { 0, 0 };
    bool isJoyEvent = false;
    JoyAxisEvent je;

    CursorEvent ce;
    ce.controller = ctr;
    bool isCursorEvent = false;

    // Cursor event ?
    if (ir.x != ir_x[ctr] || ir.y != ir_y[ctr])
    {
      ir_x[ctr] = ir.x;
      ir_y[ctr] = ir.y;
      isCursorEvent = true;
      ce.x = ir.x;
      ce.y = ir.y;
    }

    RotationEvent re[3];
    bool isRotEvent[3] = { false, false, false };
    static float oldDegs[3] = { 0, 0, 0 };
    for (int i = 0; i < 3; i++)
    {
      float degs = (i == 0) ? orient.pitch : ((i == 1) ? orient.yaw : -orient.roll);
      if (degs != oldDegs[i])
      {
        oldDegs[i] = degs;
        re[i].controller = ctr;
        isRotEvent[i] = true;
        re[i].axis = (Axis)i;
        re[i].degs = degs;
      }
    }
    
    BalanceBoardEvent bbe;
    bool isBalanceEvent = false;
    
    if (et.type == EXP_NUNCHUK)
    {
      nunchuk_t* nc = (nunchuk_t*)&et.nunchuk;

      if (nc->js.pos.x != joyPos.x || nc->js.pos.y != joyPos.y)
      {
        isJoyEvent = true;
        joyPos = nc->js.pos;
        je.controller = ctr;
        je.x = (float)joyPos.x / 128.0f - 1.0f;
        je.y = -((float)joyPos.y / 128.0f - 1.0f);
      }
      //if (IS_PRESSED(nc, NUNCHUK_BUTTON_C))		printf("Nunchuk: C pressed\n");
      //if (IS_PRESSED(nc, NUNCHUK_BUTTON_Z))		printf("Nunchuk: Z pressed\n");
      //printf("nunchuk roll  = %f\n", nc->orient.roll);
      //printf("nunchuk pitch = %f\n", nc->orient.pitch);
      //printf("nunchuk yaw   = %f\n", nc->orient.yaw);
      //printf("nunchuk joystick angle:     %f\n", nc->js.ang);
      //printf("nunchuk joystick magnitude: %f\n", nc->js.mag);
    }  
    else if (et.type == EXP_WII_BOARD)
    {
      // Balance x and y seem to be between approx -30..30,
      //  and fluctuate, so best to get closest int 
      static int prevwbx = 0;
      static int prevwby = 0;
      
      wii_board_t* wb = (wii_board_t*)&et.wb;
      
      int wbx = (int)(wb->x + 0.5f); // nearest int
      int wby = (int)(wb->y + 0.5f);
      if (wbx != prevwbx || wby != prevwby)
      {
        prevwbx = wbx;
        prevwby = wby;

        isBalanceEvent = true;
        // get coords between -1..1 
        bbe.x = (float)wbx / 30.0f; //(wbx + 30) * 10;
        bbe.y = (float)wby / 30.0f; //(wby + 30) * 8;
        if (bbe.x > 1.0f)
        {
          bbe.x = 1.0f;
        }
        if (bbe.x < -1.0f)
        {
          bbe.x = -1.0f;
        }
        if (bbe.y > 1.0f)
        {
          bbe.y = 1.0f;
        }
        if (bbe.y < -1.0f)
        {
          bbe.y = -1.0f;
        }
      }
    }

    bool isQuit = false;
    if (WPAD_ButtonsDown(ctr) & WPAD_BUTTON_HOME) 
    {
      isQuit = true;
    }
  
    bool isPressed[NUM_BUTTONS];
    bool isReleased[NUM_BUTTONS];
    u32 buttonsPressed = WPAD_ButtonsDown(ctr);// & ~WPAD_ButtonsHeld(ctr);
    u32 buttonsRel = WPAD_ButtonsUp(ctr);// & WPAD_ButtonsHeld(ctr); // ?
    
    for (int i = 0; i < NUM_BUTTONS; i++)
    {
      isPressed[i] = (buttonsPressed & WII_AMJU_BUTTON[i][0]);
      isReleased[i] = (buttonsRel & WII_AMJU_BUTTON[i][0]);
    }
    
    for (Listeners::iterator it = pListeners->begin(); it != pListeners->end(); ++it)
    {
      EventListener* e = *it;
    
      ButtonEvent be;
      be.controller = ctr;
      for (int i = 0; i < NUM_BUTTONS; i++)
      {
        if (isPressed[i])
        {
          be.isDown = true;
          be.button = (Button)WII_AMJU_BUTTON[i][1];
//std::cout << "Button event! - pressed!\n";
//PAUSE;
          
          e->OnButtonEvent(be);
        }
        else if (isReleased[i])
        {
          be.isDown = false;
          be.button = (Button)WII_AMJU_BUTTON[i][1];
//std::cout << "Button event! - released!\n";
//PAUSE;
          e->OnButtonEvent(be);
        }
      }
      
      if (isCursorEvent)
      {
        e->OnCursorEvent(ce);
      }
      
      for (int i = 0; i < 3; i++)
      {
        if (isRotEvent[i])
        {
          e->OnRotationEvent(re[i]);
        }
      }

      if (isJoyEvent)
      {
        e->OnJoyAxisEvent(je);
      }
  
      if (isBalanceEvent)
      {
        e->OnBalanceBoardEvent(bbe);
      }
  
      if (isQuit)
      {
        e->OnQuitEvent();
      }
    }
  }
}
}
