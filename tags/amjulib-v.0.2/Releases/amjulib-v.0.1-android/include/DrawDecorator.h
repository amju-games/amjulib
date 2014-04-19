#ifndef AMJU_GL_DRAW_DECORATOR_H_INCLUDED
#define AMJU_GL_DRAW_DECORATOR_H_INCLUDED

#include "Drawable.h"

namespace Amju
{
typedef void (*DrawFunc)();

// Abstract base class for types which decorate a draw function.
// E.g. shadows draw the scene (twice); outlining, etc.
class DrawDecorator : public Drawable
{
public:
  DrawDecorator() : m_drawFunc(0) {}
  virtual ~DrawDecorator() {}
  
  void SetDrawFunc(DrawFunc df) { m_drawFunc = df; }

  virtual bool Init() = 0;
  virtual void Draw() = 0;

protected:
  DrawFunc m_drawFunc;
};

// Null behaviour is to just call the draw function, with no extra processing.
class DrawDecoratorNull : public DrawDecorator
{
public:
  virtual bool Init() { return true; }
  virtual void Draw() { m_drawFunc(); }
};
}

#endif

