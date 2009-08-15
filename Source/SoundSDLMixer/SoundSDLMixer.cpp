#include "SoundSDLMixer.h"
#include <SDL_mixer.h>

namespace Amju
{
SoundSDLMixer::SoundSDLMixer()
{

}

SoundSDLMixer::~SoundSDLMixer()
{

}

bool SoundSDLMixer::PlayWav(const std::string& wavFile, float volume)
{

  return true;
}

bool SoundSDLMixer::PlaySong(const std::string& songFile)
{

  return true;
}

void SoundSDLMixer::StopSong()
{

}

void SoundSDLMixer::Update()
{

}

void SoundSDLMixer::SetWavMaxVolume(float f)
{

}

void SoundSDLMixer::SetSongMaxVolume(float f)
{

}
}
