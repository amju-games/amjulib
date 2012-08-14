#ifndef COLLECT_H_INCLUDED
#define COLLECT_H_INCLUDED

#include "Ve1Object.h"

namespace Amju 
{
class Player;

// Created when Server sends msg. Local player only can collect them.
// TODO Not createable in the same way as other Game Objects.
class Collect : public Ve1Object
{
public:
  static const char* TYPENAME;

  Collect();
  virtual const char* GetTypeName() const;
  virtual void Update(); 
  virtual bool Load(File*); 
  virtual void SetEditMenu(GuiMenu*);
  virtual void OnLocationEntry();

  // Create - all assets must already be local
  bool Create(const std::string& objFilename, const std::string& textureFilename);

  void OnPlayerCollision(Player*);

  void SetSpecialId(int s) { m_specialId = s; }

protected:
  virtual void OnBounceStop();

private:
  // Server sends special ID, which we use on collision to prove to server that it's a real Collect obj
  // TODO that's the idea, not implemented
  int m_specialId;
  float m_rot; 
  float m_rotvel;
  float m_timer;
};
} // namespace
#endif
