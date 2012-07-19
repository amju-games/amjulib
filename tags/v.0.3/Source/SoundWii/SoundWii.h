#ifndef SOUND_WII_H
#define SOUND_WII_H

#include "SoundPlayerImpl.h"
#include "BinaryResource.h"

namespace Amju
{
class SoundWii : public SoundPlayerImpl
{
public:
  SoundWii();
  ~SoundWii();
  virtual bool PlayWav(const std::string& wavFile, float volume = 1.0f);
  virtual bool PlaySong(const std::string& songFile);
  virtual void StopSong();
  virtual void Update();
  virtual void SetWavMaxVolume(float f);
  virtual void SetSongMaxVolume(float f);
  
private:
  RCPtr<BinaryResource> m_songData;  
};
}

#endif

