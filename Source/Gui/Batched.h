// Amjulib - cross platform game engine
// (c) Copyright Jason Colman 2000-2018

#pragma once

#include <AmjuGL.h>
#include <Matrix.h>

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
  // Add/remove from map
  void AddThis(); // call on successful load
  void RemoveThis(); // called in dtor, or you can call earlier

  // In draw call, don't actually draw, but set up matrix, colour, etc.
  // Then when we call AddToTrilist(), add all the tris making up this
  //  element, with vertex positions transformed, and colours set on 
  //  each vertex.
  virtual void AddToTrilist(AmjuGL::Tris& tris) = 0;

  virtual Texture* GetTexture() = 0;

protected:
  // Map hash of image filename to all elements using that image.
  // This lets us draw all elements in as few draw calls as possible.

  // NB NOT RC Ptrs in this vector, as this will give a cyclic dep.
  using AtlasMap = std::map<unsigned int, std::vector<Batched*>>;
  static AtlasMap s_atlases;

  // TODO Also we want to be able to set the draw order for the 
  //  batched elements, i.e. each atlas has a z-depth.

  // Store current state of transform, colour, etc in Draw() override,
  //  then use this to set the vertices of the tris you add to the
  //  triangle batch in AddToTrilist.
  Matrix m_combinedTransform; // combined modelview transform
  Colour m_combinedColour;

  // Hash of texture atlas filename. 
  unsigned int m_texHash = 0;

};
}
