#ifndef AVATAR_MANAGER_H
#define AVATAR_MANAGER_H

#include <Singleton.h>
#include "Ve1Character.h"
#include <vector>

namespace Amju
{
class AvatarManager : public NonCopyable
{
public:
  bool Load();

  // Set the avatar type for a character
  void SetAvatar(int type, Ve1Character*);

  // Set texture (pair) for a character
  void SetTexture(int texNum, Ve1Character*);

private:
  typedef std::vector<PMd2Model> Characters; ////RCPtr<Ve1Character> > Characters;
  Characters m_chars;

  typedef std::pair<PTexture, PTexture> TexPair;
  typedef std::vector<TexPair> TexPairs;
  TexPairs m_texPairs;
};

typedef Singleton<AvatarManager> TheAvatarManager;
}

#endif


