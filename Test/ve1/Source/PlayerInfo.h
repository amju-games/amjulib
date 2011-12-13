#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include <map>
#include <string>
#include <RCPtr.h>

namespace Amju
{
class PlayerInfo : public RefCounted
{
public:
  void SetFilename(const std::string& filename);

  void Set(const std::string& key, const std::string& val);
  const std::string& Get(const std::string& key);

  bool Load();
  bool Save();

private:
  std::string m_filename;
  typedef std::map<std::string, std::string> PIMap;
  PIMap m_map;
};
}

#endif

