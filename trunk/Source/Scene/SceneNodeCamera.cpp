#include "SceneNodeCamera.h"
#include "AmjuGL.h"

namespace Amju
{
SceneNodeCamera::SceneNodeCamera()
{

}

void SceneNodeCamera::Draw()
{
  // Don't set Identity - camera can be attached to something; multiple cameras..?
  AmjuGL::LookAt(
    m_eye.x, m_eye.y, m_eye.z,  // eye
    m_lookat.x, m_lookat.y, m_lookat.z,  // target
    m_up.x, m_up.y, m_up.z); // up
}
}
