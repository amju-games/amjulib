#include <EventPoller.h>
#include <AmjuGL.h>
#include <CursorManager.h>
#include <Game.h>
#include <SafeUtils.h>
#include <GameObjectFactory.h>
#include <GuiListBox.h>
#include <GuiTextEdit.h>
#include "GSEdit.h"
#include "GSNetError.h"
#include "Ve1SceneGraph.h"
#include "PickObject.h"
#include "ObjectManager.h"
#include "ObjectUpdater.h"
#include "MsgManager.h"
#include "LocalPlayer.h"
#include "GSNewLocation.h"
#include "Portal.h" 
#include "Useful.h"
#include "GSObjMesh.h"
#include "Portal.h"
#include "Building.h"
#include "Furniture.h"
#include "Baddie.h"
#include "TutorialNpc.h"
#include "GSQuitGame.h"

namespace Amju
{
void OnDone()
{
  // TODO Or pause menu ?
  TheGSQuitGame::Instance()->SetPrevState(TheGSEdit::Instance());
  TheGame::Instance()->SetCurrentState(TheGSQuitGame::Instance());
}

class DelReq : public Ve1Req
{
public:
  DelReq(const std::string& url) : Ve1Req(url, "delete") {}
  virtual void OnSuccess()
  {
    // ?
  }
};

void SendDelReq(int id)
{
  std::string url = TheVe1ReqManager::Instance()->MakeUrl(DELETE_OBJECT);
  url += "&obj_id=";
  url += ToString(id);
  TheVe1ReqManager::Instance()->AddReq(new DelReq(url));
}

class GetLocations : public Ve1Req
{
public:
  GetLocations(const std::string& url) : Ve1Req(url, "get locations") { }

  virtual void OnSuccess()
  {
    PXml p = m_xml.getChildNode(1);
    if (SafeStrCmp(p.getName(), "locs"))
    {
#ifdef XML_DEBUG
std::cout << "Found locs element\n";
#endif
  
      int num = p.nChildNode();

      Locs locs;
      locs.reserve(num);

      for (int i = 0; i < num; i++)
      {
        PXml child = p.getChildNode(i);
        if (!SafeStrCmp(child.getName(), "loc"))
        {
std::cout << "Got 'locs' but no 'loc' child...\n";
          OnFailure();
        }
        // Children of this 'loc' node: 'id' and 'name'
       
        int n = child.nChildNode();
        if (n != 2)
        {
std::cout << "Expected 2 children of 'loc' node!\n";
          OnFailure();
        } 
        int id = ToInt(child.getChildNode(0).getText());
        std::string name = child.getChildNode(1).getText();

std::cout << "Got new location: " << id << name << "\n";
        locs.push_back(Loc(id, name));
      }

std::cout << "Setting new location list.\n";
      TheGSEdit::Instance()->SetLocs(locs);
    } 
    else
    {
std::cout << "Get locations: expected 'locs' element\n";
      OnFailure();
    }  
  }
};

class ReqNewObjId : public Ve1Req
{
public:
  ReqNewObjId(const std::string& url, const std::string& type) : 
    Ve1Req(url, "new object id"), m_typename(type) { }

  virtual void OnSuccess()
  {
    PXml p = m_xml.getChildNode(1);
    if (SafeStrCmp(p.getName(), "id"))
    {
#ifdef XML_DEBUG
std::cout << "Found id element\n";
#endif

      std::string id = p.getText();
std::cout << "Got new object ID from server! " << id << "\n";

#ifdef USE_PROTO_OBJS
      Ve1Object* go = new ProtoObject; 
#else
      Ve1Object* go = (Ve1Object*)TheGameObjectFactory::Instance()->Create(m_typename);
      go->CreateEditNode();
#endif

      Assert(go); // Typename should be from Factory, so Create should always succeed, right ?
      if (go)
      {
std::cout << "Success! Created object of type " << m_typename << "\n";
        go->SetId(ToInt(id));

        // TODO Set location and pos
        go->SetLocation(GetLocalPlayerLocation());

        // Assets & Data file ??
        // Add to local game ? Or send req to server to create ?
        // Just add to the local game, NOT the DB or the local cache
        TheGame::Instance()->AddGameObject(go);
        go->OnLocationEntry(); 
      }
      else
      {
std::cout << "Failed to create object of type " << m_typename << "\n";
      }
      TheGSEdit::Instance()->OnNewObjectId(ToInt(id));
    }
    else
    {
      OnFailure();
    }
  }

