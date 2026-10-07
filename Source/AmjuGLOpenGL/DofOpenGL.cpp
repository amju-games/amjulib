// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026

#include "precomp.h" // first include

#include "DofOpenGL.h"
#include "RenderToTextureES2.h"

namespace Amju
{
//static RenderToTextureES2 renderToTexture;

void DofOpenGL::Draw()
{
  //renderToTexture.Start();
  m_drawFunc();
  //renderToTexture.End();
}

bool DofOpenGL::Init() 
{
  //renderToTexture.Init();
	return true;
}

}

