#include <AmjuFirst.h>
#include <GuiButton.h>
#include <GuiTextEdit.h>
#include <AmjuGL.h>
#include <ConfigFile.h>
#include "SaveConfig.h"
#include "GSProxy.h"
#include <AmjuFinal.h>

namespace Amju
{
static const char* PROXY_HOST_KEY = "proxy_host";
static const char* PROXY_PORT_KEY = "proxy_port";
static const char* PROXY_USER_KEY = "proxy_user";
static const char* PROXY_PW_KEY = "proxy_pw";

static void OnUseProxy()
{
}

void SetProxy()
{
  static GameConfigFile* config = TheGameConfigFile::Instance();

  std::string serverStr = config->GetValue(PROXY_HOST_KEY, "");
  int portNum = ToInt(config->GetValue(PROXY_PORT_KEY));
  std::string userStr = config->GetValue(PROXY_USER_KEY, "");
  std::string pwStr = config->GetValue(PROXY_PW_KEY, "");
  
  HttpClient::SetProxy(serverStr, portNum, userStr, pwStr);
}

static void OnProxyOkButton()
{
  TheGSProxy::Instance()->OnOk();
}

static void OnProxyCancelButton()
{
  TheGSProxy::Instance()->GoBack(); 
}

GSProxy::GSProxy()
{
}

void GSProxy::Update()
{
  GSGui::Update();

}

void GSProxy::Draw()
{
  GSGui::Draw();

}

void GSProxy::Draw2d()
{
  GSGui::Draw2d();
}

void GSProxy::OnOk()
{
  // Make the changes then go back

  // TODO Sanity check input before changing the proxy settings and saving to config!!

  GuiTextEdit* server = (GuiTextEdit*)GetElementByName(m_gui, "proxy-hostname-text");
  std::string serverStr = server->GetText();

  GuiTextEdit* port = (GuiTextEdit*)GetElementByName(m_gui, "proxy-port-text");
  int portNum = ToInt(port->GetText());

  GuiTextEdit* user = (GuiTextEdit*)GetElementByName(m_gui, "proxy-username-text");
  std::string userStr = user->GetText(); 
  
  GuiTextEdit* pw = (GuiTextEdit*)GetElementByName(m_gui, "proxy-pw-text");
  std::string pwStr = pw->GetText();

  // TODO If no proxy, set serverStr to ""

std::cout << "Setting HttpClient Proxy: " << serverStr << "/" << portNum << "/" << userStr << "/" << pwStr << "\n";
  HttpClient::SetProxy(serverStr, portNum, userStr, pwStr);

  // Save these settings in config
  static GameConfigFile* config = TheGameConfigFile::Instance();
  config->Set(PROXY_HOST_KEY, serverStr);
  config->Set(PROXY_PORT_KEY, ToString(portNum));
  config->Set(PROXY_USER_KEY, userStr);
  config->Set(PROXY_PW_KEY, pwStr);
  SaveConfig();

  GoBack();
}

void GSProxy::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-proxy.txt");
  Assert(m_gui);

  // Set focus element, cancel element, command handlers
  GuiButton* start = (GuiButton*)GetElementByName(m_gui, "proxy-ok-button");
  start->SetCommand(Amju::OnProxyOkButton);
  start->SetHasFocus(true);

  GuiButton* quit = (GuiButton*)GetElementByName(m_gui, "proxy-cancel-button");
  quit->SetCommand(Amju::OnProxyCancelButton);
  quit->SetIsCancelButton(true);

  // Restore previous values from config
  // Should be per-user, so get from player info ????
  // If no player info, try game config
  static GameConfigFile* config = TheGameConfigFile::Instance();

  GuiTextEdit* server = (GuiTextEdit*)GetElementByName(m_gui, "proxy-hostname-text");
  server->SetText(config->GetValue(PROXY_HOST_KEY, ""));

  GuiTextEdit* port = (GuiTextEdit*)GetElementByName(m_gui, "proxy-port-text");
  port->SetText(config->GetValue(PROXY_PORT_KEY));

  GuiTextEdit* user = (GuiTextEdit*)GetElementByName(m_gui, "proxy-username-text");
  user->SetText(config->GetValue(PROXY_USER_KEY, ""));
  
  GuiTextEdit* pw = (GuiTextEdit*)GetElementByName(m_gui, "proxy-pw-text");
  pw->SetText(config->GetValue(PROXY_PW_KEY, ""));

  GuiButton* useProxy = (GuiButton*)GetElementByName(m_gui, "use-proxy-button");
  useProxy->SetCommand(OnUseProxy);
}

} // namespace