  virtual void OnFailure()
  {
std::cout << "FAILED to get new object ID.\n";
    TheGSEdit::Instance()->OnNewObjectId(-1); // ==> error
  }

private:
  std::string m_typename;
};

bool GSEditListener::OnCursorEvent(const CursorEvent& ce)
{
  return TheGSEdit::Instance()->OnCursorEvent(ce);
}

bool GSEditListener::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return TheGSEdit::Instance()->OnMouseButtonEvent(mbe);
}

GSEdit::GSEdit()
{
  m_mouseDownLeft = false;
  m_mouseDownRight = false;
  m_location = 0; // Set in OnActive 
  m_isPicking = false;
  m_newObjectId = -1;
  m_getDragVec = false;
  m_selObj = 0;
  m_listener = new GSEditListener;
}

void GSEdit::SetLocs(const Locs& locs)
{
  m_locs = locs;
}

void GSEdit::OnNewObjectId(int id)
{
  m_newObjectId = id;
}

int GSEdit::GetUniqueId()
{
  // Get ID for a new game object which is globally unique for the entire game.
  // Could query server. Or make one from time + MAC address etc ?

  return 0;
}

void GSEdit::Update()
{
  GSBase::Update();

  GetVe1SceneGraph()->Update();

  // TODO Update online req mgr if we are waiting for something, 
  //  e.g. a new ID or are uploading some content.

  // From GSMain: probably DON'T do these:
  TheObjectManager::Instance()->Update(); 
  TheObjectUpdater::Instance()->Update();
  //TheMsgManager::Instance()->Update();
  TheGame::Instance()->UpdateGameObjects();
  //TheSAP::Instance()->Update(); // sweep & prune

}

void GSEdit::Draw()
{
  AmjuGL::SetClearColour(Colour(1, 1, 0, 1));

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  const float FOVY = 60.0f;
  const float NEAR_PLANE = 1.0f;
  const float FAR_PLANE = 3000.0f;
  float aspect = 1.3f;
  AmjuGL::SetPerspectiveProjection(FOVY, aspect, NEAR_PLANE, FAR_PLANE);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();

  GetVe1SceneGraph()->Draw();
  
  GetVe1SceneGraph()->GetCamera()->Draw();

  if (m_getDragVec)
  {
    m_getDragVec = false;
    // Get up and right vectors 
    Matrix mat;
    mat.ModelView(); // Get Modelview matrix

    // TODO Attempt to fix drag axes:

    m_up = Vec3f(0, 1, 0); //mat[1], mat[5], mat[9]);
    m_right = Vec3f(mat[0], mat[4], mat[8]);

    // Make sure m_right is only in (x, z) plane..?
    m_right.y = 0;
    m_right.Normalise();
  }

  // Draw axes for selected object
  if (m_selObj)
  {
    AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);

    PushColour();
    AmjuGL::SetColour(0, 0, 0, 1);

    Vec3f p0 = m_selObj->GetPos();
    Vec3f p1 = p0 + m_right * 1000.0f;
    AmjuGL::DrawLine(AmjuGL::Vec3(p0.x, p0.y, p0.z), AmjuGL::Vec3(p1.x, p1.y, p1.z));
    p0 = m_selObj->GetPos();
    p1 = p0 + m_up * 1000.0f;
    AmjuGL::DrawLine(AmjuGL::Vec3(p0.x, p0.y, p0.z), AmjuGL::Vec3(p1.x, p1.y, p1.z));

    PopColour();
    AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
  }

  if (m_isPicking)
  {
    m_isPicking = false;

    // Check for selected object
std::cout << "PICKING!\n";
    GameObject* obj = PickObject(m_mouseScreen);

    // Unselect objects -- TODO not if in multi mode
    for (SelObjects::iterator it = m_selectedObjects.begin(); it != m_selectedObjects.end(); ++it)
    {
      Ve1Object* obj = *it;
      obj->SetSelected(false);
    }
    m_selectedObjects.clear();
    // DON'T set m_selObj to 0 -- remember the last selected obj

    if (obj)
    {
std::cout << " - GOT AN OBJECT! " << obj->GetId() << "\n";

      // TODO Multi-select - use Shift and Ctrl like Maya
      if (obj)
      {
        Ve1Object* vobj = dynamic_cast<Ve1Object*>(obj);
        Assert(vobj);
        m_selectedObjects.insert(vobj);
        vobj->SetSelected(true);
        m_selObj = dynamic_cast<Ve1Object*>(obj);
        Assert(m_selObj);
      }
    }

    // when we right click: CreateContextMenu();

    // Change text
    std::string selText;
    if (m_selectedObjects.empty())
    {
      selText = "no selection";
    }
    else if (m_selectedObjects.size() == 1)
    {
      Assert(m_selObj);
      selText = "one object selected (" + ToString(m_selObj->GetId()) + ") " + m_selObj->GetTypeName();
    }
    else
    {
      selText = "multiple objects selected";
    }
    ((GuiText*)GetElementByName(m_gui, "text1"))->SetText(selText);
  }
}

