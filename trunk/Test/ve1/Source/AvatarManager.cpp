#include <File.h>
#include <StringUtils.h>
#include <ReportError.h>
#include <ResourceManager.h>
#include "AvatarManager.h"
#include "BlinkCharacter.h" // TODO TEMP TEST - use factory
#include "Md3Node.h" // factory

namespace Amju
{
const std::string& AvatarManager::GetNextName(const std::string& characterName)
{
  Assert(!m_chars.empty());

  CharMap::iterator it = m_chars.find(characterName);
  if (it == m_chars.end())
  {
std::cout << "Get next name: No character called \"" << characterName << "\"\n";
    return m_chars.begin()->first;
  }
  
  ++it;
  if (it == m_chars.end())
  {
    it = m_chars.begin();
  }
  return it->first;
}

const std::string& AvatarManager::GetPrevName(const std::string& characterName)
{
  Assert(!m_chars.empty());

  CharMap::reverse_iterator it(m_chars.find(characterName)); // NB explicit ctor call required,
    // http://stackoverflow.com/questions/2310656/reverse-iteration-from-a-given-map-iterator

  if (it == m_chars.rend())
  {
std::cout << "Get next name: No character called \"" << characterName << "\"\n";
    return m_chars.begin()->first;
  }
  
  ++it;
  if (it == m_chars.rend())
  {
    it = m_chars.rbegin();
  }
  return it->first;
}

Ve1Character* AvatarManager::Create(const std::string& characterName)
{
  // TODO TEMP TEST
  // use character name to look up create func and data directory, options, etc.

  Ve1Character* ch = 0;
  CharMap::iterator it = m_chars.find(characterName);
  if (it == m_chars.end())
  {
    Assert(0);
std::cout << "Unexpected character name: " << characterName << "\n";
    // Create a placeholder node of default type. This probably means we have not downloaded
    //  data for a new type.
    ch = new Md3Node;
  }
  else
  {
    std::string chType = it->second;
    // TODO struct with other attributes, e.g. scale factor

    // TODO use factory using type
    if (chType == "md3")
    {
      ch = new Md3Node;
    }
    else if (chType == "md2blink")
    {
      ch = new BlinkCharacter; 
    }
    // etc -- TODO use Factory
    else
    {
      ch = new Animated; // TODO rename this class
    }
  }

  Assert(ch);
  ch->SetFromCharacterName(characterName);

  return ch;
}


bool AvatarManager::Load()
{
  // Load character types - NB we want this to be downloadable.
  // Add a new object with asset list to server, which this client will then download...?

////  m_chars.clear();
////  m_texPairs.clear();

  File f;
  if (!f.OpenRead("charlist.txt"))
  {
    ReportError("Failed to load character list");
    return false;
  }

  int numChars = 0;
  if (!f.GetInteger(&numChars))
  {
    ReportError("Expected number of characters!");
    return false;
  }

////  m_chars.reserve(numChars);

  for (int i = 0; i < numChars; i++)
  {
    std::string chName;
    if (!f.GetDataLine(&chName))
    {
      f.ReportError("Expected character name");
      return false;
    }

    // Read type
    std::string chType;
    if (!f.GetDataLine(&chType))
    {
      f.ReportError("Expected character type");
      return false;
    }

    // TODO
    //ChInfo chInfo;
    //chInfo.type = chType;

    m_chars[chName] = chType;

/*
    // Surely MD2 Model should be managed by Resource manager???!!!
    Md2Model* ch = (Md2Model*)TheResourceManager::Instance()->GetRes(s);
    // Instead of:
    Md2Model* ch = new Md2Model;
    if (!ch->Load(s))
    {
      f.ReportError("Failed to load character " + ToString(i));
      return false;
    }
    m_chars.push_back(ch);
*/

  }

/*
  int numTex = 0;
  if (!f.GetInteger(&numTex))
  {
    f.ReportError("Expected num textures");
    return false;
  }

  m_texPairs.reserve(numTex);

  for (int i = 0; i < numTex; i++)
  {
    std::string tex1Name, tex2Name;
    if (!f.GetDataLine(&tex1Name) || !f.GetDataLine(&tex2Name))
    {
      f.ReportError("Failed to get 2 textures for player");
      return false;
    }
    Texture* t1 = (Texture*)TheResourceManager::Instance()->GetRes(tex1Name);
    Texture* t2 = (Texture*)TheResourceManager::Instance()->GetRes(tex2Name);
    if (!t1 || !t2)
    {
      f.ReportError("Failed to load texture");
      return false;
    } 
    m_texPairs.push_back(std::make_pair(t1, t2));
  }
*/

  return true;
}

/*
void AvatarManager::SetAvatar(int type, Ve1Character* ch)
{
  Assert(!m_chars.empty()); 

  type %= (int)m_chars.size();
  Assert(type < (int)m_chars.size());

  ch->SetMd2(m_chars[type]);
}

void AvatarManager::SetTexture(int texNum, Ve1Character* ch)
{
  Assert(!m_texPairs.empty());

  texNum %= (int)m_texPairs.size();
  Assert(texNum < (int)m_texPairs.size());

  ch->SetTex(m_texPairs[texNum].first, m_texPairs[texNum].second);
}
*/

}



