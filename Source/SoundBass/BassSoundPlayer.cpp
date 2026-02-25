/*
Amju Games source code (c) Copyright Juliet Colman 2006
*/

#include <AmjuFirst.h>

#ifdef AMJU_USE_BASS
#include <iostream>
#include "BassSoundPlayer.h"
#include <SoundManager.h>
#include "StringUtils.h"
#if defined(MACOSX)|| defined(AMJU_IOS)
#include "Bass2.4/Macosx/bass.h"
#include "Bass2.4/Macosx/bassmidi.h"
#endif
#ifdef WIN32
#include "Bass2.4/Win/bass.h"
#include "Bass2.4/Win/bassmidi.h"
#endif
#include <StringUtils.h>
#include <File.h>
#include <AmjuFinal.h>

#define BASS_DEBUG

namespace Amju
{
// For preloading: map song names to preloaded streams
static std::map<const std::string, unsigned int> s_preloaded;

void ReportError(const std::string&);

static void ReportBassError(const std::string& filename)
{
  std::string s = "BASS: Music: Can't play file: "; 
  s += filename;
  int errCode = BASS_ErrorGetCode();
  s += " Error code: ";
  s += ToString(errCode);
  ReportError(s);
}

BassSoundPlayer::BassSoundPlayer()
{
  m_chan = (DWORD)-1; 

  // check that expected version was loaded
  unsigned long ver = BASS_GetVersion();
#ifdef BASS_DEBUG
std::cout << "BASS version: " << ToHexString(ver).c_str() << "\n";
#endif

  if (HIWORD(ver) != BASSVERSION) 
  {
    std::string s = "Unexpected BASS version: expected " + ToHexString(BASSVERSION) + " got: " + ToHexString(HIWORD(ver));
    ReportError(s);
    return;
  }

  // These next two settings are intended to reduce latency.
  // Reduce the global playback buffer (default is 500ms)
  // 100ms is a safe "low" value; 30-50ms is "pro" territory.
  BASS_SetConfig(BASS_CONFIG_BUFFER, 100);

  // Reduce the update period (how often BASS checks if the buffer needs more data)
  // Default is 100ms. Lowering this to 10ms-20ms helps responsiveness.
  BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD, 20);

  // Setup output - default device
  if (!BASS_Init(-1, 44100, 0, 0, NULL))
  {
    ReportError("BASS: Can't initialize device");
  }

  SetSongMaxVolume(1.0f);
}

BassSoundPlayer::~BassSoundPlayer()
{
  // TODO Shut down to avoid stuttering or clicks
  // This seems to cause a crash, so best avoided
  //BASS_Free();
}

bool BassSoundPlayer::PlayWav(const std::string& wavFile, float volume)
{
  if (TheSoundManager::Instance()->GetWavMaxVolume() <= 0)
  {
#ifdef BASS_DEBUG
std::cout << "WAV volume is zero.\n";
#endif

    return true; // ok, not an error
  }

  // max no of simultaneous playbacks (of same wav ? or all wavs ?)
  static const int MAX_PLAYBACKS = 6;

  HSAMPLE hs = 0;
  if (GetGlueFile())
  {
#ifdef BASS_DEBUG
std::cout << "Playing wav in glue file\n";
#endif

    // Find the start of the wav in the glue file; and find the length
    uint32 wavPos = 0;
    if (!GetGlueFile()->GetSeekBase(wavFile, &wavPos))
    {
      std::string s = "BASS: Wav: not in Glue File: ";
      s += wavFile;
      ReportError(s);
    }
    uint32 wavLength = GetGlueFile()->GetSize(wavFile);

#ifdef BASS_DEBUG
std::cout << "WAV length is " << wavLength << "\n";
#endif

    // Use GlueFileBinaryData to get the data without copying it
    GlueFileBinaryData data = GetGlueFile()->GetBinary(wavPos, wavLength);
    hs = BASS_SampleLoad(
      TRUE, // in mem ?
      data.GetBuffer(), // start of wav in memory 
      0, // not used 
      wavLength,
      MAX_PLAYBACKS, 
      BASS_SAMPLE_OVER_POS); // flags
  }
  else
  {
    hs = BASS_SampleLoad(
      FALSE, // in mem ?
      (File::GetRoot() + wavFile).c_str(), // filename
      0, // file offset
      0, // "use all data up to end of file": 2 'iterators' defining a range ?
      MAX_PLAYBACKS, 
      BASS_SAMPLE_OVER_POS); // flags
  }

  if (!hs)
  {
    std::string s = "BASS: Wav: Failed to load sample: ";
    s += wavFile;
    ReportError(s);
    return false;
  } 
  HCHANNEL hc = BASS_SampleGetChannel(hs, FALSE);
  if (!hc)
  {
    std::string s = "BASS: Wav: Failed to get sample channel: ";
    s += wavFile;
    ReportError(s);
    return false;
  }

  BASS_ChannelPlay(hc, FALSE);

#if defined(MACOSX)|| defined(AMJU_IOS)
  // Set vol
  int vol = (int)(volume * TheSoundManager::Instance()->GetWavMaxVolume() * 100.0f);
  //BASS_ChannelSetAttribute(hc, -1, vol, -1);
  BASS_ChannelSetAttribute(m_chan, BASS_ATTRIB_VOL, vol);
#endif

#ifdef BASS_DEBUG
std::cout << "Apparently played wav ok!\n";
#endif

  return true;
}

