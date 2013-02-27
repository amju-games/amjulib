#include <AmjuGL.h>
#include <Colour.h>
#include <ResourceManager.h>
#include "GS2dAvatarMod.h"
#include "Sprite.h"

namespace Amju
{
struct LayerGroup
{
  typedef std::vector<PTexture> Textures;
  typedef std::vector<Colour> Colours;

  Textures textures;
  Colours colours;
  std::string name;

  bool Load(File*);
};

// Has a vec of LayerGroup, and a currently active group.
// Within the currently active group, we can traverse back and forward
//  through the textures and colours, independently of eachother.
struct LayerGroups
{
  typedef std::vector<LayerGroup> Groups;
  Groups m_groups;

  bool Load(File*);

  void NextLayer();
  void PrevLayer();
  // Go to the next/prev texture for the current layer
  void NextTexture();
  void PrevTexture();
  void NextColour();
  void PrevColour();
  int GetNumTexures() const;
  int GetNumColours() const;

  void SetSprite(LayerSprite*);
};

static LayerGroups layerGroups;

bool LayerGroup::Load(File* f)
{
  if (!f->GetDataLine(&name))
  {
    f->ReportError("Failed to load layer group name");
    return false;
  }
  int numTextures = 0;
  if (!f->GetInteger(&numTextures))
  {
    f->ReportError("Failed to load num textures for layer group " + name);
    return false;
  }
  for (int i = 0; i < numTextures; i++)
  {
    std::string t;
    if (!f->GetDataLine(&t))
    {
      f->ReportError("Failed to load texture name for layer group " + name);
      return false;
    }
    Texture* tex = (Texture*)TheResourceManager::Instance()->GetRes(t);
    if (!tex)
    {
      f->ReportError("Failed to load texture " + t + " for layer group " + name);
      return false;
    }
    textures.push_back(tex);
  }
std::cout << "Layer group: loaded " << numTextures << " textures ok.\n";

  int numColours = 0;
  if (!f->GetInteger(&numColours))
  {
    f->ReportError("Failed to load num colours for layer group " + name);
    return false;
  }
  for (int i = 0; i < numColours; i++)
  {
    std::string s;
    if (!f->GetDataLine(&s))
    {
      f->ReportError("Failed to load colour for layer group " + name);
      return false;
    }
    Colour col = FromHexString(s); // TODO error checking ?
    colours.push_back(col);
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

GS2dAvatarMod::GS2dAvatarMod()
{
  // Read from file
  File f;
  if (!f.OpenRead("layergroups.txt"))
  {
    f.ReportError("Failed to load layer groups file for 2D avatars");
    Assert(0); 
  }
  layerGroups.Load(&f); 
}

void GS2dAvatarMod::Update()
{
  GSGui::Update();

}

void GS2dAvatarMod::Draw()
{
  GSGui::Draw();

}

void GS2dAvatarMod::Draw2d()
{
  GSGui::Draw2d();
}

void GS2dAvatarMod::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-avatarmod-2d.txt");
  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
}

} // namespace