void GSEdit::Draw2d()
{
  GSBase::Draw2d();

  m_gui->Draw();

  if (m_menu)
  {
    m_menu->Draw();
  }

  TheCursorManager::Instance()->Draw();
}

void OnChooseLocOk()
{
  TheGSEdit::Instance()->GoToSelectedLocation();
}

void OnChooseLocCancel()
{
  TheGSEdit::Instance()->ShowLocationList(false);
}

void OnChangeProperty()
{
  TheGSEdit::Instance()->ShowChangeProperty(true);
}

void OnCancelProperty()
{
  TheGSEdit::Instance()->ShowPropertyList(false);
}

void OnPropertySet()
{
  TheGSEdit::Instance()->OnPropertySet();
}

void OnPropertySetCancel()
{
  TheGSEdit::Instance()->ShowChangeProperty(false);
}

void GSEdit::OnPropertySet()
{
  // Change value, update GUI, send change to server
  ShowChangeProperty(false); // Hide change GUI

  // Get new value
  GuiElement* elem = GetElementByName(m_gui, "property-value");
  std::string newVal = ((GuiTextEdit*)elem)->GetText();
  
  // Get the object to update
  Assert(m_selObj);

  // Get the property to change
  elem = GetElementByName(m_gui, "property-list");
  GuiListBox* lb = ((GuiListBox*)elem);
  int sel = lb->GetSelectedItem();
  if (sel < 0)
  {
std::cout << "Nothing selected in list box?\n";
    Assert(0);
    return;
  }
std::cout << "Selected child: " << sel << "\n";
  elem = lb->GetChild(sel);
  Assert(elem);
   
  ValMap* valmap = m_selObj->GetValMap();
  Assert(valmap);
  ValMap::iterator it = valmap->begin();
  std::advance(it, sel);
 
  std::string key = it->first;
std::cout << "Old Property for obj: " << it->first << "=" << it->second << "\n";

  // Send update 
  TheObjectUpdater::Instance()->SendUpdateReq(m_selObj->GetId(), key, newVal);

  // Set locally (or wait for server ?)
  m_selObj->SetKeyVal(key, newVal);

  // Refresh list
  ShowPropsForObj(m_selObj->GetId());
}

void GSEdit::ShowChangeProperty(bool b)
{
  if (b)
  {
    // Check that an item is selected in list box
    GuiElement* elem = GetElementByName(m_gui, "property-list");
    GuiListBox* lb = ((GuiListBox*)elem);
    int sel = lb->GetSelectedItem();
    if (sel < 0)
    {
std::cout << "Nothing selected, can't change!\n";
      return;
    }
    else
    {
      // Get current value, populate text edit box
      ValMap* valmap = m_selObj->GetValMap();
      ValMap::iterator it = valmap->begin();
      std::advance(it, sel);
      std::string val = it->second;
      GuiTextEdit* edit = (GuiTextEdit*)GetElementByName(m_gui, "property-value");
      edit->SetText(val); 
    }
  }

  if (b)
  {
    GetElementByName(m_gui, "property-comp")->SetVisible(true); // parent
  }
  GetElementByName(m_gui, "property-value")->SetVisible(b);
  GetElementByName(m_gui, "property-set")->SetVisible(b);
  GetElementByName(m_gui, "property-set-cancel")->SetVisible(b);
}

