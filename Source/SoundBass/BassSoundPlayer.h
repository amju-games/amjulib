/*
Amju Games source code (c) Copyright Jason Colman 2006
*/

#ifdef AMJU_USE_BASS 
#ifndef AMAJU_BASS_SOUND_PLAYER_H_INCLUDED
#define AMAJU_BASS_SOUND_PLAYER_H_INCLUDED

#include <SoundPlayerImpl.h>

namespace Amju
{
class BassSoundPlayer : public SoundPlayerImpl
{
public:
  BassSoundPlayer();
  ~BassSoundPlayer();

  bool PlayWav(const std::string& wavFile, float volume = 1.0f) override;
  bool PlaySong(const std::string& songFile) override;
  void StopSong() override;
  void Update() override;
  void SetSongMaxVolume(float) override;

#ifdef AMJU_USE_BASS_MIDI
  bool MidiSetSoundFont(const char* soundfont) override;
  bool MidiNoteOn(int note) override;
  bool MidiNoteOff(int note) override;
#endif // AMJU_USE_BASS_MIDI
    
private:
  std::string m_lastSongName;

  // Channel for current song
  unsigned long m_chan;
};
}

#endif
#endif // AMJU_USE_BASS

