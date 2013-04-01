#include <AmjuFirst.h>
#include "SoundManager.h"
#include "Timer.h"
#include <AmjuFinal.h>

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
  if (!m_pImpl)
  {
    return;
  }

  m_pImpl->SetGlueFile(glueFile);
}

GlueFile* SoundManager::GetGlueFile()
{
  if (!m_pImpl)
  {
    return 0;
  }

  return m_pImpl->GetGlueFile();
}

bool SoundManager::PlayWav(const std::string& wav, float vol)
{
  AMJU_CALL_STACK;

  if (!m_pImpl)
  {
    return false;
  }

  float t = TheTimer::Instance()->GetElapsedTime();

  WavMap::iterator it = m_wavs.find(wav);
  if (it == m_wavs.end())
  {
    m_wavs[wav] = std::make_pair(1, t);
    return m_pImpl->PlayWav(wav, vol);
  }

  static const int MAX_NUM_SAME_WAV = 10; // TODO CONFIG
  // The idea is: if more than MAX_NUM_SAME_WAV copies of the same sound are playing,
  // wait 3 seconds before allowing any more copies of the sound to play.
  // After the 3 secs, up to MAX_NUM_SAME_WAV of the same sounds can be played again, etc.

  int& count = it->second.first;
  float& time = it->second.second;

  if (t - time > 3.0f) // TODO CONFIG
  {
    count = 0; // The count has expired, we can play more instances of this wav
  }

  if (count > MAX_NUM_SAME_WAV )  
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
  if (!m_pImpl)
  {
    return false;
  }

  if (m_song != songFile)
  {
    StopSong();
    m_song = songFile;
    return m_pImpl->PlaySong(songFile);
  }
  return true;
}

void SoundManager::StopSong()
{
  if (!m_pImpl)
  {
    return;
  }

  m_pImpl->StopSong();
  m_song = "";
}

void SoundManager::Update()
{
  if (!m_pImpl)
  {
    return;
  }

  m_pImpl->Update();
}

void SoundManager::SetWavMaxVolume(float f)
{
  m_maxWavVol = f; 

  if (!m_pImpl)
  {
    return;
  }

  m_pImpl->SetWavMaxVolume(f);
}

void SoundManager::SetSongMaxVolume(float f)
{ 
  m_maxSongVol = f; 

  if (!m_pImpl)
  {
    return;
  }

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
