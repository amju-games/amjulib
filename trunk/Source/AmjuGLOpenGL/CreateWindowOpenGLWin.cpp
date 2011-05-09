#if defined(WIN32) && defined(AMJU_USE_SDL)

#include "AmjuGL-OpenGL.h"
// Use SDL for now
#include <SDL.h>
#include "ReportError.h"
#include "AmjuAssert.h"

namespace Amju
{
bool AmjuGLOpenGL::CreateWindow(AmjuGLWindowInfo* w)
{
  Assert(w);
  const SDL_VideoInfo* video;
 
  if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) 
  {
    std::string err = std::string("Couldn't initialize SDL: ") + SDL_GetError();
    ReportError(err);
    return false;
  }
         
  /* Quit SDL properly on exit */
  atexit(SDL_Quit);
 
  /* Get the current video information */
  video = SDL_GetVideoInfo();
  if (!video) 
  {
    std::string err = std::string("Couldn't get video information: ") + SDL_GetError();
    ReportError(err);
    return false;
  }

  //SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);  // not needed ?
  //SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 1); ???

  // TODO fullscreen option
  int mode = SDL_OPENGL;
  if (w->IsFullScreen())
  {
    mode |= SDL_FULLSCREEN;
  }

  if (SDL_SetVideoMode(w->GetWidth(), w->GetHeight(), 
    0,   // ignored..? 0 same as video->vfmt->BitsPerPixel ?
    mode) == 0) 
  {
    std::string err = std::string("Couldn't set video mode: ") + SDL_GetError();
    ReportError(err);
    return false;
  }

  return true;
}
}

#endif // WIN32

