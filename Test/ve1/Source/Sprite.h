#ifndef AMJU_SPRITE_H_INCLUDED
#define AMJU_SPRITE_H_INCLUDED

#include <TextureSequence.h>
#include <Vec2.h>

namespace Amju
{
class Sprite
{
public:
  Sprite();
  void SetCellRange(int minCell, int maxCell);
  void SetCellTime(float cellTime);
  void SetCell(int cell);

  void Draw(const Vec2f& pos, float size);
  void Update();
  bool Load(const std::string& texFilename, int numCellsX, int numCellsY);

private:
  TextureSequence m_seq;
  int m_minCellNum;
  int m_maxCellNum;
  int m_cellNum;
  float m_maxCellTime;
  float m_cellTime;
};
}

#endif