unsigned int BassSoundPlayer::LoadSong(const std::string& songFile)
{
  unsigned int res = 0;

  // If Glue File is set, use it to load song into memory.
  // Else use file.

  // The Bass load function is different for some music file types.
  auto extension = GetFileExt(songFile);

  // Currently expecting song file to be a .it file, or a .ogg file.
  // .mod files are treated like .it files.
  // .mp3, m4a files are treated like .oggs.
  bool isItFile = (extension == "it" || extension == "mod");

  // Loading song from Glue file, or file system?
  if (GetGlueFile())
  {
#ifdef _DEBUG
std::cout << "BASS: using glue file.\n";
#endif
    // Find the start of the song in the glue file, and find the length
    uint32 songPos = 0;
    if (!GetGlueFile()->GetSeekBase(songFile, &songPos))
    {
      ReportError("BASS: Music: not in Glue File: " + songFile);
      return 0;
    }

    // Use GlueFileBinaryData to get the data without copying it
    uint32 length = GetGlueFile()->GetSize(songFile);
    GlueFileBinaryData data = GetGlueFile()->GetBinary(songPos, length);

    if (isItFile) // .it?
    {
      if (!(res = BASS_MusicLoad(
        TRUE, // in memory ?
        data.GetBuffer(), // start of song data 
        0, // offset
        length, // length
        BASS_SAMPLE_LOOP | BASS_MUSIC_SURROUND | BASS_MUSIC_PRESCAN, 
        // BASS_MUSIC_PRESCAN is there so we can seek to a position.
        // NB BASS_SAMPLE_LOOP is looping the song! Should be a param!
        0)))  // sample rate - 0 => use default value
      {
        ReportBassError(songFile + " (in glue file)");
      }
    }
    else
    {
      // .ogg file, in Glue file.
      if (!(res = BASS_StreamCreateFile(
        TRUE,  // in memory
        data.GetBuffer(), // start of song data 
        0, // offset
        length, // length
        0))) // flags
      {
        ReportBassError(songFile + " (in glue file)");
      }
    }
  }
  else if (isItFile)
  {
    // BASS_MUSIC_PRESCAN is there so we can seek to a position.
    if (!(res = BASS_MusicLoad(
      FALSE, // mem ?
      (File::GetRoot() + songFile).c_str(), // file
      0, // offset
      0, // length
      // NB BASS_SAMPLE_LOOP is looping the song! Should be a param!
      BASS_SAMPLE_LOOP | BASS_MUSIC_SURROUND | BASS_MUSIC_PRESCAN, 
      0)))  // sample rate - 0 => use default value
    {
      ReportBassError(songFile);
    }
  }
  else
  {
    // .Ogg file
    if (!(res = BASS_StreamCreateFile(
      FALSE, // file, not mem?
      (File::GetRoot() + songFile).c_str(), // file
      0, // offset
      0, // length
      0))) // flags
    {
      ReportBassError(songFile);
    }
  }

  return res;
}

bool BassSoundPlayer::Preload(const std::string& songFile)
{
std::cout << "Preloading " << songFile << "... ";

  auto it = s_preloaded.find(songFile);
  if (it != s_preloaded.end()) 
  {
std::cout << " already loaded!\n";
    return true;
  }

std::cout << "\n";
  auto stream = LoadSong(songFile);
  if (stream == 0) 
  {
std::cout << "Preload failed :( \n";
    return false;
  }
    
  s_preloaded[songFile] = stream;
std::cout << ".. preloaded ok!\n";
  return true; 
}

void BassSoundPlayer::ClearPreloadedSongs()
{
  for (const auto& [name, stream] : s_preloaded)
  {
    BASS_StreamFree(stream);
  }
  s_preloaded.clear();
}

