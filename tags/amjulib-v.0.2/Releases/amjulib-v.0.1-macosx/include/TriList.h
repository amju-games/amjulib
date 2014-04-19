#ifndef TRI_LIST_H_INCLUDED
#define TRI_LIST_H_INCLUDED

#include "AmjuGL.h"

namespace Amju
{
class TriList : public Drawable
{
public:
};

class TriListStatic : public TriList
{
public:
  enum { DRAWABLE_TYPE_ID = 3 };
  virtual void Set(const AmjuGL::Tris& tris) = 0;
};

TriListStatic* MakeTriList();
TriListStatic* MakeTriList(const AmjuGL::Tris& tris);

class TriListDynamic : public TriList
{
public:
  enum { DRAWABLE_TYPE_ID = 4 };
  virtual void Set(const AmjuGL::Tris& tris) = 0;
};

// TODO in future prefer tri strips
class TriStrip : public Drawable
{
};
}

#endif

