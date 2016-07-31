#ifndef CREATE_TEXT_H
#define CREATE_TEXT_H

#include <string>
#include "SceneNode.h"

namespace Amju
{
SceneNode* MakeChar(char k, float* width);

class TextMaker
{
public:
  virtual ~TextMaker() {}

  SceneNode* MakeText(const std::string& text);

  void SetDataDir(const std::string&);
 
protected:
  SceneNode* MakeChar(char k, float* width);

  // Override to do something fancy to letters. 
  // Each character has Decorate called on it.
  virtual PSceneNode Decorate(PSceneNode p) { return p; }

protected:
  std::string m_dataDir; // where the .obj files live
};
}

#endif


