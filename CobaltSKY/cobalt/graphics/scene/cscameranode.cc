
#include <cobalt/graphics/scene/cscameranode.hh>
#include <cobalt/graphics/cscamera.hh>


csCameraNode::csCameraNode()
  : csSpatialNode()
  , m_camera(0)
{
}

csCameraNode::~csCameraNode()
{
  CS_RELEASE(m_camera);
}


void csCameraNode::SetCamera(csCamera *camera)
{
  CS_SET(m_camera, camera);
}


void csCameraNode::TransformationChanged()
{
  csSpatialNode::TransformationChanged();

  if (m_camera)
  {
    const csMatrix4f &m = GetMatrix();

    csVector3f e, v;
    m_camera->SetUp(m.GetZAxis(v));
    m_camera->SetEye(m.GetTranslation(e));
    m.GetYAxis(v);
    csVector3f::Add(e, v, e);
    m_camera->SetSpot(e);
    m_camera->UpdateCameraMatrices();
  }
}