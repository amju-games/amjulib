#include "AmjuGL-DX9.h"
#include <AmjuGLWindowInfo.h>
#include <WinScreen.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h> // sprintf

using namespace Amju;

//-----------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------
//HWND       hWnd = 0;
HINSTANCE   hInstance = 0;

LPDIRECT3D9       g_pD3D = 0;
//LPDIRECT3DDEVICE9 AmjuGLDX9::dd = 0;

WNDCLASSEX winClass;

static LPCWSTR AMJU_WINDOW_CLASS_NAME = L"MY_WINDOWS_CLASS";

void init(void);
void shutDown(void);
//void render(void);


namespace Amju
{
bool AmjuGLDX9::CreateWindow(AmjuGLWindowInfo* w)
{
	winClass.lpszClassName = AMJU_WINDOW_CLASS_NAME;
	winClass.cbSize        = sizeof(WNDCLASSEX);
	winClass.style         = CS_HREDRAW | CS_VREDRAW;
  if (m_wndproc)
  {
	  winClass.lpfnWndProc   = m_wndproc;
  }
  else
  {
	  winClass.lpfnWndProc   = DefWindowProc;
  }
	winClass.hInstance     = hInstance;
	//winClass.hIcon	       = LoadIcon(hInstance, (LPCTSTR)IDI_DIRECTX_ICON);
  //  winClass.hIconSm	   = LoadIcon(hInstance, (LPCTSTR)IDI_DIRECTX_ICON);
	winClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winClass.lpszMenuName  = NULL;
	winClass.cbClsExtra    = 0;
	winClass.cbWndExtra    = 0;

	if (!RegisterClassEx(&winClass))
  {
		return false;
  }

	HWND hWnd = CreateWindowEx(
    NULL, 
    AMJU_WINDOW_CLASS_NAME, 
    L"Amju",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,		
    0, 
    0, 
    w->GetWidth(), 
    w->GetHeight(), 
    NULL, 
    NULL, 
    hInstance, 
    NULL);
 
	if (hWnd == NULL)
  {
		return false; 
  }

  SetHWnd(hWnd);

  ShowWindow(hWnd, SW_SHOW);
  UpdateWindow(hWnd);

  init();

  return true;
}
} // namespace

void KillDX9Window()
{
	shutDown();
  UnregisterClass(AMJU_WINDOW_CLASS_NAME, winClass.hInstance );
}

/*
//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR     lpCmdLine,
                    int       nCmdShow )
{

    ShowWindow( g_hWnd, nCmdShow );
    UpdateWindow( g_hWnd );

	init();

	while( uMsg.message != WM_QUIT )
	{
		if( PeekMessage( &uMsg, NULL, 0, 0, PM_REMOVE ) )
		{ 
			TranslateMessage( &uMsg );
			DispatchMessage( &uMsg );
		}
        else
		    render();
	}

	shutDown();

    UnregisterClass( "MY_WINDOWS_CLASS", winClass.hInstance );

	return uMsg.wParam;
}
*/

//-----------------------------------------------------------------------------
// Name: init()
// Desc: 
//-----------------------------------------------------------------------------
void init( void )
{
    g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );

	if( g_pD3D == NULL )
	{
		// TO DO: Respond to failure of Direct3DCreate8
		return;
	}

    D3DDISPLAYMODE d3ddm;

    if( FAILED( g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )
	{
		// TO DO: Respond to failure of GetAdapterDisplayMode
		return;
	}

	HRESULT hr;

	if( FAILED( hr = g_pD3D->CheckDeviceFormat( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
												d3ddm.Format, D3DUSAGE_DEPTHSTENCIL,
												D3DRTYPE_SURFACE, D3DFMT_D16 ) ) )
	{
		if( hr == D3DERR_NOTAVAILABLE )
			// POTENTIAL PROBLEM: We need at least a 16-bit z-buffer!
			return;
	}

	//
	// Do we support hardware vertex processing? if so, use it. 
	// If not, downgrade to software.
	//

	D3DCAPS9 d3dCaps;

	if( FAILED( g_pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, 
		                               D3DDEVTYPE_HAL, &d3dCaps ) ) )
	{
		// TO DO: Respond to failure of GetDeviceCaps
		return;
	}

	DWORD dwBehaviorFlags = 0;

	if( d3dCaps.VertexProcessingCaps != 0 )
		dwBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		dwBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//
	// Everything checks out - create a simple, windowed device.
	//

	D3DPRESENT_PARAMETERS d3dpp;
	memset(&d3dpp, 0, sizeof(d3dpp));

    d3dpp.BackBufferFormat       = d3ddm.Format;
	d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed               = TRUE;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;

    if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, (HWND)GetHWnd(),
                                      dwBehaviorFlags, &d3dpp, &AmjuGLDX9::dd ) ) )
	{
		// TO DO: Respond to failure of CreateDevice
		return;
	}
}

