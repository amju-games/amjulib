#ifndef SOUND_SDL_MIXER_H
#define SOUND_SDL_MIXER_H

#include "SoundPlayerImpl.h"

namespace Amju
{
// Implement SoundPlayerImpl interface using SDL_Mixer lib
class SoundSDLMixer : public SoundPlayerImpl
{
public:
  SoundSDLMixer();
  ~SoundSDLMixer();
  virtual bool PlayWav(const std::string& wavFile, float volume = 1.0f);
  virtual bool PlaySong(const std::string& songFile);
  virtual void StopSong();
  virtual void Update();
  virtual void SetWavMaxVolume(float f);
  virtual void SetSongMaxVolume(float f);

private:
};
}

#endif // SOUND_SDL_MIXER_H
