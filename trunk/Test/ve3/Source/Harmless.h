#ifndef HARMLESS_H_INCLUDED
#define HARMLESS_H_INCLUDED

#include "Ve1Object.h"

namespace Amju
{
class Harmless : public Ve1Object
{
public:
  static const char* TYPENAME;

  virtual const char* GetTypeName() const;
  virtual bool Load(File*);
  virtual void Update();

private:
  Vec2f m_size;
};
}

#endif

