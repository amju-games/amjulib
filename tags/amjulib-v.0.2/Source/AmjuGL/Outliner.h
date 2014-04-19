#ifndef AMJU_GL_OUTLINER_H_INCLUDED
#define AMJU_GL_OUTLINER_H_INCLUDED

#include <DrawDecorator.h>

namespace Amju
{
// TODO
// All kinds of special drawables (or just drawables?) have common base class.
// We only need one Create function, passing in ID of the type of drawable we want.
// The type we create also depends on the AmjuGL impl in use.
// Use AbstractFactory!

// For now
// Draws an outline around whatever is drawn
class Outliner : public DrawDecorator
{
public:
  // Pass this ID to AmjuGL::Create
  static const int TYPE_ID = 2;

  // TODO Set line width, colour, etc

};
}

#endif

