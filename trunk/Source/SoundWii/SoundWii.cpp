#include <AmjuFirst.h>
#include <iostream>
#include <aesndlib.h>
#include <asndlib.h>
#include <gcmodplay.h>
//#include <mp3player.h>
#include "SoundWii.h"
#include <File.h>
#include <ReportError.h>
#include <Pause.h>
#include <StringUtils.h>
#include <AmjuFinal.h>

//#define AMJU_WII_REPORT_SND

namespace Amju
{
static MODPlay play; 

SoundWii::SoundWii()
{
#ifdef AMJU_WII_REPORT_SND
  std::cout << "Creating Wii sound player.\n";
  PAUSE;
#endif

  ASND_Init();
  AESND_Init();
  MODPlay_Init(&play);
  ////MP3Player_Init();
}

SoundWii::~SoundWii()
{
}

bool SoundWii::PlayWav(const std::string& wavFile, float volume)
{
#ifdef AMJU_WII_REPORT_SND
  std::cout << "Playing wav: " << wavFile << "\n";
  PAUSE;
#endif

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
#ifdef AMJU_WII_REPORT_SND
  std::cout << "Attempting to play song: " << songFile << "\n";
  PAUSE;
#endif

  std::string filename = songFile;

  if (GetGlueFile())
  {
    // Find the start of the song in the glue file; and find the length
    uint32 songPos = 0;
    if (GetGlueFile()->GetSeekBase(filename, &songPos))
    {
#ifdef AMJU_WII_REPORT_SND
      std::string s = "Music: YES, it's in Glue File! ";
      s += filename;
      s += " size: " + ToString(GetGlueFile()->GetSize(filename)) + "\n";
      std::cout << s;
      PAUSE
#endif
    }
    else
    {
#ifdef AMJU_WII_REPORT_SND
      std::string s = "Music: not in Glue File: ";
      s += filename;
      std::cout << s << "\n";
      PAUSE
#endif
      return false;
    }
    uint32 length = GetGlueFile()->GetSize(filename);

    // Use GlueFileBinaryData to get the data without copying it
    GlueFileBinaryData data = GetGlueFile()->GetBinary(songPos, length);

    std::string ext = GetFileExt(songFile);
    if (ext == "mod")
    {
      //m_songData = (BinaryResource*)data.GetBuffer();
      MODPlay_SetMOD(&play, data.GetBuffer()); //m_songData->GetData()); 
      MODPlay_SetVolume(&play,63,63);
      MODPlay_Start(&play);

#ifdef AMJU_WII_REPORT_SND
  std::cout << "Playing song: " << songFile << "\n";
  PAUSE;
#endif
    }
    else if (ext == "mp3")
    {
//      MP3Player_PlayBuffer(data.GetBuffer(), length, NULL);
//      MP3Player_Volume(63);

#ifdef AMJU_WII_REPORT_SND
  std::cout << "Playing song: " << songFile << ", song length: " << length << "\n";
  PAUSE;
#endif
    }
    else
    {
#ifdef AMJU_WII_REPORT_SND
      std::string s = "Music: unexpected format: ";
      s += ext;
      ReportError(s);
      PAUSE
#endif
    }
  }
  else
  {
std::cout << "\n\n\n\n\nNO MUSIC GLUE FILE!!!\n";
Assert(0);
  }
  return true;
}

void SoundWii::StopSong()
{
#ifdef AMJU_WII_REPORT_SND
  std::cout << "Stopping song.\n";
  PAUSE;
#endif

  MODPlay_Stop(&play);
  /*
  if (MP3Player_IsPlaying())
  {
    MP3Player_Stop();
  }
  else
  {
    MODPlay_Stop(&play);
  }

  MODPlay_Stop(&play);

  */
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
