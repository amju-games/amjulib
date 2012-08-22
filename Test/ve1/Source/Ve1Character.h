#ifndef VE1_CHARACTER_H
#define VE1_CHARACTER_H

#include <SceneNode.h>
#include "Ve1Object.h"

namespace Amju
{
// TODO Rename to Ve1CharacterNode
class Ve1Character : public SceneNode 
{
public:
  Ve1Character() {} //Ve1Object* obj) : m_obj(obj) {}

  virtual bool Load(File* f);
  virtual void Draw() = 0;

  // Md2, Md3 etc do these in different ways
  virtual void SetFromCharacterName(const std::string& characterName) = 0;
  virtual void SetAnim(const std::string& animName) = 0;

  void SetObj(Ve1Object* obj) { m_obj = obj; }

protected:
  RCPtr<Ve1Object> m_obj;
};

}

#endif

