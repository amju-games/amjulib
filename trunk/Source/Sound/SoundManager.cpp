#include "SoundManager.h"
#include "Timer.h"

namespace Amju
{
SoundManager::SoundManager()
{
  m_maxWavVol = 1.0f;
  m_maxSongVol = 1.0f;
}

SoundManager::~SoundManager()
{
}

void SoundManager::SetImpl(SoundPlayerImpl* pImpl)
{
  m_pImpl = pImpl;
}

void SoundManager::SetGlueFile(GlueFile* glueFile)
{
  m_pImpl->SetGlueFile(glueFile);
}

GlueFile* SoundManager::GetGlueFile()
{
  return m_pImpl->GetGlueFile();
}

bool SoundManager::PlayWav(const std::string& wav, float vol)
{
  AMJU_CALL_STACK;

  float t = TheTimer::Instance()->GetElapsedTime();

  WavMap::iterator it = m_wavs.find(wav);
  if (it == m_wavs.end())
  {
    m_wavs[wav] = std::make_pair(1, t);
    return m_pImpl->PlayWav(wav, vol);
  }

  // The idea is: if more than 3 copies of the same sound are playing,
  // wait 3 seconds before allowing any more copies of the sound to play.
  // After the 3 secs, up to 3 of the same sounds can be played again, etc.

  int& count = it->second.first;
  float& time = it->second.second;

  if (t - time > 3.0f) // TODO CONFIG
  {
    count = 0; // The count has expired, we can play more instances of this wav
  }

  if (count > 3 )  // TODO CONFIG
  {
    return false; // Too many instances of this wav playing
  }

  // Increment count and update time to now for this wav
  ++count;
  time = t;
  return m_pImpl->PlayWav(wav, vol);
}

bool SoundManager::PlaySong(const std::string& songFile)
{
  if (m_song != songFile)
  {
    return m_pImpl->PlaySong(songFile);
  }
  return true;
}

void SoundManager::StopSong()
{
  m_pImpl->StopSong();
  m_song = "";
}

void SoundManager::Update()
{
  m_pImpl->Update();
}

void SoundManager::SetWavMaxVolume(float f)
{
  m_maxWavVol = f; 
  m_pImpl->SetWavMaxVolume(f);
}

void SoundManager::SetSongMaxVolume(float f)
{ 
  m_maxSongVol = f; 
  m_pImpl->SetSongMaxVolume(f);
}

float SoundManager::GetWavMaxVolume() const 
{
  return m_maxWavVol; 
}

float SoundManager::GetSongMaxVolume() const 
{ 
  return m_maxSongVol; 
}
}
