#include "LayerGroup.h"
#include "ObjectUpdater.h"
#include "LocalPlayer.h"

namespace Amju
{
bool Layer::Load(File* f)
{
  if (!f->GetDataLine(&m_name))
  {
    f->ReportError("Failed to load layer group name");
    return false;
  }
  int numTextures = 0;
  if (!f->GetInteger(&numTextures))
  {
    f->ReportError("Failed to load num textures for layer group " + m_name);
    return false;
  }
  for (int i = 0; i < numTextures; i++)
  {
    std::string t;
    if (!f->GetDataLine(&t))
    {
      f->ReportError("Failed to load texture name for layer group " + m_name);
      return false;
    }
    Texture* tex = (Texture*)TheResourceManager::Instance()->GetRes(t);
    if (!tex)
    {
      f->ReportError("Failed to load texture " + t + " for layer group " + m_name);
      return false;
    }
    m_textures.push_back(tex);
  }
std::cout << "Layer group: loaded " << numTextures << " textures ok.\n";

  int numColours = 0;
  if (!f->GetInteger(&numColours))
  {
    f->ReportError("Failed to load num colours for layer group " + m_name);
    return false;
  }
  for (int i = 0; i < numColours; i++)
  {
    std::string s;
    if (!f->GetDataLine(&s))
    {
      f->ReportError("Failed to load colour for layer group " + m_name);
      return false;
    }
    Colour col = FromHexString(s); // TODO error checking ?
    m_colours.push_back(col);
  }
std::cout << "Layer group: loaded " << numColours << " colours ok.\n";

  return true;
}

LayerGroupManager::LayerGroupManager()
{
  // Read from file
  File f;
  if (!f.OpenRead("layergroups.txt"))
  {
    f.ReportError("Failed to load layer groups file for 2D avatars");
    Assert(0); 
  }
  Load(&f);
}

bool LayerGroupManager::Load(File* f)
{
  int numGroups = 0;
  if (!f->GetInteger(&numGroups))
  {
    f->ReportError("Expected num groups");
    return false;
  }

  m_layers.reserve(numGroups);

  for (int i = 0; i < numGroups; i++)
  {
    Layer g;
    if (!g.Load(f))
    {
      return false;
    }
    m_layers.push_back(g);
  }
std::cout << "Success! Loaded " << numGroups << " layer groups.\n";
  return true;
}

int LayerGroupManager::GetNumTextures(int layer) const
{
  return m_layers[layer].m_textures.size();
}

int LayerGroupManager::GetNumColours(int layer) const
{
  return m_layers[layer].m_colours.size();
}

Texture* LayerGroupManager::GetTexture(int layer, int num) const
{
  return m_layers[layer].m_textures[num];
}

const Colour& LayerGroupManager::GetColour(int layer, int num) const
{
  return m_layers[layer].m_colours[num];
}

int LayerGroups::GetCurrentLayer() const
{
  return m_currentLayer;
}


void LayerGroups::NextLayer()
{
  m_currentLayer++;
  if (m_currentLayer >= (int)m_layers.size())
  {
    m_currentLayer = 0;
  }
}

void LayerGroups::PrevLayer()
{
  m_currentLayer--;
  if (m_currentLayer < 0)
  {
    m_currentLayer = m_layers.size() - 1;
  }
}

void LayerGroups::NextTexture()
{
  static LayerGroupManager* lgm = TheLayerGroupManager::Instance();

  LayerSet& g = m_layers[m_currentLayer];
  g.m_currentTexture++;
  if (g.m_currentTexture >= lgm->GetNumTextures(m_currentLayer))
  {
    g.m_currentTexture = 0;
  }
}

void LayerGroups::PrevTexture()
{
  static LayerGroupManager* lgm = TheLayerGroupManager::Instance();

  LayerSet& g = m_layers[m_currentLayer];
  g.m_currentTexture--;
  if (g.m_currentTexture < 0)
  {
    g.m_currentTexture = lgm->GetNumTextures(m_currentLayer) - 1;
  }
}


void LayerGroups::NextColour()
{
  static LayerGroupManager* lgm = TheLayerGroupManager::Instance();

  LayerSet& g = m_layers[m_currentLayer];
  g.m_currentColour++;
  if (g.m_currentColour >= lgm->GetNumColours(m_currentLayer))
  {
    g.m_currentColour = 0;
  }
}

void LayerGroups::PrevColour()
{
  static LayerGroupManager* lgm = TheLayerGroupManager::Instance();

  LayerSet& g = m_layers[m_currentLayer];
  g.m_currentColour--;
  if (g.m_currentColour < 0)
  {
    g.m_currentColour = lgm->GetNumColours(m_currentLayer) - 1;
  }
}

int LayerGroups::GetNumTextures() const
{
  static LayerGroupManager* lgm = TheLayerGroupManager::Instance();
  return lgm->GetNumTextures(m_currentLayer);
}

int LayerGroups::GetNumColours() const
{
  static LayerGroupManager* lgm = TheLayerGroupManager::Instance();
  return lgm->GetNumColours(m_currentLayer);
}

void LayerGroups::SetSprite(LayerSprite* sprite)
{
  static LayerGroupManager* lgm = TheLayerGroupManager::Instance();

  sprite->Clear();
  int size = m_layers.size();
  for (int i = 0; i < size; i++)
  {
    LayerSet& g = m_layers[i];

    Texture* tex = lgm->GetTexture(i, g.m_currentTexture);
    const Colour& col = lgm->GetColour(i, g.m_currentColour);

    SpriteLayer sl(i, tex, col, g.m_currentTexture, g.m_currentColour);
    sprite->AddLayer(sl);
  }
}

void LayerGroups::SetFromSprite(const LayerSprite& sprite)
{
  m_layers.clear();
  for (LayerSprite::LayerMap::const_iterator it = sprite.m_map.begin(); it != sprite.m_map.end(); it++)
  {
    LayerSet ls;
    const SpriteLayer& sl = it->second;

    // TODO Skip non visible layers
    ls.m_currentTexture = sl.texIndex;
    ls.m_currentColour = sl.colIndex;

    m_layers.push_back(ls);
  }
}

void LayerGroups::SendToServer()
{
  int id = GetLocalPlayerId();

  int size = m_layers.size();
  for (int i = 0; i < size; i++)
  {
    LayerSet& g = m_layers[i];
    // Send in form sprite_texture_0 = texture.png, sprite_colour_0 = ff00ff.
    // OR could be indices into lists held by groups manager.

    std::string key = "sprite_tex_" + ToString(i);
    std::string val = ToString(g.m_currentTexture);
    TheObjectUpdater::Instance()->SendUpdateReq(id, key, val);

    key = "sprite_col_" + ToString(i);
    val = ToString(g.m_currentColour);
    TheObjectUpdater::Instance()->SendUpdateReq(id, key, val);
  }
}
}