void GSEdit::OnActive()
{
  GSBase::OnActive();

  TheGSNetError::Instance()->SetPrevState(this);

  // Make sure nothing selected 
  m_selObj = 0;
  m_selectedObjects.clear();

  // We only want to do this if we decide to reload the world
//  GetVe1SceneGraph()->Clear();

  // Load HUD GUI
  m_gui = LoadGui("gui-edit.txt");
  Assert(m_gui);

  // Set commands for GUI buttons
  m_gui->GetElementByName("done-button")->SetCommand(Amju::OnDone);

  m_location = GetLocalPlayerLocation();
  std::string loc = "Location: " + ToString(m_location);
  ((GuiText*)m_gui->GetElementByName("location_info"))->SetText(loc);

  // Send request for list of all locations
  std::string url = TheVe1ReqManager::Instance()->MakeUrl(GET_ALL_LOCATIONS);
  TheVe1ReqManager::Instance()->AddReq(new GetLocations(url)); 

  // Hide locations list box until we want to choose a new location

  GuiElement* elem = GetElementByName(m_gui, "location-ok");
  elem->SetCommand(OnChooseLocOk);

  elem = GetElementByName(m_gui, "location-cancel");
  elem->SetCommand(OnChooseLocCancel); 

  ShowLocationList(false);
  ShowPropertyList(false);
  GetElementByName(m_gui, "property-change")->SetCommand(OnChangeProperty);
  GetElementByName(m_gui, "property-cancel")->SetCommand(OnCancelProperty);
  GetElementByName(m_gui, "property-set")->SetCommand(Amju::OnPropertySet);
  GetElementByName(m_gui, "property-set-cancel")->SetCommand(OnPropertySetCancel);

  TheEventPoller::Instance()->AddListener(m_listener, 10); // low priority
}

void GSEdit::ShowPropertyList(bool b)
{
  GuiElement* props = GetElementByName(m_gui, "property-comp");
  props->SetVisible(b);
  if (b)
  {
//    GetElementByName(m_gui, "property-comp")->SetVisible(true); // parent
    GetElementByName(m_gui, "property-list")->SetVisible(true);
    GetElementByName(m_gui, "property-change")->SetVisible(true);
    GetElementByName(m_gui, "property-value")->SetVisible(false);
  }

  ShowChangeProperty(false);
}

void GSEdit::ShowLocationList(bool b)
{
  if (!m_gui)
  {
    return;
  }

  GuiElement* locs = GetElementByName(m_gui, "location-list");
  locs->SetVisible(b);
  GuiElement* elem = GetElementByName(m_gui, "location-ok");
  elem->SetVisible(b);
  elem = GetElementByName(m_gui, "location-cancel");
  elem->SetVisible(b);
}

void GSEdit::OnDeactive()
{
  GSBase::OnDeactive();

  TheEventPoller::Instance()->RemoveListener(m_gui);
  m_gui = 0;

  if (m_menu)
  {
    TheEventPoller::Instance()->RemoveListener(m_menu);
  }
  m_menu = 0;

  TheEventPoller::Instance()->RemoveListener(m_listener);

  // TODO Check for outstanding changes, confirm and save to server
}

bool GSEdit::OnCursorEvent(const CursorEvent& ce)
{
  m_mouseScreen = Vec2f(ce.x, ce.y);

  // If dragging, move selected object(s)
  static float oldx = ce.x;
  static float oldy = ce.y;
  float dx = ce.x - oldx;
  float dy = ce.y - oldy;
  oldx = ce.x;
  oldy = ce.y;

  // Move sel objects
  if (m_mouseDownLeft && 
      !m_selectedObjects.empty())
  {
    std::string str;
    for (SelObjects::iterator it = m_selectedObjects.begin(); it != m_selectedObjects.end(); ++it)
    {
      // Move this obj
      GameObject* obj = *it;
      Vec3f pos = obj->GetPos();
      pos += m_right * dx * 100.0f;
      pos += m_up * dy * 100.0f;

      str = ToString(pos); // TODO pos change if multiple objects ? 

      obj->SetPos(pos);

//std::cout << "Should be sending pos update: " << pos << "\n";
      // Don't send, wait til mouse up event
//      TheObjectUpdater::Instance()->SendPosUpdateReq(obj->GetId(), pos, m_location);
    }
    ((GuiText*)GetElementByName(m_gui, "text1"))->SetText(str);
  }

  return false;
}

