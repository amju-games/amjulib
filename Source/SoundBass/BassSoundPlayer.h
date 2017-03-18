/*
Amju Games source code (c) Copyright Jason Colman 2006
*/

// Music theory test
#define AMJU_USE_MIDI
#define AMJU_USE_BASS

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

  virtual bool PlayWav(const std::string& wavFile, float volume = 1.0f);
  virtual bool PlaySong(const std::string& songFile);
  virtual void StopSong();
  virtual void Update();
  virtual void SetSongMaxVolume(float);

#ifdef AMJU_USE_MIDI
  virtual bool MidiSetSoundFont(const char* soundfont) override;
  virtual bool MidiNoteOn(int note) override;
  virtual bool MidiNoteOff(int note) override;
#endif // AMJU_USE_MIDI
    
private:
  std::string m_lastSongName;

  // Channel for current song
  unsigned long m_chan;
};
}

#endif
#endif // AMJU_USE_BASS

