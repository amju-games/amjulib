#ifndef ANIMATED_H_INCLUDED
#define ANIMATED_H_INCLUDED

#include "Ve1Character.h"
#include "Md2Model.h"

namespace Amju
{
// TODO Rename to Md2Node etc

class Animated : public Ve1Character
{
public:
  Animated();

  virtual void Draw();
  virtual void Update();

  virtual void SetFromCharacterName(const std::string& characterName);

  virtual void SetAnim(Anim animName);

  // TODO Why do we need these ?
  bool LoadMd2(const std::string& md2name);
  void SetMd2(Md2Model* model);
  Md2Model* GetMd2();

protected:
  void SetAnim(int animId);

  // No texture data here - we don't know how many textures 

  PMd2Model m_pModel;

private:
  int m_anim;
  // TODO Need next anim ?
  int m_frame;
  int m_nextFrame;
  float m_t; // varies from 0..1
};
}

#endif
