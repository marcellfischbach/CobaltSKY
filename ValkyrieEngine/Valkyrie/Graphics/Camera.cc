
#include <Valkyrie/Graphics/Camera.hh>
#include <Valkyrie/Graphics/IRenderer.hh>


vkCamera::vkCamera()
  : vkObject()
  , m_near(1.0f)
  , m_far(1024.0f)
{
  m_cameraMatrix.SetIdentity();
  m_cameraMatrixInv.SetIdentity();
  m_projectionMatrix.SetIdentity();
  m_projectionMatrixInv.SetIdentity();
}

vkCamera::~vkCamera()
{

}

void vkCamera::Apply(IRenderer *renderer) const
{
  renderer->SetViewMatrix(m_cameraMatrix);
  renderer->SetViewMatrixInv(m_cameraMatrixInv);
  renderer->SetProjectionMatrix(m_projectionMatrix);
  renderer->SetProjectionMatrixInv(m_projectionMatrixInv);
}

void vkCamera::UpdateCameraMatrices()
{
  m_cameraMatrix.SetLookAt(m_eye, m_spot, m_up);
  m_cameraMatrixInv.SetLookAtInv(m_eye, m_spot, m_up);
}

void vkCamera::UpdateProjectionMatrices()
{
  switch (m_projectionMode)
  {
  case ePM_Perspective:
    {
      m_right  = sin(m_perspectiveAngle) * m_near;
      m_left = -m_right;
      m_top = m_perspectiveAspect * m_right;
      m_bottom = -m_top;
      m_projectionMatrix.SetPerspective(m_left, m_right, m_bottom, m_top, m_near, m_far);
      m_projectionMatrixInv.SetPerspectiveInv(m_left, m_right, m_bottom, m_top, m_near, m_far);
      m_topLeft.Set(m_left, m_top, -m_near);
      m_topRight.Set(m_right, m_top, -m_near);
      m_bottomLeft.Set(m_left, m_bottom, -m_near);
      m_bottomRight.Set(m_right, m_bottom, -m_near);
      m_topLeft.Normalize();
      m_topRight.Normalize();
      m_bottomLeft.Normalize();
      m_bottomRight.Normalize();
    }
    break;
  case ePM_Orthographic:
    {
      m_right = m_orthographicViewport.x;
      m_left = -m_right;
      m_top = m_orthographicViewport.y;
      m_bottom = -m_top;
      m_projectionMatrix.SetOrthographic(m_left, m_right, m_bottom, m_top, m_near, m_far);
      m_projectionMatrixInv.SetOrthographicInv(m_left, m_right, m_bottom, m_top, m_near, m_far);
    }
    break;
  }


}

void vkCamera::SetPerspective(float angle, float aspect)
{
  m_perspectiveAngle = angle;
  m_perspectiveAspect = aspect;
  m_projectionMode = ePM_Perspective;

  UpdateProjectionMatrices();
}

void vkCamera::SetOrthographic(const vkVector2f &viewport)
{
  m_orthographicViewport = viewport;
  m_projectionMode = ePM_Orthographic;

  UpdateProjectionMatrices();
}


void vkCamera::GetPlanePoints(float distance, vkVector3f points[4])
{
  switch (m_projectionMode)
  {
  case ePM_Perspective:
    vkVector3f::Mul(m_bottomLeft, distance, points[0]);
    vkVector3f::Mul(m_bottomRight, distance, points[1]);
    vkVector3f::Mul(m_topLeft, distance, points[2]);
    vkVector3f::Mul(m_topRight, distance, points[3]);
    break;
  case ePM_Orthographic:
    points[0].Set(-m_orthographicViewport.x, -m_orthographicViewport.y, -distance);
    points[1].Set(m_orthographicViewport.x, -m_orthographicViewport.y, -distance);
    points[2].Set(-m_orthographicViewport.x, m_orthographicViewport.y, -distance);
    points[3].Set(m_orthographicViewport.x, m_orthographicViewport.y, -distance);
    break;
  }

  vkMatrix4f::Transform(m_cameraMatrixInv, points[0], points[0]);
  vkMatrix4f::Transform(m_cameraMatrixInv, points[1], points[1]);
  vkMatrix4f::Transform(m_cameraMatrixInv, points[2], points[2]);
  vkMatrix4f::Transform(m_cameraMatrixInv, points[3], points[3]);
}