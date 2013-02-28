#include "LayerGroup.h"
#include "ObjectUpdater.h"
#include "LocalPlayer.h"

namespace Amju
{

bool LayerGroup::Load(File* f)
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


bool LayerGroups::Load(File* f)
{
  int numGroups = 0;
  if (!f->GetInteger(&numGroups))
  {
    f->ReportError("Expected num groups");
    return false;
  }

  m_groups.reserve(numGroups);

  for (int i = 0; i < numGroups; i++)
  {
    LayerGroup g;
    if (!g.Load(f))
    {
      return false;
    }
    m_groups.push_back(g);
  }
std::cout << "Success! Loaded " << numGroups << " layer groups.\n";
  return true;
}

int LayerGroups::GetCurrentLayer() const
{
  return m_currentLayer;
}


void LayerGroups::NextLayer()
{
  m_currentLayer++;
  if (m_currentLayer >= (int)m_groups.size())
  {
    m_currentLayer = 0;
  }
}

void LayerGroups::PrevLayer()
{
  m_currentLayer--;
  if (m_currentLayer < 0)
  {
    m_currentLayer = m_groups.size() - 1;
  }
}

void LayerGroups::NextTexture()
{
  LayerGroup& g = m_groups[m_currentLayer];
  g.m_currentTexture++;
  if (g.m_currentTexture >= (int)g.m_textures.size())
  {
    g.m_currentTexture = 0;
  }
}

void LayerGroups::PrevTexture()
{
  LayerGroup& g = m_groups[m_currentLayer];
  g.m_currentTexture--;
  if (g.m_currentTexture < 0)
  {
    g.m_currentTexture = g.m_textures.size() - 1;
  }
}


void LayerGroups::NextColour()
{
  LayerGroup& g = m_groups[m_currentLayer];
  g.m_currentColour++;
  if (g.m_currentColour >= (int)g.m_colours.size())
  {
    g.m_currentColour = 0;
  }
}

void LayerGroups::PrevColour()
{
  LayerGroup& g = m_groups[m_currentLayer];
  g.m_currentColour--;
  if (g.m_currentColour < 0)
  {
    g.m_currentColour = g.m_colours.size() - 1;
  }
}

int LayerGroups::GetNumTextures() const
{
  const LayerGroup& g = m_groups[m_currentLayer];
  return g.m_textures.size();
}

int LayerGroups::GetNumColours() const
{
  const LayerGroup& g = m_groups[m_currentLayer];
  return g.m_colours.size();
}


void LayerGroups::SetSprite(LayerSprite* sprite)
{
  sprite->Clear();
  int size = m_groups.size();
  for (int i = 0; i < size; i++)
  {
    LayerGroup& g = m_groups[i];
    SpriteLayer sl(i, g.m_textures[g.m_currentTexture], g.m_colours[g.m_currentColour]);
    sprite->AddLayer(sl);
  }
}

void LayerGroups::SetFromSprite(const LayerSprite& sprite)
{
}

void LayerGroups::SendToServer()
{
  int id = GetLocalPlayerId();

  int size = m_groups.size();
  for (int i = 0; i < size; i++)
  {
    LayerGroup& g = m_groups[i];
    // Send in form sprite_texture_0 = texture.png, sprite_colour_0 = ff00ff.
    // OR could be indices into lists held by groups manager.

    std::string key = "sprite_texture_" + ToString(i);
    std::string val = ToString(g.m_currentTexture);
    TheObjectUpdater::Instance()->SendUpdateReq(id, key, val);

    key = "sprite_colour_" + ToString(i);
    val = ToString(g.m_currentColour);
    TheObjectUpdater::Instance()->SendUpdateReq(id, key, val);
  }
}
}

