/*
Amju Games source code (c) Copyright Jason Colman 2000-2008
*/

#ifndef AMJU_ANIM_PLAYER_H_INCLUDED
#define AMJU_ANIM_PLAYER_H_INCLUDED

#include "KeyFrame.h"
#include "IndexedMesh.h"
#include "Anim.h"

namespace Amju
{
class Skeleton;

// Holds the current state of an animated mesh
class AnimPlayer
{
public:
  AnimPlayer();

  void SetMesh(PMesh);
  
  // Update animation; interpolate between keyframes or just use the latest
  //  keyframe.
  void Update(float dt, bool interpolate = true);

  void Draw();
  void DrawSkel();

  void CreateBones(const Skeleton*);

  // Set the Animation to play. Start from the first keyframe of the
  // new anim. Blend into this keyframe from whatever the current
  // keyframe is, over a specified time ?
  void SetAnim(PAnim p, float blendTime); 

  // Get time of current anim
  float GetTime() const;

  void ResetTime();

private:
  // Time in current anim
  float m_time;

  // Array of bones. Each bone can point to children, which also live in
  //  this array.
  Bones m_bones;

  // Mesh + skin (bone weights)
  PMesh m_pMesh;

  // Current animation
  PAnim m_pAnim;

  // Next animation, if we are blending from one to the next
  PAnim m_pNextAnim;

  // Remaining time to blend between current and next anim
  float m_blendTime;
 
  // Set when we reach the end of the anim and loop
  bool m_hasLooped;
};
}

#endif

