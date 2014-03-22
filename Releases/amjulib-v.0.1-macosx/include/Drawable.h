#ifndef AMJU_GL_DRAWABLE_H_INCLUDED
#define AMJU_GL_DRAWABLE_H_INCLUDED

#include <RCPtr.h>
#include <Factory.h>
#include <Singleton.h>

namespace Amju
{
// Abstract base class for drawable things created by the AmjuGL impl.
class Drawable : public RefCounted
{
public:
  virtual ~Drawable() {}
  
  virtual bool Init() = 0;
  virtual void Draw() = 0;
};

typedef RCPtr<Drawable> PDrawable;
typedef Factory<Drawable, int> DrawableFactory;
}

#endif

