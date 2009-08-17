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
  // TODO Free old buffer

  // Use raw sound data file, ending .snd
  // Save data as 16 bit signed big endian stereo
  // (Use GoldWave, www.goldwave.com)
  std::string filename = File::GetRoot() + "sound/" + wavFile + ".snd";

  // TODO The resource could go away while still be played.
  // Have a resource group for wavs so we know that won't happen.
  ResourceFile* res = (ResourceFile*)TheResourceManager::Instance()->
    GetRes(filename, &ResourceFileLoader);

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
  std::string filename = File::GetRoot() + "sound/" + songFile;

  m_songData = (ResourceFile*)TheResourceManager::Instance()->
    GetRes(filename, &ResourceFileLoader);

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
