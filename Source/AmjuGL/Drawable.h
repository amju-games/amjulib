#ifndef AMJU_GL_DRAWABLE_H_INCLUDED
#define AMJU_GL_DRAWABLE_H_INCLUDED

#include <RCPtr.h>

namespace Amju
{
// Abstract base class for drawable things created by the AmjuGL impl.
class Drawable : public RefCounted
{
public:
  Drawable() : m_drawFunc(0) {}
  virtual ~Drawable() {}
  
  typedef void (*DrawFunc)();
  void SetDrawFunc(DrawFunc df) { m_drawFunc = df; }

  virtual bool Init() = 0;
  virtual void Draw() = 0;

protected:
  DrawFunc m_drawFunc;
};

typedef RCPtr<Drawable> PDrawable;


// Null behaviour is to just call the draw function, with no extra processing.
class DrawableNull : public Drawable
{
public:
  virtual bool Init() { return true; }
  virtual void Draw() { m_drawFunc(); }
};
}

#endif

