#include "AmjuFirst.h"
#include <StringUtils.h>
#include "ConfigFile.h"
#include "File.h"
#include "Tokeniser.h"
#include <iostream>
#include "AmjuFinal.h"

using namespace std;

namespace Amju
{
static const bool NOT_BINARY = false;

ConfigFile::ConfigFile()
{
  AMJU_CALL_STACK;

}

void ConfigFile::Erase(const std::string& key)
{
  AMJU_CALL_STACK;

  m_values.erase(key);
}

void ConfigFile::Clear()
{
  AMJU_CALL_STACK;

  m_values.clear();
}

bool ConfigFile::Save(const std::string& filename, bool useRoot)
{
  AMJU_CALL_STACK;

  File f(true, File::STD); // No glue, TODO MEM ?
  if (!f.OpenWrite(filename, File::CURRENT_VERSION, NOT_BINARY, useRoot))
  {
    f.ReportError("Couldn't open config file for writing.");
    return false;
  }
  // Save each key/value pair.
  for (ConfigMap::iterator it = m_values.begin(); it != m_values.end(); ++it)
  {
    string s = it->first;
    s += " ";
    s += it->second;
    f.Write(s);
#ifdef CONFIG_FILE_DEBUG
    std::cout << "Config file: saving: " << s.c_str() << std::endl;
#endif
  }
  return true;
}

bool ConfigFile::Load(const std::string& filename, bool useRoot)
{
  AMJU_CALL_STACK;

  // Ctor args: arg 1 (true) => has version info
  // arg2 (false) => don't use Glue File implementation.
  File f(true, File::STD);

  if (!f.OpenRead(filename, NOT_BINARY, useRoot))
  {
#ifdef _DEBUG
    // This is ok for a clean install, so don't complain in a release build.
    f.ReportError("Couldn't open config file.");
#endif
    return false;
  }
  // This config file simply consists of pairs of tokens. The first of each 
  // pair is the key; the second is the value.
  Tokeniser toker;

  string configLine;
  while (f.GetDataLine(&configLine))
  {
    string key;
    string value;

    if (!toker.Tokenise(&configLine, &key))
    {
      string error = "No value for " + key + " in config file."; 
      f.ReportError(error);
      return false;
    }
    // Tokeniser chops head (the key) off configLine, leaving the value tail.
    value = configLine;

    // Set value in map.
    Set(key, value);

  }
  // No more tokens. This is ok, we have finished.
  return true;
}

std::string ConfigFile::GetValue(const std::string& key, const std::string& defaultVal) const
{
  ConfigMap::const_iterator it = m_values.find(key);
  if (it == m_values.end())
  {
    return defaultVal;
  }
  return it->second;
}

float ConfigFile::GetFloat(const std::string& key, float defaultVal) const
{
  AMJU_CALL_STACK;

  ConfigMap::const_iterator it = m_values.find(key);
  if (it == m_values.end())
  {
    return defaultVal;
  }
  return ToFloat(it->second.c_str());
}

int ConfigFile::GetInt(const std::string& key, int defaultVal) const
{
  AMJU_CALL_STACK;

  ConfigMap::const_iterator it = m_values.find(key);
  if (it == m_values.end())
  {
    return defaultVal;
  }
  return ToInt(it->second.c_str());
}

bool ConfigFile::Exists(const std::string& key) const
{
  AMJU_CALL_STACK;

  ConfigMap::const_iterator it = m_values.find(key);
  if (it == m_values.end())
  {
    return false;
  }
  return true;
}

void ConfigFile::Set(const std::string& key, const std::string& value)
{
  AMJU_CALL_STACK;

  if (!key.empty())
  {
    m_values[key] = value;
  }
}

int ConfigFile::GetChecksum() const
{
  AMJU_CALL_STACK;

  // Add up all the characters in the values.
  int t = 0;
  for (ConfigMap::const_iterator it = m_values.begin(); it != m_values.end(); ++it)
  {
    const std::string& s = it->second;
    int size = s.size();
    for (int i = 0; i < size; i++)
    {
      t += (i + 1) * (int)s[i];
    }
  }
  return t;
}
}
