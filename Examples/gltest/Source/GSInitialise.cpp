#include <Game.h>
#include "GSAnimTest.h"
#include "GSBogusSSAO.h"
#include "GSDepthOfField.h"
#include "GSDepthOfField.h"
#include "GSEnvMap.h"
#include "GSFireTemple.h"
#include "GSInitialise.h"
#include "GSLighting.h"
#include "GSMandel.h"
#include "GSNormalMap.h"
#include "GSRenderColourToTexture.h"
#include "GSRenderDepthToTexture.h"
#include "GSShadow2.h"
#include "GSShaderWave.h"
#include "GSPatchwork.h"
#include "GSTerrain.h"
#include "GSRayTrace.h"
#include "GSWater1.h"
#include "GSWaterTemple.h"

#include "StateList.h"

namespace Amju
{
GSInitialise::GSInitialise()
{
  // TODO Serialise last choice in config
  m_nextState = TheGSRayTrace::Instance();
//  m_maxTime = 0.1f;
}

void GSInitialise::OnActive()
{
  GSBase::OnActive();

  AddState("Teapot - Normal map", TheGSNormalMap::Instance());
  AddState("Teapot - Env map", TheGSEnvMap::Instance());
  AddState("Ray Trace", TheGSRayTrace::Instance());
  AddState("Zelda Fire Temple", TheGSFireTemple::Instance());
  AddState("Zelda Water Temple", TheGSWaterTemple::Instance());
  AddState("Water1", TheGSWater1::Instance());
  AddState("Anim", TheGSAnimTest::Instance());
  AddState("Patchwork", TheGSPatchwork::Instance());
  AddState("Terrain", TheGSTerrain::Instance());
  AddState("Mandelbrot shader", TheGSMandel::Instance());
  AddState("Lighting", TheGSLighting::Instance());
  AddState("Bogus SSAO", TheGSBogusSSAO::Instance());
  AddState("Render colour to texture", TheGSRenderColourToTexture::Instance());
  AddState("Render depth to texture", TheGSRenderDepthToTexture::Instance());
  AddState("Depth of field", TheGSDepthOfField::Instance());
  AddState("Shadows 2", TheGSShadow2::Instance());
  AddState("Wave", TheGSShaderWave::Instance());
}

void GSInitialise::Update()
{
  GSBase::Update();
  TheGame::Instance()->SetCurrentState(m_nextState);
}

void GSInitialise::DrawScene()
{
}

bool GSInitialise::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSInitialise::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
