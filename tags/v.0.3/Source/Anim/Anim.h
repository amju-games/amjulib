/*
Amju Games source code (c) Copyright Jason Colman 2000-2008
*/

#ifndef AMJU_ANIM_H_INCLUDED
#define AMJU_ANIM_H_INCLUDED

#include <vector>
#include "SharedPtr.h"
#include "KeyFrame.h"
#include "Skeleton.h"

namespace Amju
{
class File;

// Map bone IDs to KeyFrames
typedef std::map<int, KeyFrames> BoneKeyFrames;


// An animation is a list of KeyFrameTimes for each bone
class Anim : public Shareable
{
public:
  Anim();

  bool Load(File*);
#ifdef SCENE_EDITOR
  bool Save(File*);
#endif

  // Need to set skeleton so we can serialise bone names
  void SetSkeleton(PSkeleton pSkel);

  bool Repeats() const;

  const BoneKeyFrames& GetBoneKeyFrames() const;

  float GetDuration() const;

private:
  BoneKeyFrames m_boneKeyFrames;

  // Flags: does this anim repeat ? Or Should it freeze at the final frame 
  bool m_repeats;

  PSkeleton m_pSkel;

  // Duration of animation: i.e. m_time in AnimPlayer loops when it reaches this value
  float m_animDuration;
};

typedef SharedPtr<Anim> PAnim;

}

#endif

