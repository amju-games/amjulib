#include "SoundSDLMixer.h"
#include <SDL_mixer.h>
#include <ReportError.h>

namespace
{
typedef std::pair<Mix_Chunk*, int>  SoundPair; 

typedef std::vector<SoundPair> Sounds;

Sounds mySounds;
}

namespace Amju
{
SoundSDLMixer::SoundSDLMixer()
{
    int audio_rate = 22050;
    Uint16 audio_format = AUDIO_S16SYS;
    int audio_channels = 2;
    int audio_buffers = 4096;
 
    if(Mix_OpenAudio(
        audio_rate, audio_format, audio_channels, audio_buffers) != 0) 
    {
        ReportError(Mix_GetError());
    }
}

SoundSDLMixer::~SoundSDLMixer()
{
    Mix_CloseAudio();

}

bool SoundSDLMixer::PlayWav(const std::string& wavFile, float volume)
{
    Mix_Chunk *sound = Mix_LoadWAV((std::string("sound/") + wavFile).c_str());
    if(sound == NULL) 
    {
        ReportError(std::string("Unable to load wav file: ") + std::string(Mix_GetError()));
        return false;
    }

    int channel = Mix_PlayChannel(-1, sound, 0);
    if(channel == -1)
    {
        ReportError(std::string("Unable to play wav file: ") + std::string(Mix_GetError()));
        return false;
    }

    mySounds.push_back(SoundPair(sound, channel));

    return true;
}

bool SoundSDLMixer::PlaySong(const std::string& filename)
{
    static Mix_Music *music = 0;

    if (music)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
    }
    
    music = Mix_LoadMUS((std::string("sound/") + filename).c_str());
    if(music == NULL) 
    {
        ReportError(std::string("Unable to load music file: ") + std::string(Mix_GetError()));
        return false;
    }

    // Max loops
    if(Mix_PlayMusic(music, -1) == -1) 
    {
        ReportError(std::string("Unable to play music file: ") + std::string(Mix_GetError()));
        return false;
    }


    return true;
}

void SoundSDLMixer::StopSong()
{

}

void SoundSDLMixer::Update()
{
    // 1st step: free up sounds which have finished
    for (unsigned int i = 0; i < mySounds.size(); i++)
    {
        if (Mix_Playing(mySounds[i].second) == 0)
        {
            // It's finished
            Mix_FreeChunk(mySounds[i].first);
            mySounds[i].first = 0;
        }
    }

    // 2nd step: delete finished sounds from the vector
}

void SoundSDLMixer::SetWavMaxVolume(float f)
{

}

void SoundSDLMixer::SetSongMaxVolume(float f)
{

}
}