bool BassSoundPlayer::PlaySong(const std::string& songFile)
{
  // Stop old song, don't free the stream tho
  StopSong();

  // Play song even if song vol is currently zero - it may be turned up.

#ifdef _DEBUG
std::cout << "BASS: playing new song: " << songFile.c_str() << "\n";
#endif

  auto it = s_preloaded.find(songFile);
  if (it == s_preloaded.end())
  {
std::cout << " ..not preloaded, loading now...\n";
    m_chan = LoadSong(songFile);
    if (m_chan != 0)
    {
      s_preloaded[songFile] = m_chan;
    }
  }
  else
  {
std::cout << " ..song already preloaded!\n";
    m_chan = it->second;
  }

#if defined(MACOSX)|| defined(AMJU_IOS)
  // Set vol
  int vol = static_cast<int>
    (TheSoundManager::Instance()->GetSongMaxVolume() * 100.0f);
  BASS_ChannelSetAttribute(m_chan, BASS_ATTRIB_VOL, vol);
#endif

  BASS_ChannelPlay(m_chan, FALSE);
  
#ifdef USE_REVERB
  // Set some reverb..?
  int REVERB_PRIORITY = 1;
  auto fxHandle = BASS_ChannelSetFX(m_chan, BASS_FX_DX8_REVERB, REVERB_PRIORITY);
  float REVERB_IN_GAIN = 0.0f;
  float REVERB_MIX = -0.5f;
  float REVERB_TIME_MS = 200.0f;
  float REVERB_HR_REVERB_TIME_RATIO = 0.1f;
  BASS_DX8_REVERB reverbParams { REVERB_IN_GAIN, REVERB_MIX, REVERB_TIME_MS, REVERB_HR_REVERB_TIME_RATIO };
  BASS_FXSetParameters(fxHandle, &reverbParams);
#endif

#ifdef BASS_DEBUG
std::cout << "BASS: new song: " << songFile.c_str() << " chan: " << m_chan << "\n";
#endif

  // Remember song filename, so we can restart if necessary
  m_lastSongName = songFile;

  return true;
}

void BassSoundPlayer::StopSong()
{
  if (m_chan == (unsigned int)-1)
  {
    return;
  }

#ifdef BASS_DEBUG
std::cout << "BASS: Stopping song on channel " << m_chan << "\n";
#endif
  BASS_ChannelStop(m_chan);

  // Don't free the stream, we keep it in the preloaded pool.
  //BASS_StreamFree(m_chan);

  // But this is ok - should it be zero tho?
  m_chan = -1;
 
  m_lastSongName.clear();
}

void BassSoundPlayer::PauseSong() 
{
  if (m_chan == (unsigned int)-1)
  {
    return;
  }

  BASS_ChannelPause(m_chan);
}

void BassSoundPlayer::ResumeSong() 
{
  if (m_chan == (unsigned int)-1)
  {
    return;
  }

  BASS_ChannelPlay(m_chan, FALSE);
}

void BassSoundPlayer::SetSongSeekPosition(float seconds)
{
  if (m_chan == (unsigned int)-1)
  {
    return;
  }

  // Convert seconds to a byte position
  QWORD bytes = BASS_ChannelSeconds2Bytes(m_chan, seconds); 

  // Set the position
  BASS_ChannelSetPosition(m_chan, bytes, BASS_POS_BYTE);
}

void BassSoundPlayer::Update()
{
}

void BassSoundPlayer::SetSongMaxVolume(float f)
{
  SoundPlayerImpl::SetSongMaxVolume(f); 

  if (m_chan == (unsigned int)-1)
  {
    return;
  }

  float newVol = f * 100.0f;

  BASS_ChannelSetAttribute(m_chan, BASS_ATTRIB_VOL, newVol);

#ifdef AMJU_IOS
  // TODO Is this necessary?
  if (newVol > 0 && !m_lastSongName.empty())
  {
    PlaySong(m_lastSongName);
  }
#endif
}

#ifdef AMJU_USE_BASS_MIDI
static HSTREAM str = 0;
bool BassSoundPlayer::MidiSetSoundFont(const std::string& soundfont)
{
std::cout << "Setting sound font: " << soundfont << "\n";
  // create a MIDI stream - No FX to reduce latency. 
  str = BASS_MIDI_StreamCreate(16, BASS_SAMPLE_FLOAT | BASS_MIDI_NOFX, 0);
#ifdef MACOSX
  // Attempting to reduce latency...
  BASS_ChannelSetAttribute(str, BASS_ATTRIB_NOBUFFER, 1);
#endif

  BASS_MIDI_FONT font;
  font.font=BASS_MIDI_FontInit(soundfont.c_str(), 0);
  if (font.font) 
  { 
    font.preset=-1; // all presets
    font.bank=0; // default bank(s)
    BASS_MIDI_StreamSetFonts(0,&font,1); // make it the default
    BASS_MIDI_StreamSetFonts(str,&font,1); // apply to current stream too

    std::cout << "Sound font seems to have been set ok.\n";
  }
  else
  {
//#ifdef _DEBUG
    std::cout << "Failed to load soundfont: " << soundfont << "\n";
    std::cout << BASS_ErrorGetCode() << "\n";
//#endif
    return false;
  }

  // 10ms update period 
//  BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD,10);

//  BASS_SetConfig(BASS_CONFIG_BUFFER, 64); 
  // 500 default; we want to decrease latency but not
  //  break up the sound

  BASS_ChannelPlay(str, FALSE /* don't restart the channel */);

  return true;
}

bool BassSoundPlayer::MidiNoteOn(int note, int velocity)
{
  BASS_MIDI_StreamEvent(str, 0, MIDI_EVENT_NOTE, MAKEWORD(note, velocity));
  return true;
}

bool BassSoundPlayer::MidiNoteOff(int note)
{
  BASS_MIDI_StreamEvent(str, 0, MIDI_EVENT_NOTE, MAKEWORD(note, 0));
  return true;
}
#endif // AMJU_USE_BASS_MIDI
}
#endif // AMJU_USE_BASS

