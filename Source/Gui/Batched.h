// Amjulib - cross platform game engine
// (c) Copyright Juliet Colman 2000-2018

#pragma once

#include <AmjuGL.h>
#include <Matrix.h>
#include "GuiElement.h"

namespace Amju
{
class Texture;

// * Batched * 
// Mixin Base class for visible GUI elements which we batch together to
//  minimise draw calls.
class Batched
{
public:
  virtual ~Batched();

  // Call this to draw all batched elements in as few draw calls as poss.
  static void DrawAll();

  // Override Draw to stash the AmjuGL state, 
  //  so the tris you add in AddToTrilist are correctly transformed, coloured, etc.
  virtual void Draw() = 0;

protected:
  // Add to map of elements to batch draw
  // In use: AddToBatch(this) from Draw() (and don't actually draw)
  void AddToBatch(GuiElement*); 

  // In draw call, don't actually draw, but set up matrix, colour, etc.
  // Then when we call AddToTrilist(), add all the tris making up this
  //  element, with vertex positions transformed, and colours set on 
  //  each vertex.
  virtual void AddToTrilist(AmjuGL::Tris& tris) = 0;

private:
  int GetTexHash(GuiElement*); // used in AddToBatch only

private:
  // Map hash of image filename to all elements using that image.
  // This lets us draw all elements in as few draw calls as possible.
  using FilenameHash = unsigned int;

  // NB NOT RC Ptrs in this vector, as this will give a cyclic dep.
  using TextureToBatchMap = std::map<FilenameHash, std::vector<Batched*>>;
  // Sort batches by ascending z-coord. 
  // Z-coords are converted to ints for easy comparison.
  using AtlasMap = std::map<int, TextureToBatchMap>;
  static AtlasMap s_atlases;

protected:
  // Store current state of transform, colour, etc in Draw() override,
  //  then use this to set the vertices of the tris you add to the
  //  triangle batch in AddToTrilist.
  Matrix m_combinedTransform; // combined modelview transform
  Colour m_combinedColour;

private:
  // Hash of texture atlas filename. 
  // Used in GetTexHash only.
  unsigned int m_texHash = 0;
};

// * GuiFlush *
// Gui type which calls Batched::DrawAll. I.e. it flushes all batched elements.
// This can be used to force a draw order, in the absence of a more drastic
//  rewrite.
class GuiFlush : public GuiElement
{
public:
  static const char* NAME;
  GuiFlush();
  std::string GetTypeName() const override { return NAME; }
  void Draw() override;
  bool Load(File*) override { return true; }
  bool Save(File*) override;
};
}