bool GSEdit::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  // Ignore if a GUI is active - this should happen anyway!!

  m_mouseScreen = Vec2f(mbe.x, mbe.y);

  // Left click: check for clicking on an object
  if (mbe.button == AMJU_BUTTON_MOUSE_LEFT)
  {
    if (!mbe.isDown && m_mouseDownLeft && !m_selectedObjects.empty())
    {
      // Mouse up - if we were dragging something, send its new position now.
      for (SelObjects::iterator it = m_selectedObjects.begin(); it != m_selectedObjects.end(); ++it)
      {
        GameObject* obj = *it;
        Vec3f pos = obj->GetPos();
        TheObjectUpdater::Instance()->SendPosUpdateReq(obj->GetId(), pos, m_location);
      }
    }

    m_mouseDownLeft = mbe.isDown;
//std::cout << "Left Mouse button event! " << (m_mouseDownLeft ? "DOWN" : "UP") << "\n";
    m_isPicking = true;
    m_getDragVec = true; // TODO if key held down
  }
  else if (mbe.button == AMJU_BUTTON_MOUSE_RIGHT)
  {
    m_mouseDownRight = mbe.isDown;
    if (m_mouseDownRight)
    {
      // Create context menu
      CreateContextMenu();
    }
  }

  return false;
}

void OnNewLocation()
{
  TheGSNewLocation::Instance()->SetMode(GSNewLocation::AMJU_ADD_NEW);
  TheGame::Instance()->SetCurrentState(TheGSNewLocation::Instance());
}

void OnEditLocation()
{
  TheGSNewLocation::Instance()->SetMode(GSNewLocation::AMJU_EDIT);
  TheGame::Instance()->SetCurrentState(TheGSNewLocation::Instance());
}

void OnGotoLocation()
{
  TheGSEdit::Instance()->ChooseLocation();
}

void GSEdit::ChooseLocation()
{
  m_menu = 0;

  // Show listbox, populate with all locations
  GuiElement* elem = GetElementByName(m_gui, "location-list");
  GuiListBox* lb = dynamic_cast<GuiListBox*>(elem);
  Assert(lb);
  lb->Clear();
  int s = m_locs.size();
  for (int i = 0; i < s; i++)
  {
    const Loc& loc = m_locs[i];
    GuiText* t = new GuiText;
    t->SetText(ToString(loc.m_id) + " \"" + loc.m_name + "\"");
    lb->AddItem(t);
  }
  ShowLocationList(true);
}

void GSEdit::GoToSelectedLocation()
{
  GuiElement* elem = GetElementByName(m_gui, "location-list");
  GuiListBox* lb = dynamic_cast<GuiListBox*>(elem);
  Assert(lb);
  int n = lb->GetNumChildren();
  int id = -1;
  for (int i = 0; i < n; i++)
  {
    GuiElement* child = lb->GetChild(i);
    if (child->IsSelected())
    {
      id = m_locs[i].m_id;
      break;
    }
  }
  if (id != -1)
  {
    SetLocalPlayerLocation(id); // Go to a new location
  }
}

class NewObjectCommand : public GuiCommand
{
public:
  NewObjectCommand(const std::string& typeName, const std::string& assetfile, const std::string& datafile, bool alive = false) : 
    m_typeName(typeName), m_assetfile(assetfile), m_datafile(datafile), m_alive(alive) { }

  virtual bool Do()
  {
    // Create request to get new ID. Make obj when we get response.
    std::string url = TheVe1ReqManager::Instance()->MakeUrl(GET_NEW_OBJECT_ID);
    url += "&type=" + m_typeName;
    url += "&assetfile=" + m_assetfile + "&datafile=" + m_datafile; 
    if (m_alive)
    {
      url += "&alive=1";
    }

std::cout << "Create new object of type " << m_typeName << " URL: " << url << "\n";

    TheVe1ReqManager::Instance()->AddReq(new ReqNewObjId(url, m_typeName)); 

    return false;
  }

private:
  std::string m_typeName;
  std::string m_assetfile;
  std::string m_datafile;
  bool m_alive; // create with current timestamp, so immediately committed
};

// For baddies and NPCs, set the type and texture as key/val pairs
class NewCharObjectCommand : public NewObjectCommand
{
// 
};

//class SetObjMeshCommand : public GuiCommand
//{
//public:
//  SetObjMeshCommand(int objId) : m_id(objId) {}
//  virtual bool Do()
//  {
//    TheGSObjMesh::Instance()->SetId(m_id);
//    TheGame::Instance()->SetCurrentState(TheGSObjMesh::Instance());
//    return false;
//  }
//
//private:
//  int m_id;
//};

class SetPropsCommand : public GuiCommand
{
public:
  SetPropsCommand(int objId) : m_id(objId)  {}
  virtual bool Do()
  {
    // Set properties in another State ?
//    TheGSSetObjectProperties::Instance()->SetId(m_id);
//    TheGame::Instance()->SetCurrentState(TheGSSetObjectProperties);

    TheGSEdit::Instance()->ShowPropsForObj(m_id);
    return false; // no undo
  }

private:
  int m_id;
};

