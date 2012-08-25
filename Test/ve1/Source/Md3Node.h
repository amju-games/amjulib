#ifndef MD3_NODE_H_INCLUDED
#define MD3_NODE_H_INCLUDED

#include <Md3.h>
#include "Ve1Character.h"

namespace Amju
{
class Md3Node : public Ve1Character
{
public:
  Md3Node();
  virtual void Draw();
  virtual void Update();
  virtual void SetFromCharacterName(const std::string& characterName);
  virtual void SetAnim(const std::string& animName);

private:
  // Md3 mesh - owned by ResourceManager
  RCPtr<CModelMD3> m_model;
};

}

#endif

