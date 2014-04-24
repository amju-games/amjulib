#include <Screenshot.h>
#include <SavePng.h>
#include <AmjuGL.h>
#include <Screen.h>
#include "GSVe3Mug.h"
#include "Mugshot.h"
#include "LocalPlayer.h"
#include "Ve1OnlineReqManager.h"

namespace Amju
{
static int frame = 0;

void GSVe3Mug::Draw()
{
}

void GSVe3Mug::Draw2d()
{
  frame++;

  // Just draw Mugshot
  Mugshot m;
  m.Init(GetLocalPlayer());
  AmjuGL::PushMatrix();
  // Move to bottom left, as this is where screenshot starts right now
  AmjuGL::Translate(-1.0f, -0.8f, 0); // TEST
  m.Draw(); 
  AmjuGL::PopMatrix();

  if (frame != 1)
  {
    return;
  }

  // Next frame:
  // Get screenshot
  static const int BYTES_PER_PIXEL = 3;
  // Fraction of screen width and height that the mugshot covers
  static const int SCREEN_FRACTION = 20;
  int w = 64; // TODO Screen::X() / SCREEN_FRACTION;
  int h = 64; // TODO Screen::Y() / SCREEN_FRACTION;
  unsigned char* mem = new unsigned char[BYTES_PER_PIXEL * w * h];
  AmjuGL::GetScreenshot(mem, w, h);

  // Save as .png
  std::string filename = "mugshot.png";
  bool savedPngOk = SavePng(w, h, mem, filename.c_str(), BYTES_PER_PIXEL);
  if (savedPngOk)
  {
    std::cout << "Saved png OK! Filename: " << filename << "\n";
  
    // Upload to server
    std::string url = TheVe1ReqManager::Instance()->MakeUrl(UPLOAD_MUGSHOT);
    url += "&playerid=" + ToString(GetLocalPlayerId());
    std::cout << "Upload to this url: " << url << "\n";

    UploadScreenshot(filename, url);

    AmjuGL::SetClearColour(Colour(0, 0, 0, 1));
    GoBack();
  }
  else
  {
    std::cout << "FAILED to save png! Filename: " << filename << "\n";
  }
}

void GSVe3Mug::OnActive()
{
  frame = 0;
  AmjuGL::SetClearColour(Colour(0.5f, 0.5f, 0.5f, 1));
}

}

