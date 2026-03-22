// Amjulib - cross platform game engine
// (c) Copyright Juliet Colman 2000-2026

#include <AmjuHash.h>
#include <Texture.h>
#include <TriList.h>
#include "Batched.h"

namespace Amju
{
Batched::AtlasMap Batched::s_atlases;

Batched::~Batched()
{
}

int Batched::GetTexHash()
{
  if (m_texHash == 0)
  {
    m_texHash = HashString(GetTexture()->GetResName());
  }
  return m_texHash;
}

void Batched::AddToBatch()
{
  auto& vec = s_atlases[GetTexHash()];
  // Only add once! E.g. on successful load. Calling in Draw() is really
  //  inefficient.
  // Assuming we only add each element once before trashing the vec, we can
  //  just check in debug mode, and not check in release mode:
#ifdef _DEBUG
  if (std::find(vec.begin(), vec.end(), this) != vec.end())
  {
    // This means the same batched gui element is being added more than once.
    // E.g. it is added in Load and in Draw? Currently we call AddToBatch in
    //  Draw(), then clear the vector after drawing the batch.
    Assert(0);
  }
#endif // _DEBUG

  // No existing copy in the vec, so add it.
  vec.push_back(this);
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
    for (Batched* batched : vec)
    {
      batched->AddToTrilist(tris[i]);
    }
    i++;
  }

  std::vector<RCPtr<TriListDynamic>> triLists;
  
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

  // Oh no: while we do this, we have to call AddToBatch every frame, for every batched
  //  GUI element. Much better to call once for each element, e.g. on successful load,
  //  but then we will need to be careful to clear this map when we reload the GUI.
  s_atlases.clear();
}

// ----------------------------------------------------------------------------

const char* GuiFlush::NAME = "flush";

GuiFlush::GuiFlush()
{
  // Set a name for editor mode
  SetName(GetTypeName()); // unusual but we won't want to access this node by name, right?
}

void GuiFlush::Draw()
{
  Batched::DrawAll();
}

bool GuiFlush::Save(File* f)
{
  return f->Write(GetTypeName());
}
}
