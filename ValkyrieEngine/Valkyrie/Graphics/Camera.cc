
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
      m_topLeft.Set(-m_left / m_near, -m_top / m_near, -1.0f);
      m_topRight.Set(-m_right / m_near,- m_top / m_near, -1.0f);
      m_bottomLeft.Set(-m_left / m_near, -m_bottom / m_near, -1.0f);
      m_bottomRight.Set(-m_right / m_near, -m_bottom / m_near, -1.0f);
      printf("Perspective:\n");
      printf("   TopLeft    : <%.2f %.2f %.2f>\n", m_topLeft.x, m_topLeft.y, m_topLeft.z);
      printf("   TopRight   : <%.2f %.2f %.2f>\n", m_topRight.x, m_topRight.y, m_topRight.z);
      printf("   BottomLeft : <%.2f %.2f %.2f>\n", m_bottomLeft.x, m_bottomLeft.y, m_bottomLeft.z);
      printf("   BottomRight: <%.2f %.2f %.2f>\n", m_bottomRight.x, m_bottomRight.y, m_bottomRight.z);

    }
    break;
  case ePM_Orthographic:
    {
      m_right = m_orthographicViewport.x;
      m_left = -m_right;
      m_top = m_orthographicViewport.y;
      m_bottom = -m_top;
      printf("Ortho: %.2f %.2f %.2f %.2f\n", m_left, m_right, m_bottom, m_top);
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


void vkCamera::GetPlanePoints(float distance, vkVector3f *points) const
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

  /*
  printf("GetPlanePoints: %f <%.2f %.2f %.2f> <%.2f %.2f %.2f>\n", distance, m_eye.x, m_eye.y, m_eye.z, m_spot.x, m_spot.y, m_spot.z);
  printf("    <%.2f %.2f %.2f>\n", points[0].x, points[0].y, points[0].z);
  printf("    <%.2f %.2f %.2f>\n", points[1].x, points[1].y, points[1].z);
  printf("    <%.2f %.2f %.2f>\n", points[2].x, points[2].y, points[2].z);
  printf("    <%.2f %.2f %.2f>\n", points[3].x, points[3].y, points[3].z);
  */
}