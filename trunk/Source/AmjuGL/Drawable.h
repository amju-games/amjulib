#ifndef AMJU_GL_DRAWABLE_H_INCLUDED
#define AMJU_GL_DRAWABLE_H_INCLUDED

#include <RCPtr.h>
#include <Factory.h>
#include <Singleton.h>

namespace Amju
{
typedef void (*DrawFunc)();

// Abstract base class for drawable things created by the AmjuGL impl.
class Drawable : public RefCounted
{
public:
  Drawable() : m_drawFunc(0) {}
  virtual ~Drawable() {}
  
  void SetDrawFunc(DrawFunc df) { m_drawFunc = df; }

  virtual bool Init() = 0;
  virtual void Draw() = 0;

protected:
  DrawFunc m_drawFunc;
};

typedef RCPtr<Drawable> PDrawable;


typedef Singleton<Factory<Drawable, int> > TheDrawableFactory;


// Null behaviour is to just call the draw function, with no extra processing.
class DrawableNull : public Drawable
{
public:
  virtual bool Init() { return true; }
  virtual void Draw() { m_drawFunc(); }
};
}

#endif

