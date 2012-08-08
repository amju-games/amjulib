#ifndef GS_OPTIONS_H_INCLUDED
#define GS_OPTIONS_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"
#include "PlayerInfo.h"

namespace Amju 
{
class GSOptions : public GSGui
{
  GSOptions();
  friend class Singleton<GSOptions>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();
  virtual void OnDeactive(); 

  void OnKeyboard();

  void LoadSettingsFromPI(PlayerInfo*);
  void SaveSettingsToPI(PlayerInfo*);

private:
  // TODO Variables for volume, screen, kb, etc.
  float m_musicVol;
  float m_soundVol;
};
typedef Singleton<GSOptions> TheGSOptions;
} // namespace
#endif
