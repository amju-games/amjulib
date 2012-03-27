#ifndef GS_EDIT_LOCATION_H_INCLUDED
#define GS_EDIT_LOCATION_H_INCLUDED

#include "GSNewLocation.h"

namespace Amju
{
class GSEditLocation : public GSNewLocation
{
private:
  virtual void RequestNewId();
  virtual void OnActive();
};

typedef Singleton<GSEditLocation> TheGSEditLocation;
}

#endif

