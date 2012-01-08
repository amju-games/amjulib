#ifndef VE1_OBJECT_H
#define VE1_OBJECT_H

#include <GameObject.h>
#include <Xml/XmlNodeInterface.h>
#include <string>
#include <GuiMenu.h>

namespace Amju
{
class Ve1Object : public GameObject
{
public:
  // Set a state (key, val) pair
  virtual void Set(const std::string& key, const std::string& val);
  // TODO Or use Variable ?
//  virtual void Set(const std::string& key, int val);

  // Set the position - if a character, move to this pos over time, don't instantly set.
  virtual void MoveTo(const Vec3f& pos);

  // Set menu items appropriate to this object type
  virtual void SetMenu(GuiMenu*) { }

protected:
};

// Keep track of keys used to set object properties
#define SET_KEY(x) x
}

#endif

