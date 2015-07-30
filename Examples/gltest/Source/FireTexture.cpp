#include "AmjuFirst.h"
#include <iostream>
#include "FireTexture.h"
#include <Timer.h>
#include "AmjuFinal.h"

namespace Amju
{
FireTexture::FireTexture()
{
  AMJU_CALL_STACK;

  for (int i = 0; i < FIRE_ARRAY_SIZE; i++)
  {
    src[i] = 0;
  }  

}

bool FireTexture::Init()
{
  uint32 data[FIRE_ARRAY_SIZE];
  for (int i = 0; i < FIRE_ARRAY_SIZE; i++)
  {
    data[i] = 0x00000000;
  }

  m_tex.Create((unsigned char*)data, FIRE_SIZE, FIRE_SIZE, 4);

  return true;
}

void FireTexture::Update()
{
  AMJU_CALL_STACK;

  int i; 

  static float time = 0;
  float dt = TheTimer::Instance()->GetDt();
  time +=dt;
  if (time < 0.1f)
  {
    return;
  }
  time = 0;

  const int BORDER = 8;
  for (int j = 0; j < FIRE_SIZE; j++)
  {
    if (j < BORDER || j >= FIRE_SIZE - BORDER)
    {
      src[j] = 0;
      src[j + FIRE_SIZE] = 0;
    }
    else
    {
      uint8 r = (uint8)((rand()%128) + 128);  
      uint8 r1 = (uint8)((rand()%128) + 128);  
      src[j] = r;
      src[j + FIRE_SIZE] = r1;
    }
  }

  for (i = 1+2*FIRE_SIZE; i < (FIRE_ARRAY_SIZE - 1); i++)
  {
    int a = src[i-1] + src[i+1] + src[i-1-FIRE_SIZE] + src[i-FIRE_SIZE] + src[i+1-FIRE_SIZE] +
         src[i-1-2*FIRE_SIZE] + src[i-2*FIRE_SIZE] + src[i+1-2*FIRE_SIZE];
    //a = a >> 3;
    a /= 8;
    //std::cout << a << " "; 
    a &= 0xf0;
    //if (a < 16) a = 0;
      
    dst[i] = (uint8)a;
  }

  //std::cout << "\n\n";

  // Copy uint8 data to uint32 data.

  static uint32 data[FIRE_ARRAY_SIZE];
  //uint32* data = (uint32*)m_tex.GetData();
  for (i = 0; i < FIRE_ARRAY_SIZE; i++)
  {
    //cout << "src " << (int)src[i] << " <- dst " << (int)dst[i] << "\n";
    src[i] = dst[i];

// This works for all platforms except wii?
#ifndef GEKKO
    uint32 alpha = ((uint32)dst[i]) << 24; //data[i] & 0xff000000; // ENDIAN
    uint32 res = 0x00ff00ff | alpha; // ENDIAN


#else
    uint32 alpha = 0x000000c0; // data[i] & 0x000000ff; // ENDIAN
    uint32 res = 0xffffff00 | alpha; // ENDIAN
#endif

    data[i] = res;

  }
  m_tex.UseThisTexture();
  AmjuGL::UpdateTexture(m_tex.GetId(), 0, 0, FIRE_SIZE, FIRE_SIZE, (const uint8*)data);

/*
  m_pTexture->Bind();
  glTexSubImage2D(GL_TEXTURE_2D, 
                  0, 
                  0, 
                  0, 
                  FIRE_SIZE, 
                  FIRE_SIZE, 
                  GL_RGBA, 
                  GL_UNSIGNED_BYTE, 
                  m_pTexture->GetData());
*/
}
}

