#include "PlayerInfo.h"
#include <File.h>
#include <StringUtils.h>
#include <Directory.h>
#include <iostream>

namespace Amju
{
PlayerInfo::~PlayerInfo()
{
  Save();
}

void PlayerInfo::SetFilename(const std::string& filename)
{
  m_filename = filename;
}

void PlayerInfo::PISetString(const std::string& key, const std::string& val)
{
  m_map[key] = val;
}

void PlayerInfo::PISetBool(const std::string& key, bool val)
{
  m_map[key] = (val ? "1" : "0");
}

void PlayerInfo::PISetInt(const std::string& key, int val)
{
  m_map[key] = ToString(val);
}

void PlayerInfo::PISetFloat(const std::string& key, float val)
{
  m_map[key] = ToString(val);
}

const std::string& PlayerInfo::PIGetString(const std::string& key) const
{
  PIMap::const_iterator it = m_map.find(key);
#ifdef _DEBUG
  if (it == m_map.end())
  {
    std::cout << "Player info: no key '" << key << "'\n";
  }
#endif
  Assert(it != m_map.end());
  return it->second;
}

bool PlayerInfo::PIGetBool(const std::string& key) const
{
  PIMap::const_iterator it = m_map.find(key);
  if (it == m_map.end())
  {
    return false;
  }
  return (it->second == "1" ? true : false); 
}

int PlayerInfo::PIGetInt(const std::string& key) const
{
  PIMap::const_iterator it = m_map.find(key);
#ifdef _DEBUG
  if (it == m_map.end())
  {
    std::cout << "Player info: no key '" << key << "'\n";
  }
#endif
  Assert(it != m_map.end());
  return ToInt(it->second);
}

float PlayerInfo::PIGetFloat(const std::string& key) const
{
  PIMap::const_iterator it = m_map.find(key);
#ifdef _DEBUG
  if (it == m_map.end())
  {
    std::cout << "Player info: no key '" << key << "'\n";
  }
#endif
  Assert(it != m_map.end());
  return ToFloat(it->second);
}

bool PlayerInfo::Load()
{
  // FileExists doesn't append File::Root
  if (FileExists(File::GetRoot() + m_filename))
  {
#ifdef PI_DEBUG
std::cout << "Loading player info " << m_filename << "...\n";
#endif
  }
  else
  {
#ifdef PI_DEBUG
std::cout << "Loading player info " << m_filename << " doesn't exist! - - it's a new file\n";
#endif
    // We assume the player is new and has not saved any player info yet - this is OK.
    return true;
  }

#ifdef PI_DEBUG
std::cout << "Loading player info " << m_filename << "...\n";
#endif

  File f;
  if (!f.OpenRead(m_filename))
  {
    return false;
  }
  int num = 0;
  if (!f.GetInteger(&num))
  {
    return false;
  }
  for (int i = 0; i < num; i++)
  {
    std::string s;
    if (!f.GetDataLine(&s))
    {
      return false;
    }

#ifdef PI_DEBUG
std::cout << " Got line: " << s << "\n";
#endif

    Strings strs = Split(s, ',');
    if (strs.size() != 2)
    {
      f.ReportError("Bad line in player info: " + s);
      return false;
    }
    PISetString(strs[0], strs[1]);
  }
#ifdef PI_DEBUG
std::cout << "End of player info load.\n";
#endif

  return true;
}

bool PlayerInfo::Save()
{
#ifdef PI_DEBUG
std::cout << "SAVING PLAYER INFO FILE " << m_filename << "\n";
#endif

  File f; // TODO no glue file
  if (!f.OpenWrite(m_filename))
  {
    return false;
  }
  f.WriteInteger(m_map.size());
  for (PIMap::iterator it = m_map.begin(); it != m_map.end(); ++it)
  {
    // TODO what about commas in keys or values ?!
    f.Write(it->first + std::string(",") + it->second);
  }

  return true;
}


PlayerInfoManager::PlayerInfoManager()
{
  Load();
  m_currentPI = 0;
}

PlayerInfoManager::~PlayerInfoManager()
{
  Save();
}

PlayerInfo* PlayerInfoManager::GetPI()
{
  Assert(m_currentPI);
  return m_currentPI;
}

static const char* PIM_FILENAME = "playerinfo.txt"; 

bool PlayerInfoManager::Load()
{
#ifdef PI_DEBUG
std::cout << "PlayerInfoManager: loading players...\n";
#endif

  if (!FileExists(File::GetRoot() + PIM_FILENAME))
  {
#ifdef PI_DEBUG
std::cout << "No playerinfo file exists to load.\n";
#endif
    return true;
  } 
  File f;
  if (!f.OpenRead(PIM_FILENAME))
  {
#ifdef PI_DEBUG
std::cout << "Player info file does exist but we couldn't open it for reading!\n";
#endif
    return false;
  }
  int numPlayers = 0;
  if (!f.GetInteger(&numPlayers))
  {
    f.ReportError("Expected num players"); 
    return false; 
  }

#ifdef PI_DEBUG
std::cout << "PlayerInfoManager: opened file, got " << numPlayers << " as number of players.\n";
#endif
  for (int i = 0; i < numPlayers; i++)
  {
    std::string s;
    if (!f.GetDataLine(&s))
    {
      f.ReportError("Expected player name " + ToString(i) + " of " + ToString(numPlayers));
      return false;
    }

#ifdef PI_DEBUG
std::cout << "PlayerInfoManager: player " << i << " name: \"" << s << "\"\n";
#endif
    m_map[s] = 0; 
  }
  return true;
}

bool PlayerInfoManager::Save()
{
  File f;
  if (!f.OpenWrite(PIM_FILENAME))
  {
    return false;
  }
  int size = m_map.size();
  f.WriteInteger(size);
  for (PIMap::iterator it = m_map.begin(); it != m_map.end(); ++it)
  {
    f.Write(it->first); 
  }
  return true;
}

void PlayerInfoManager::SetCurrentPlayer(const std::string& filename)
{
  PIMap::iterator it = m_map.find(filename);
  if (it == m_map.end() || !it->second)
  {
    PlayerInfo* pi = new PlayerInfo;
    pi->SetFilename(filename);
    if (!pi->Load())
    {
      // TODO What to do here ?? Player has lost player data!! Oh no!
    }
    m_map[filename] = pi;
    m_currentPI = pi;
  }
  else
  {
    m_currentPI = it->second;
  }
}

Strings PlayerInfoManager::GetPlayerNames() const
{
  Strings s;
  for (PIMap::const_iterator it = m_map.begin(); it != m_map.end(); ++it)
  {
    s.push_back(it->first);
  }
  return s;
}
}

