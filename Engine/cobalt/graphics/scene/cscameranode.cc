
#include <cobalt/graphics/scene/cscameranode.hh>
#include <cobalt/graphics/cscamera.hh>


cs::CameraNode::CameraNode()
  : cs::SpatialNode()
  , m_camera(0)
{
}

cs::CameraNode::~CameraNode()
{
  CS_RELEASE(m_camera);
}


void cs::CameraNode::SetCamera(cs::Camera *camera)
{
  CS_SET(m_camera, camera);
}


void cs::CameraNode::TransformationChanged()
{
  cs::SpatialNode::TransformationChanged();

  if (m_camera)
  {
    const cs::Matrix4f &m = GetMatrix();

    cs::Vector3f e, v;
    m_camera->SetUp(m.GetZAxis(v));
    m_camera->SetEye(m.GetTranslation(e));
    m.GetYAxis(v);
    cs::Vector3f::Add(e, v, e);
    m_camera->SetSpot(e);
    m_camera->UpdateCameraMatrices();
  }
}