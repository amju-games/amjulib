#include <iostream>
#include <gcmodplay.h>
#include <asndlib.h> 
#include "SoundWii.h"
#include "File.h"

namespace Amju
{
static MODPlay play; 

SoundWii::SoundWii()
{
  ASND_Init(); 
  MODPlay_Init(&play);
}

SoundWii::~SoundWii()
{
}

bool SoundWii::PlayWav(const std::string& wavFile, float volume)
{
  // TODO 
  // Buffers are kept in memory until Resource Manager trashes them - so we
  // should inc the ref count here, then dec the count when we finish playing
  // the sound.
  // OR Have a resource group for wavs so we know that won't happen.

  // Use raw sound data file, ending .snd
  // Save data as 16 bit signed big endian stereo
  // (Use GoldWave, www.goldwave.com)
  std::string filename = "sound/" + wavFile + ".snd";

  // TODO The resource could go away while still be played.
  BinaryResource* res = (BinaryResource*)TheResourceManager::Instance()->
    GetRes(filename);

  s32 voice1 = SND_GetFirstUnusedVoice();

  ASND_SetVoice(
    voice1,
    VOICE_STEREO_16BIT, // TODO Try mono
    8000, // pitch,
    0, // delay, 
    (void*)(res->GetData()), //wavbuffer, 
    res->GetSize(), //size, 
    (int)(volume * 255.0f), //volume_l, 
    (int)(volume * 255.0f), //volume_r, 
    0 //callback);
  );

  return true;
}

bool SoundWii::PlaySong(const std::string& songFile)
{
  std::string filename = "sound/" + songFile;

  m_songData = (BinaryResource*)TheResourceManager::Instance()->
    GetRes(filename);

  if (!m_songData)
  {
    return false;
  }

  MODPlay_SetMOD(&play, m_songData->GetData()); 
  MODPlay_Start(&play);

  return true;
}

void SoundWii::StopSong()
{
  MODPlay_Stop(&play);

  // Soung data is resource, managed by ResourceManager
  // TODO Give ResourceManager a hint that we have finished with data
}

void SoundWii::Update()
{

}

void SoundWii::SetWavMaxVolume(float f)
{

}

void SoundWii::SetSongMaxVolume(float f)
{

}
}
