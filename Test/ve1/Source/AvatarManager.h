#ifndef AVATAR_MANAGER_H
#define AVATAR_MANAGER_H

#include <utility>
#include <vector>
#include <Singleton.h>
#include "Ve1Character.h"
#include <Md2Model.h>

namespace Amju
{
class AvatarManager : public NonCopyable
{
public:
  bool Load();

  Ve1Character* Create(const std::string& characterName);

  int GetNumNames() const;
  const std::string& GetName(int) const;
  bool GetIndex(const std::string& name, int* index) const;

//  const std::string& GetNextName(const std::string& characterName);
//  const std::string& GetPrevName(const std::string& characterName);

/*
  // Set the avatar type for a character
  void SetAvatar(int type, Ve1Character*);

  // Set texture (pair) for a character
  void SetTexture(int texNum, Ve1Character*);
*/

private:
/*
  typedef std::vector<PMd2Model> Characters; ////RCPtr<Ve1Character> > Characters;
  Characters m_chars;

  typedef std::pair<PTexture, PTexture> TexPair;
  typedef std::vector<TexPair> TexPairs;
  TexPairs m_texPairs;
*/

  // Maintain the order in charlist.txt.
  // All chars are MD3.
  typedef std::vector<std::string > CharMap;
  //typedef std::map<std::string, std::string> CharMap;
  CharMap m_chars;
};

typedef Singleton<AvatarManager> TheAvatarManager;
}

#endif


