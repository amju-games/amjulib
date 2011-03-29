/*
Amju Games source code (c) Copyright Jason Colman 2000-2008
*/

#ifndef KEY_FRAME_H_INCLUDED
#define KEY_FRAME_H_INCLUDED

#include <map>
#include "Quaternion.h"
#include "Bone.h"

namespace Amju
{
class File;
  
// "RST" - rotation, scaling, translation
class RST
{
public:
  Quaternion m_quat;
  //  scale and translate
  Vec3f m_scale;
  Vec3f m_translate;
};

class KeyFrame;
typedef SharedPtr<KeyFrame> PKeyFrame;

// Stores orientation for a bone. 
// Animation stores list of KeyFrames for each bone.
class KeyFrame : public Shareable
{
public:
  KeyFrame();

  bool Load(File*);
#ifdef SCENE_EDITOR
  bool Save(File*);
#endif

  float GetTime() const;

  // If interpolation is not needed, call this to just get the 
  //  transformation for this keyframe
  const Matrix& GetMatrix() const;

  static Matrix Interpolate(
    PKeyFrame k1, PKeyFrame k2, float between);

  // Interpolate between two key frames, using the 'between' value (0..1)
  // to set the positions of the bones.
  static Matrix Interpolate(const RST& r1, const RST& r2, float t);

private:
  float m_time;
  Matrix m_matrix;
};

// Map time of key frame to the keyframe itself
typedef std::map<float, PKeyFrame> KeyFrames;

}

#endif

