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
  Ve1Object();

  virtual void Draw() { } // we use Scene Graph to draw

  // Called when object is 'activated' because the local player changes location to 
  //  where this object is. 
  virtual void OnLocationEntry() = 0;

  // Set a state (key, val) pair
  virtual void Set(const std::string& key, const std::string& val);
  // TODO Or use Variable ?
//  virtual void Set(const std::string& key, int val);

  // Set the position - if a character, move to this pos over time, don't instantly set.
  virtual void MoveTo(const Vec3f& pos, int location);

  // Set menu items appropriate to this object type
  virtual void SetMenu(GuiMenu*) { }

  // This kind of object live in one location 
  int GetLocation() const;
  void SetLocation(int loc);

protected:
  // Location ID. -1 means the object doesn't live in one particular location, it's in every location, 
  //  i.e. it's in whatever the current location is.
  int m_location;
};

// Keep track of keys used to set object properties
#define SET_KEY(x) x
}

#endif

