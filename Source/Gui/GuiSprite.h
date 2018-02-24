// Amjulib - cross platform game engine
// (c) Copyright Jason Colman 2000-2018

#pragma once

#include <Batched.h>
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
class GuiSprite : public GuiImage, public Batched
{
public:
  static const char* NAME;

  void Draw() override;
  bool Load(File*) override;
  void Animate(float animValue) override;

  // So we can set up sprites programmatically
  void SetCell(int cell);
  void SetCellRange(int minCell, int maxCell);
  void SetNumCells(int cellsX, int cellsY);

private:
  // Add the 2 tris comprising this sprite quad to the given tris.
  // This is used to bunch up all quads using the same texture into one draw call.
  void AddToTrilist(AmjuGL::Tris& tris) override;

  Texture* GetTexture() override;

protected:
  // Number of cells in x and y (We set this per GuiSprite, not for the image.
  //  This lets us have different notional grid sizes on the same image.)
  Vec2i m_numCellsXY;
  // Current cell (left to right, then top to bottom)
  int m_cell = 0;
  // For animating the current cell, the start and end of the range of
  //  cells to animate through.
  int m_minCell = 0;
  int m_maxCell = 0;
};
}
