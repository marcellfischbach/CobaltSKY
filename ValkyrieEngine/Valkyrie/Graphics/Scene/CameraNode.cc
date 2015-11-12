
#include <Valkyrie/Graphics/Scene/CameraNode.hh>
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
    m_camera->SetUp(m.GetYAxis(v));
    m_camera->SetEye(m.GetTranslation(e));
    m.GetZAxis(v);
    vkVector3f::Sub(e, v, e);
    m_camera->SetSpot(e);
    /*
    printf("%.2f %.2f %.2f    %.2f %.2f %.2f   %.2f %.2f %.2f\n",
           m_camera->GetEye().x,
           m_camera->GetEye().y,
           m_camera->GetEye().z,
           m_camera->GetSpot().x,
           m_camera->GetSpot().y,
           m_camera->GetSpot().z,
           m_camera->GetUp().x,
           m_camera->GetUp().y,
           m_camera->GetUp().z);
    */
    m_camera->UpdateCameraMatrices();
  }
}