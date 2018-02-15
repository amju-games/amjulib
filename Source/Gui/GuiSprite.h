// Amjulib - cross platform game engine
// (c) Copyright Jason Colman 2000-2018

#pragma once

#include <GuiImage.h>

namespace Amju
{
// * GuiSprite *
// Type of image, where we we draw one cell of a grid-style texture atlas.
// We can animate the current cell through a range of cell numbers.
// We optimise draw calls by batching all the sprites which use the
//  same atlas. So this type is preferable to GuiImage.

// TODO Not sure if we need to inherit from GuiImage -- but could be a good thing,
//  as perhaps we can replace Images with Sprites without affecting a lot of
//  other code.
class GuiSprite : public GuiImage
{
public:
  static const char* NAME;

  ~GuiSprite();

  void Draw() override;
  bool Load(File*) override;
  void Animate(float animValue) override;

  // Call this to draw all GuiSprites in as few draw calls as poss.
  static void DrawAllSprites();

  // So we can set up sprites programmatically
  void SetCell(int cell);
  void SetCellRange(int minCell, int maxCell);
  void SetNumCells(int cellsX, int cellsY);

private:
  // Add/remove from map
  void AddThis();
  void RemoveThis();

  void BuildTris() override;

protected:
  // Map hash of image filename to all sprites using that image.
  // This lets us draw all GuiSprites in as few draw calls as possible.

  // NB NOT RC Ptrs in this vector, as this will give a cyclic dep.
  using SpriteSheetMap = std::map<unsigned int, std::vector<GuiSprite*>>;
  static SpriteSheetMap s_sprites;

  // Number of cells in x and y (We set this per Gui Sprite, not for the image.
  //  This lets us have different grid sizes on the same image.)
  Vec2i m_numCellsXY;
  // Current cell (left to right, then top to bottom)
  int m_cell = 0;
  // For animating the current cell, the start and end of the range of
  //  cells to animate through.
  int m_minCell = 0;
  int m_maxCell = 0;

  // Hash of texture filename. 
  unsigned int m_texHash = 0;
};
}
