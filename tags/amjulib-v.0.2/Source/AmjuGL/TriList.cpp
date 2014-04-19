#include <AmjuGL.h>
#include "TriList.h"

namespace Amju
{
// Convenience functions to make appropriate Tri List type from triangles
TriListStatic* MakeTriList()
{
  return (TriListStatic*)AmjuGL::Create(TriListStatic::DRAWABLE_TYPE_ID);
}

TriListStatic* MakeTriList(const AmjuGL::Tris& tris)
{
  TriListStatic* tls = (TriListStatic*)AmjuGL::Create(TriListStatic::DRAWABLE_TYPE_ID);
  if (tls)
  {
    tls->Set(tris);
  }
  return tls; 
}

}