class DelObjCommand : public GuiCommand
{
public:
  DelObjCommand(int objId) : m_id(objId) {}
  virtual bool Do()
  {
    std::cout << "SORRY DUDE NOT IMPLEMENTED YET\n";
    // Must show confirm yes/no
    static Game* g = TheGame::Instance();
    if (g->GetGameObject(m_id))
    {
      Ve1Object* vo = dynamic_cast<Ve1Object*>(g->GetGameObject(m_id).GetPtr());
      if (vo)
      {
        vo->OnLocationExit();
      }
      g->EraseGameObject(m_id);
    }
    SendDelReq(m_id);
    return false;
  }

private:
  int m_id;
};

void GSEdit::ShowPropsForObj(int id)
{
  GuiListBox* props = (GuiListBox*)GetElementByName(m_gui, "property-list");
  props->Clear();

  Ve1Object* obj = (Ve1Object*)TheGame::Instance()->GetGameObject(id).GetPtr();
  ValMap* valmap = obj->GetValMap();

std::cout << "Here are the properties for the selected object (" << obj->GetId() << "):\n";

  for (ValMap::iterator it = valmap->begin(); it != valmap->end(); ++it)
  {
std::cout << "Property for obj: " << it->first << "=" << it->second << "\n";
    GuiText* t = new GuiText;
    t->SetText(it->first + " = " + it->second);
    props->AddItem(t);
  }

  ShowPropertyList(true);
}

void GSEdit::CreateContextMenu()
{
  m_menu = new GuiMenu;
  TheEventPoller::Instance()->AddListener(m_menu);

  if (m_selectedObjects.empty())
  {
    m_menu->AddChild(new GuiMenuItem("Go to Location...", OnGotoLocation));
    m_menu->AddChild(new GuiMenuItem("Add new Location...", OnNewLocation));
    m_menu->AddChild(new GuiMenuItem("Update this location mesh...", OnEditLocation));

    GuiMenu* childMenu = new GuiMenu;
    childMenu->SetName("Child menu");

    // Get types from Game Object Factory..?
    // TODO Just hard code it, so we can do different things depending on the type.
    // E.g. differnet file names. Automatically commit objects where filenames are known ?
    // For baddies and NPCs, data files could contain all possible values. Then use a (key, val)
    // to set which one. Like for Players.
      
    childMenu->AddChild(new GuiMenuItem("Furniture", 
      new NewObjectCommand(Furniture::TYPENAME, "none", "none")));

    childMenu->AddChild(new GuiMenuItem("Building", 
      new NewObjectCommand(Building::TYPENAME, "none", "none")));

    childMenu->AddChild(new GuiMenuItem("Portal", 
      new NewObjectCommand(Portal::TYPENAME, "none", "none", true)));

    childMenu->AddChild(new GuiMenuItem("Baddie", 
      new NewObjectCommand(Baddie::TYPENAME, "baddie-assets.txt", "baddie-data.txt", true)));

    childMenu->AddChild(new GuiMenuItem("TutorialNPC", 
      new NewObjectCommand(TutorialNpc::TYPENAME, "tutnpc-assets.txt", "tutnpc-data.txt", true)));

    m_menu->AddChild(new GuiNestMenuItem("New Game Object...", childMenu));
  }
  else if (m_selectedObjects.size() == 1)
  {
    Ve1Object* obj = *(m_selectedObjects.begin());
    Assert(obj == m_selObj);

    //?m_menu->Clear(); 

    m_menu->AddChild(new GuiMenuItem("Set properties...", new SetPropsCommand(obj->GetId())));
    obj->SetEditMenu(m_menu); // add type-specific options to menu
    m_menu->AddChild(new GuiMenuItem("Delete this object", new DelObjCommand(obj->GetId())));
  }
  else
  {
    // Multi selection TODO
std::cout << "Create context menu for multiple objects...\n";
  }

  Vec2f pos = m_mouseScreen;
  if (pos.x + m_menu->GetSize().x > 1.0f)
  {
    pos.x = 1.0f - m_menu->GetSize().x;
  }
  if (pos.y - m_menu->GetSize().y < -1.0f)
  {
    pos.y = -1.0f + m_menu->GetSize().y;
  }
  m_menu->SetLocalPos(pos);
}

bool GSEdit::OnKeyEvent(const KeyEvent&)
{
  // TODO Set move command etc for objects with keys ??

  return false;
}

}

