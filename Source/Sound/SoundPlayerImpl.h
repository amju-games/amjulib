/*
Amju Games source code (c) Copyright Jason Colman 2004
*/

#ifndef SOUND_PLAYER_IMPL_H_INCLUDED
#define SOUND_PLAYER_IMPL_H_INCLUDED

#include <string>
#include <RCPtr.h>
#include "GlueFile.h"

namespace Amju
{
// Base class - doesn't do anything. Subclasses implement sound-playing for 
// different platforms.
class SoundPlayerImpl : public RefCounted
{
public:
  // Get/Set the Glue File used by the file callbacks.
  // This lets us set a different Glue File to the one used for the bulk
  // of the game data.
  void SetGlueFile(GlueFile* glueFile) { m_pGlueFile = glueFile; }
  GlueFile* GetGlueFile() { return m_pGlueFile.GetPtr(); }

  SoundPlayerImpl() {}
  virtual ~SoundPlayerImpl() {}

  // Play a .WAV file. Return immediately, with false if sound can't be played.
  // volume parameter should vary between 0 (silent) and 1.0 (full volume).
  virtual bool PlayWav(const std::string& wavFile, float volume = 1.0f) { return true; }

  // Play a song file. Return immediately, with false if file can't be played.
  // Song files are expected to be MOD/S3M etc format.
  // MIDI is currently NOT supported.
  virtual bool PlaySong(const std::string& songFile) { return true; }

  // Stop playing the current song. This should be ok to call if the file wasn't
  // being played anyway. 
  virtual void StopSong() {}

  // Call this every time round the game loop. This is so we can tell the
  // Engine when a sound has finished: it allows us to call the 
  // Callback by polling if we are unable to do it any other way.
  virtual void Update() {}

  // Set volume for WAVS, between 1.0 (full volume) and 0 (silent)
  virtual void SetWavMaxVolume(float f) {}

  // Set volume for songs, between 1.0 (full volume) and 0 (silent) 
  virtual void SetSongMaxVolume(float f) {}

protected:
  RCPtr<GlueFile> m_pGlueFile;
};

typedef RCPtr<SoundPlayerImpl> PSoundPlayerImpl;
}
#endif
