/*
Amju Games source code (c) Copyright Jason Colman 2000-2008
*/

#ifndef ANIM_MANAGER_H_INCLUDED
#define ANIM_MANAGER_H_INCLUDED

#include <map>
#include <string>
#include <utility>
#include "Singleton.h"
#include "IndexedMesh.h"
#include "Skin.h"
#include "Skeleton.h"
#include "Anim.h"
#include "AnimPlayer.h"

namespace Amju
{
// Make it easier to use the Anim classes; share data between characters
class AnimManager
{
public:
  bool SetUpAnimPlayer(
    AnimPlayer* pPlayer,
    const std::string& meshName,
    const std::string& skinName,
    const std::string& skelName);

  // Load anim; call to load up all anims up front. You need to specify the
  //  skeleton to which the anim relates so we can look up bone names.
  bool LoadAnim(const std::string& animName, const std::string& skelName);

  // Returns a named anim, previously loaded.
  PAnim GetAnim(const std::string& animName);

  // Clear all data, call at e.g. end of a level
  void Clear();

private:
  typedef std::pair<PMesh, PSkin> MeshPair;
  typedef std::map<std::string, MeshPair> MeshMap;
  MeshMap m_meshes;

  typedef std::map<std::string, PSkeleton> SkelMap;
  SkelMap m_skels;
  
  typedef std::map<std::string, PAnim> AnimMap;
  AnimMap m_anims;
};

typedef Singleton<AnimManager> TheAnimManager;
}

#endif
