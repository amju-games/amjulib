#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include <map>
#include <string>
#include <RCPtr.h>
#include <Singleton.h>
#include <StringUtils.h>

namespace Amju
{
class PlayerInfo : public RefCounted
{
public:
  ~PlayerInfo();

  void SetFilename(const std::string& filename);

  void PISetString(const std::string& key, const std::string& val);
  void PISetBool(const std::string& key, bool val);
  void PISetInt(const std::string& key, int val);
  void PISetFloat(const std::string& key, float val);

  const std::string& PIGetString(const std::string& key) const;
  bool PIGetBool(const std::string& key) const;
  int PIGetInt(const std::string& key) const;
  float PIGetFloat(const std::string& key) const;

  bool Exists(const std::string& key) const;

  // Load file -- succeeds if file does not exist, assume not created yet
  bool Load();
  bool Save();

private:
  std::string m_filename;
  typedef std::map<std::string, std::string> PIMap;
  PIMap m_map;
};

typedef RCPtr<PlayerInfo> PPlayerInfo;


class PlayerInfoManager : public NonCopyable
{
public:
  ~PlayerInfoManager();

  void SetCurrentPlayer(const std::string& playerName);
  PlayerInfo* GetPI(); // gets current player, must be set first
  Strings GetPlayerNames() const; // returns list of player filenames
  int GetNumPlayerNames() const;

  bool Load();
  bool Save();

private:
  PlayerInfoManager();
  friend class Singleton<PlayerInfoManager>;

  typedef std::pair<unsigned int, PPlayerInfo> TimestampPlayerInfo;

  typedef std::map<const std::string, TimestampPlayerInfo> PIMap;
  PIMap m_map;
  PlayerInfo* m_currentPI;
};

// Easy to identify keys -- maybe hash later ? TODO
#define PI_KEY(str) str

typedef Singleton<PlayerInfoManager> ThePlayerInfoManager;
}

#endif

