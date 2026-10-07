// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026

#ifndef OUTLINER_OPENGL_H_INCLUDED
#define OUTLINER_OPENGL_H_INCLUDED

#include <Outliner.h>

namespace Amju
{
class OutlinerOpenGL : public Outliner
{
public:
  virtual bool Init();
  virtual void Draw();
};
}

#endif

