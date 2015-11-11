
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
      float r = sin(m_perspectiveAngle) * m_near;
      float l = -r;
      float t = m_perspectiveAspect * r;
      float b = -t;
      m_projectionMatrix.SetPerspective(l, r, b, t, m_near, m_far);
      m_projectionMatrixInv.SetPerspectiveInv(l, r, b, t, m_near, m_far);
    }
    break;
  case ePM_Orthographic:
    {
      float r = m_orthographicViewport.x;
      float l = -r;
      float t = m_orthographicViewport.y;
      float b = -t;
      m_projectionMatrix.SetOrthographic(l, r, b, t, m_near, m_far);
      m_projectionMatrixInv.SetOrthographicInv(l, r, b, t, m_near, m_far);
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