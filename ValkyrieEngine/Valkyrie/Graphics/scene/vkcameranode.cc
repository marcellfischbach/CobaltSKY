
#include <Valkyrie/Graphics/scene/vkcameranode.hh>
#include <Valkyrie/Graphics/Camera.hh>


vkCameraNode::vkCameraNode()
  : vkSpatialNode()
  , m_camera(0)
{
}

vkCameraNode::~vkCameraNode()
{
  VK_RELEASE(m_camera);
}


void vkCameraNode::SetCamera(vkCamera *camera)
{
  VK_SET(m_camera, camera);
}


void vkCameraNode::TransformationChanged()
{
  vkSpatialNode::TransformationChanged();

  if (m_camera)
  {
    const vkMatrix4f &m = GetMatrix();

    vkVector3f e, v;
    m_camera->SetUp(m.GetZAxis(v));
    m_camera->SetEye(m.GetTranslation(e));
    m.GetYAxis(v);
    vkVector3f::Add(e, v, e);
    m_camera->SetSpot(e);
    m_camera->UpdateCameraMatrices();
  }
}