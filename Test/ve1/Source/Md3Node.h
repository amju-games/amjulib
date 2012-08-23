#ifndef MD3_NODE_H_INCLUDED
#define MD3_NODE_H_INCLUDED

#include "Ve1Character.h"

namespace Amju
{
class Md3Node : public Ve1Character
{
public:
  virtual void Draw();
  virtual void SetFromCharacterName(const std::string& characterName);
  virtual void SetAnim(const std::string& animName);

private:
  // Md3 mesh
};

}

#endif

