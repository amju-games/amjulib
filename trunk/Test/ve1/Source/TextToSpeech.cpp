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
    HRESULT hr = pVoice->Speak(ws.c_str(), 0, NULL);
#endif

  }

  ISpVoice * pVoice;
};

void TextToSpeech(const std::string& text)
{
  static SpeechInit spinit;

  spinit.Speak(text);
}
}
