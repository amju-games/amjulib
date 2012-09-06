#ifndef GS_WAITFORNEWLOCATION_H_INCLUDED
#define GS_WAITFORNEWLOCATION_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
// Go to this state when we start or enter a new location. 
// Wait until we have created the game objects which are critical to entering a new location.
// Mainly this is the Terrain for the location.

class GSWaitForNewLocation : public GSGui
{
  GSWaitForNewLocation();
  friend class Singleton<GSWaitForNewLocation>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  // Call to show user that we are downloading stuff
  void SetLatestDownloadedFilename(const std::string& filename);

};
typedef Singleton<GSWaitForNewLocation> TheGSWaitForNewLocation;
} // namespace
#endif
