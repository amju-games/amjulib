#include <AmjuFirst.h>
#include <iostream>
#include <gcmodplay.h>
#include <asndlib.h> 
#include "SoundWii.h"
#include "File.h"
#include <ReportError.h>
#include <Pause.h>
#include <AmjuFinal.h>

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
  // Use raw sound data file, ending .snd
  // Save data as 16 bit signed big endian stereo
  // (Use GoldWave, www.goldwave.com)
  std::string filename = wavFile; 

  s32 voice1 = SND_GetFirstUnusedVoice();

  if (GetGlueFile())
  {
    // Find the start of the SND in the glue file; and find the length
    uint32 songPos = 0;
    if (!GetGlueFile()->GetSeekBase(filename, &songPos))
    {
#ifdef AMJU_WII_REPORT_SND
      std::string s = "SND: not in Glue File: ";
      s += filename;
      ReportError(s);
      PAUSE
#endif
      return false;
    }
    uint32 length = GetGlueFile()->GetSize(filename);

    // Use GlueFileBinaryData to get the data without copying it
    GlueFileBinaryData data = GetGlueFile()->GetBinary(songPos, length);

    ASND_SetVoice(
      voice1,
      VOICE_STEREO_16BIT, // TODO Try mono
      16000, // pitch,
      0, // delay, 
      (void*)(data.GetBuffer()), //res->GetData()), //wavbuffer, 
      length, //res->GetSize(), //size, 
      (int)(volume * 255.0f), //volume_l, 
      (int)(volume * 255.0f), //volume_r, 
      0 //callback);
    );
  }

  return true;
}

bool SoundWii::PlaySong(const std::string& songFile)
{
  std::string filename = songFile;

  if (GetGlueFile())
  {
    // Find the start of the song in the glue file; and find the length
    uint32 songPos = 0;
    if (!GetGlueFile()->GetSeekBase(filename, &songPos))
    {
#ifdef AMJU_WII_REPORT_SND
      std::string s = "Music: not in Glue File: ";
      s += filename;
      ReportError(s);
      PAUSE
#endif
      return false;
    }
    uint32 length = GetGlueFile()->GetSize(filename);

    // Use GlueFileBinaryData to get the data without copying it
    GlueFileBinaryData data = GetGlueFile()->GetBinary(songPos, length);

    //m_songData = (BinaryResource*)data.GetBuffer();
    MODPlay_SetMOD(&play, data.GetBuffer()); //m_songData->GetData()); 
    MODPlay_Start(&play);
  }

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
