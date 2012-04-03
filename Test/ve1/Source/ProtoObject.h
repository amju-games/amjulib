#ifndef PROTO_OBJECT_H_INCLUDED
#define PROTO_OBJECT_H_INCLUDED

#include "Ve1Object.h"

namespace Amju
{
// This type of object is what you get when you create a new object in Edit mode.
// When all required info is set, we send to server and can destroy this object.
// We then get the new object from the server.
class ProtoObject : public Ve1Object
{
public:
  ProtoObject();
  virtual const char* GetTypeName() const;
  virtual void OnLocationEntry();
  virtual AABB* GetAABB();
  virtual void Update();

  // TODO Can't use SetLocation() in base class because this object is not known by ObjectManager.
  // ..so maybe change that..
  void SetProtoLocation(int loc);

protected:
  AABB m_aabb;
};
}

#endif

