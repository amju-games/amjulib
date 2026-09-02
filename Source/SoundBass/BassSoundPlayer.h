/*
Amju Games source code (c) Copyright Juliet Colman 2006
*/

#pragma once

#ifdef AMJU_USE_BASS 

#include <SoundPlayerImpl.h>

namespace Amju
{
class BassSoundPlayer : public SoundPlayerImpl
{
public:
  BassSoundPlayer();
  ~BassSoundPlayer();

  void ShutDown() override;
  bool PlayWav(const std::string& wavFile, float volume = 1.0f) override;
  bool PlaySong(const std::string& songFile) override;
  void StopSong() override;
  void PauseSong() override;
  void ResumeSong() override;
  void SetSongSeekPosition(float seconds) override;
  float GetSongElapsedTimeSeconds() const override;

  // Preload song to reduce latency later on
  bool Preload(const std::string& songFile) override;
  // Clear preloaded songs when possible to reduce mem usage
  void ClearPreloadedSongs() override;

  void Update() override;
  void SetSongMaxVolume(float) override;

//#ifdef AMJU_USE_BASS_MIDI
  bool MidiSetSoundFont(const std::string& soundFontFile) override;
  bool MidiNoteOn(int note, int velocity) override;
  bool MidiNoteOff(int note) override;
//#endif // AMJU_USE_BASS_MIDI
   
private:
  unsigned int LoadSong(const std::string& songFile);
 
private:
  std::string m_lastSongName;

  // Channel for current song
  unsigned long m_chan;
};
}

#endif // AMJU_USE_BASS
