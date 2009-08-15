#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ogcsys.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <fat.h>
#include <gcmodplay.h>
#include <iostream>
#include <asndlib.h> // j.c. from comments

#include "SoundWii.h"
#include "File.h"
#include "Pause.h"
#include "ResourceFile.h"

namespace Amju
{
// TODO Pasted from sample code
// Modplay
static MODPlay play; 
//static size_t mod_size;
//static char * songBuffer;
//static size_t result;

SoundWii::SoundWii()
{
  ASND_Init(); // j.c.
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

  ResourceFile* res = (ResourceFile*)TheResourceManager::Instance()->
    GetRes(filename, &ResourceFileLoader);

  /*
  FILE *f = fopen(filename.c_str(), "rb");

  if (f == NULL) 
  {
    printf("Didn't open file!\n");
    PAUSE;
    fclose(f);
    return false;
  }

  fseek (f , 0, SEEK_END);
  size_t size = ftell (f);
  rewind(f);

  // allocate memory to contain the whole file:
  char* wavbuffer = (char*) malloc (size);
  if (wavbuffer == NULL) 
  { 
    perror ("Memory error\n"); 
    PAUSE;
  }

  // copy the file into the buffer:
  result = fread (wavbuffer,1,size,f);
  if (result != size) 
  { 
    perror ("Error reading sound file\n"); 
    PAUSE;
  }

  fclose(f);
  */

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
  // Free old buffer
  //if (songBuffer)
  //{
  //  free(songBuffer);
  //}

  std::string filename = File::GetRoot() + "sound/" + songFile;

  ResourceFile* res = (ResourceFile*)TheResourceManager::Instance()->
    GetRes(filename, &ResourceFileLoader);

  /*
  FILE *f = fopen(filename.c_str(), "rb");

  if (f == NULL) 
  {
    printf("Didn't open file!\n");
    fclose(f);
    return false;
  }
  else 
  {
    fseek (f , 0, SEEK_END);
    mod_size = ftell (f);
    rewind(f);

    // allocate memory to contain the whole file:
    songBuffer = (char*) malloc (sizeof(char)*mod_size);
    if (songBuffer == NULL) 
    { 
      perror ("Memory error\n"); 
    }

    // copy the file into the buffer:
    result = fread (songBuffer, 1, mod_size, f);
    if (result != mod_size) 
    { 
      perror ("Reading error\n"); 
    }

    fclose(f);
    */

  MODPlay_SetMOD(&play, res->GetData()); //songBuffer);
  MODPlay_Start(&play);
  
  return true;
}

void SoundWii::StopSong()
{
  MODPlay_Stop(&play);
  if (songBuffer)
  {
    free(songBuffer);
  }
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
