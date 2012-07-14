#include "AvatarManager.h"
#include <File.h>
#include <StringUtils.h>
#include <ReportError.h>
#include <ResourceManager.h>

namespace Amju
{
bool AvatarManager::Load()
{
  // Load character types - NB we want this to be downloadable.
  // Add a new object with asset list to server, which this client will then download...?

  m_chars.clear();
  m_texPairs.clear();

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

  m_chars.reserve(numChars);

  for (int i = 0; i < numChars; i++)
  {
    std::string s;
    if (!f.GetDataLine(&s))
    {
      f.ReportError("Expected MD2 filename");
      return false;
    }

    // Surely MD2 Model should be managed by Resource manager???!!!
    Md2Model* ch = (Md2Model*)TheResourceManager::Instance()->GetRes(s);
    // Instead of:
    /*
    Md2Model* ch = new Md2Model;
    if (!ch->Load(s))
    {
      f.ReportError("Failed to load character " + ToString(i));
      return false;
    }
    */

    m_chars.push_back(ch);
  }

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

  return true;
}

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

}



