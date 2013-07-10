#ifndef TREASURE_H_INCLUDED
#define TREASURE_H_INCLUDED

#include "Ve1Object.h"

namespace Amju
{
class Player;

class Treasure : public Ve1Object
{
public:
  static const char* TYPENAME;

  Treasure();
  virtual const char* GetTypeName() const;
  virtual bool Load(File*);
  virtual void Update();

  void OnCollidePlayer(Player* p);

private:
  int m_points;
};
}

#endif
