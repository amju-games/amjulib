// Amjulib - cross platform game engine
// (c) Copyright Jason Colman 2000-2018

#include <Texture.h>
#include <TriList.h>
#include "Batched.h"

namespace Amju
{
Batched::AtlasMap Batched::s_atlases;

Batched::~Batched()
{
  RemoveThis();
}

void Batched::AddThis()
{
  auto& vec = s_atlases[m_texHash];
  // Only add once! Could happen when reloading.
  if (std::find(vec.begin(), vec.end(), this) == vec.end())
  {
    s_atlases[m_texHash].push_back(this);
  }
}

void Batched::RemoveThis()
{
  auto& vec = s_atlases[m_texHash];
  vec.erase(std::remove(vec.begin(), vec.end(), this), vec.end());
}

void Batched::DrawAll()
{
  // Vector of Tri Lists: one tri list is used to draw all the sprites which use
  //  the same texture, so we minimise draw calls.
  // NB To my surprise, making these vecs static annhililated the frame rate!
  std::vector<AmjuGL::Tris> tris;
  // Vector of textures: each texture corresponds to a separate tri list.
  std::vector<Texture*> textures;
  unsigned int numLists = s_atlases.size();
  tris.resize(numLists);
  textures.resize(numLists);

  // First: get all the tris from each sprite into the right tri list.
  int i = 0;
  // Iterate over ALL sprites
  for (auto& p : s_atlases)
  {
    // vec contains only sprites using the same texture.
    auto& vec = p.second;
    if (vec.empty())
    {
      i++;
      continue; // Hmm, should prune this node?
    }
    textures[i] = vec[0]->GetTexture(); // get the texture used by all elements in this vec

    // Add the tris for each element to the current tri list batch
    for (Batched* sprite : vec)
    {
      sprite->AddToTrilist(tris[i]);
    }
    i++;
  }

  // TriLists: don't create each time.
  // Making this guy static does not kill frame rate, and is slightly better
  //  than non-static
  static std::vector<RCPtr<TriListDynamic>> triLists;
  while (triLists.size() < numLists)
  {
    triLists.push_back((TriListDynamic*)
      AmjuGL::Create(TriListDynamic::DRAWABLE_TYPE_ID));
  }

  // Second: draw each tri list, setting the texture once for the tri list.
  // TODO We should also sort by shader. For now, we assume it's the same 
  //  shader for all sprites.
  for (unsigned int i = 0; i < numLists; i++)
  {
    if (!textures[i])
    {
      // In practice this happens when there are no sprites to draw, so there
      //  are empty vecs in s_sprites.
      continue;
    }
    textures[i]->UseThisTexture();
    triLists[i]->Set(tris[i]);
    triLists[i]->Draw();
  }
}

}
