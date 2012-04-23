#include "TextToSpeech.h"

#ifdef WIN32
// MS Speech API
// See http://msdn.microsoft.com/en-us/library/ms720163%28v=vs.85%29.aspx

#define _ATL_APARTMENT_THREADED
#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>

// Windows: Yoiks, uses SAPI
#include <sapi.h>
#pragma comment(lib, "sapi.lib")

#endif // WIN32

#ifdef MACOSX

#include <ApplicationServices/ApplicationServices.h>

#endif // MACOSX

namespace Amju
{
struct SpeechInit
{
  SpeechInit()
  {
#ifdef WIN32
    ::CoInitialize(0);
    pVoice = 0;

    HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
    if (!SUCCEEDED(hr))
    {
        pVoice = 0;
    }
#endif

  }

  ~SpeechInit()
  {
#ifdef WIN32
    if (pVoice)
    {
      pVoice->Release();
    }

    ::CoUninitialize();
#endif

  }

  void Speak(const std::string& s)
  {
#ifdef WIN32
    std::wstring ws;
    ws.assign(s.begin(), s.end());
    HRESULT hr = pVoice->Speak(ws.c_str(), SPF_PURGEBEFORESPEAK | SPF_ASYNC, NULL);
#endif

#ifdef MACOSX

  // From my old code, TODO

  // Make a "pascal-style" string. By trial and error I found that this means with an 8-bit
  // string length at the start.
  static char buf[500];
  short len = s.size(); //strlen(s.size);
  buf[0] = len & 0x00ff;
  //buf[1] = (len & 0xff00) >> 8;
  strcpy(&buf[1], &s[0]);
  OSErr res = SpeakString((const unsigned char*)buf);

#endif

  }

#ifdef WIN32
  ISpVoice * pVoice;
#endif
};

void TextToSpeech(const std::string& text)
{
  static SpeechInit spinit;

  spinit.Speak(text);
}
}