//-----------------------------------------------------------------------------
// Name: shutDown()
// Desc: 
//-----------------------------------------------------------------------------
void shutDown( void )
{
    if( AmjuGLDX9::dd != NULL )
        AmjuGLDX9::dd->Release();

    if( g_pD3D != NULL )
        g_pD3D->Release();
}

//-----------------------------------------------------------------------------
// Name: render()
// Desc: 
//-----------------------------------------------------------------------------

void render( void )
{
  if (AmjuGLDX9::dd)
  {
//    AmjuGL::InitFrame(1.0f, 0, 0);
//    AmjuGL::BeginScene();
//    AmjuGL::EndScene();

    static float f = 0;
    f += 0.001f;
    AmjuGLDX9::dd->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                         D3DCOLOR_COLORVALUE(f, 1.0f - f,0.0f,1.0f), 1.0f, 0 );

    AmjuGLDX9::dd->BeginScene();

	// Render geometry here...

    AmjuGLDX9::dd->EndScene();

    AmjuGLDX9::dd->Present( NULL, NULL, NULL, NULL );
  }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  // TODO TEMP TEST
  return DefWindowProc( hWnd, uMsg, wParam, lParam );

    switch(uMsg) 
    {
    /*
    case WM_ACTIVATE:             // Watch For Window Activate Message
    {
      if (!HIWORD(wParam))          // Check Minimization State
      {
        active=TRUE;            // Program Is Active
      }
      else
      {
        active=FALSE;           // Program Is No Longer Active
      }

      return 0;               // Return To The Message Loop
    }

    case WM_MOUSEMOVE:
    {
      int xPos = LOWORD(lParam); 
      int yPos = HIWORD(lParam); 

      Engine::Instance()->MousePos(xPos, yPos);
      break;
    }

    case WM_LBUTTONDOWN:
    {
      // SetCapture() means we still get mouse events when the
      // mouse leaves the client area.
      SetCapture(hWnd);
      bool down = (wParam & MK_LBUTTON) != 0;
      bool ctrl = (wParam & MK_CONTROL) != 0;
      bool shift = (wParam & MK_SHIFT) != 0;

      Engine::Instance()->MouseLButton(down, ctrl, shift); 
      break;
    }

    case WM_LBUTTONUP:
    {
      // We must now release the mouse - end of SetCapture()
      // mouse ownership.
      ReleaseCapture();
      bool down = (wParam & MK_LBUTTON) != 0;
      bool ctrl = (wParam & MK_CONTROL) != 0;
      bool shift = (wParam & MK_SHIFT) != 0;

      Engine::Instance()->MouseLButton(down, ctrl, shift); 
      break;
    }

    case WM_KEYDOWN:
      if (!SpecialKey(wParam))
      {
         //Engine::Instance().handleKey(wParam, true);
      }
      KeyDown(wParam);
      break;

    case WM_KEYUP:
      KeyUp(wParam);
      break;

    case WM_DESTROY:
      wglMakeCurrent(hDC, NULL);
      wglDeleteContext(hRC);
      if(fullscreen)
        RestoreDisplayMode();
      PostQuitMessage( 0 );
      break;

    case WM_SIZE:
    {
      int w = LOWORD(lParam);
      int h = HIWORD(lParam);
      ChangeSize(w, h);
      break;
    }
    */

    case WM_CLOSE:
    {
      PostQuitMessage(0);
      return 0;
    }

    case WM_PAINT:
      if (true) //bReady) 
      {
        //Draw(hDC);
        render();
        ValidateRect(hWnd, NULL);
      }
      break;

    // No screen saver - necessary for joystick players.
    // Also don't go into power save mode! 
    case WM_SYSCOMMAND:
      if (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER)
      {
        break;
      }
      // Fall through

    default:
      return DefWindowProc( hWnd, uMsg, wParam, lParam );
    }

    return 0;
}            
