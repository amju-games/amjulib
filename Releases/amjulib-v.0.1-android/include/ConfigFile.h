#ifndef CONFIG_FILE_H_INCLUDED
#define CONFIG_FILE_H_INCLUDED

// Designed to get game configuration
#include <map>
#include <string>
#include <Singleton.h>

namespace Amju
{
class ConfigFile : public NonCopyable
{
public:
  ConfigFile();

  bool Load(const std::string& filename, bool useRoot = true);
  bool Save(const std::string& filename, bool useRoot = true);

  // Returns default if key does not exist
  std::string GetValue(const std::string& key, const std::string& defaultVal = "") const;
  float GetFloat(const std::string& key, float defaultVal = 0) const;
  int GetInt(const std::string& key, int defaultVal = 0) const;

  bool Exists(const std::string& key) const;

  void Set(const std::string& key, const std::string& value);
  void SetFloat(const std::string& key, float value);
  void SetInt(const std::string& key, int value);

  void Erase(const std::string& key);

  void Clear();

  int GetChecksum() const;

protected:
  typedef std::map<std::string, std::string> ConfigMap;
  ConfigMap m_values;
};

// You can have multiple config files but there is one GameConfigFile 
//  which is used for game-global config, and is the default 
//  config file to use.
class GameConfigFile : public ConfigFile
{
private:
  GameConfigFile() {}
  friend class Singleton<GameConfigFile>;

public:
  ~GameConfigFile() { Save(); }

  // Set full path to config file at start up
  void SetFilePath(const std::string& filepath) { m_filepath = filepath; }

  bool Load()
  {
    return ConfigFile::Load(m_filepath, false);
  }

  bool Save()
  {
    return ConfigFile::Save(m_filepath, false);
  }

private:
  std::string m_filepath;
};

typedef Singleton<GameConfigFile> TheGameConfigFile;
}
#endif

