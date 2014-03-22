#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <Singleton.h>
#include <NonCopyable.h>
#include "SoundPlayerImpl.h"
#include <GlueFile.h>

namespace Amju
{
class SoundManager : public NonCopyable
{
private:
  SoundManager();
  friend class Singleton<SoundManager>;

public:
  ~SoundManager();

  void SetImpl(SoundPlayerImpl*);

  // Get/Set the Glue File used by the file callbacks.
  // This lets us set a different Glue File to the one used for the bulk
  // of the game data.
  void SetGlueFile(GlueFile* glueFile);
  GlueFile* GetGlueFile();

  // Play a .WAV file. Return immediately, with false if sound can't be played.
  // volume parameter should vary between 0 (silent) and 1.0 (full volume).
  bool PlayWav(const std::string& wavFile, float volume = 1.0f);

  // Play a song file. Return immediately, with false if file can't be played.
  // Song files are expected to be MOD/S3M etc format.
  // MIDI is currently NOT supported.
  bool PlaySong(const std::string& songFile);

  // Stop playing the current song. This should be ok to call if the file wasn't
  // being played anyway. 
  void StopSong();

  // Call this every time round the game loop. This is so we can tell the
  // Engine when a sound has finished: it allows us to call the 
  // Callback by polling if we are unable to do it any other way.
  void Update();

  // Set volume for WAVS, between 1.0 (full volume) and 0 (silent)
  void SetWavMaxVolume(float f);

  // Set volume for songs, between 1.0 (full volume) and 0 (silent) 
  void SetSongMaxVolume(float f);

  float GetWavMaxVolume() const;
  float GetSongMaxVolume() const;

private:
  PSoundPlayerImpl m_pImpl;

  float m_maxWavVol;
  float m_maxSongVol;

  // Map of the number of copies of each wav being played, and the last 
  // time the wav was played.
  typedef std::map<std::string, std::pair<int, float> > WavMap;
  WavMap m_wavs;

  // Filename of playing song
  std::string m_song;
};

typedef Singleton<SoundManager> TheSoundManager;
}

#endif
