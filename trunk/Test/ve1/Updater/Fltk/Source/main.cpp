// Client updater using FLTK for GUI

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Text_Display.H>
//#include <FL/Fl_PNG_Image.H>
#include "../../Generic/Source/Updater.h"

using namespace Amju;

Fl_Text_Display* textdisp = 0; 
Fl_Text_Buffer* textbuf = 0; 

void Report(const char* s)
{
  Fl::lock();
  textbuf->append(s);
  Fl::unlock();
  Fl::awake(textdisp);
}

int main(int argc, char **argv) 
{
  static const int WIDTH = 400;
  static const int HEIGHT = 300;

  textbuf = new Fl_Text_Buffer;

  Fl_Window *window = new Fl_Window(WIDTH, HEIGHT, "My Game Updater");

  textdisp = new Fl_Text_Display(0, 0, WIDTH, HEIGHT);
  textdisp->buffer(textbuf);
  textdisp->wrap_mode(Fl_Text_Display::WRAP_AT_PIXEL, WIDTH - 10); // TODO width ok ?

  textbuf->append("Checking for updated client.\n");

  // Hmm, link error. Need to configure fltk with png support ?
  // (Tried -lfltk_png)
//  Fl_PNG_Image* png = new Fl_PNG_Image("mygame.png");

  window->end();
  window->show(argc, argv);

  // From threads example
  Fl::lock();

  Updater* up = new Updater(Report);
  up->Start();

  return Fl::run();
}
