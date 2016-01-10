
#include <Valkyrie/Graphics/Camera.hh>
#include <Valkyrie/Graphics/IGraphics.hh>


vkCamera::vkCamera()
  : vkObject()
  , m_near(1.0f)
  , m_far(1024.0f)
  , m_projectionChanged(false)
  , m_clipper(new vkPlaneClipper())
{
  m_cameraMatrix.SetIdentity();
  m_cameraMatrixInv.SetIdentity();
  m_projectionMatrix.SetIdentity();
  m_projectionMatrixInv.SetIdentity();
}

vkCamera::~vkCamera()
{

}

void vkCamera::Apply(IGraphics *renderer) 
{
  renderer->SetViewMatrix(m_cameraMatrix);
  renderer->SetViewMatrixInv(m_cameraMatrixInv);

  if (m_projectionChanged)
  {
    UpdateProjectionMatrices(renderer);
  }
  renderer->SetProjectionMatrix(m_projectionMatrix);
  renderer->SetProjectionMatrixInv(m_projectionMatrixInv);
}

void vkCamera::UpdateCameraMatrices()
{
  m_cameraMatrix.SetLookAt(m_eye, m_spot, m_up);
  m_cameraMatrixInv.SetLookAtInv(m_eye, m_spot, m_up);
}

void vkCamera::UpdateProjectionMatrices(IGraphics *renderer)
{
  switch (m_projectionMode)
  {
    case ePM_Perspective:
      renderer->GetPerspectiveProjection(m_left, m_right, m_bottom, m_top, m_near, m_far, m_projectionMatrix);
      renderer->GetPerspectiveProjectionInv(m_left, m_right, m_bottom, m_top, m_near, m_far, m_projectionMatrixInv);
      break;
    case ePM_Orthographic:
      renderer->GetOrthographicProjection(m_left, m_right, m_bottom, m_top, m_near, m_far, m_projectionMatrix);
      renderer->GetOrthographicProjectionInv(m_left, m_right, m_bottom, m_top, m_near, m_far, m_projectionMatrixInv);
      break;
  }
  m_projectionChanged = false;
}

void vkCamera::UpdateProjectionValues()
{
  switch (m_projectionMode)
  {
  case ePM_Perspective:
    {
      m_right  = sin(m_perspectiveAngle) * m_near;
      m_left = -m_right;
      m_top = m_perspectiveAspect * m_right;
      m_bottom = -m_top;
      m_topLeft.Set(m_left / m_near, 1.0f, m_top / m_near);
      m_topRight.Set(m_right / m_near, 1.0f, m_top / m_near);
      m_bottomLeft.Set(m_left / m_near, 1.0f, m_bottom / m_near);
      m_bottomRight.Set(m_right / m_near, 1.0f, m_bottom / m_near);
    }
    break;
  case ePM_Orthographic:
    {
      m_right = m_orthographicViewport.x;
      m_left = -m_right;
      m_top = m_orthographicViewport.y;
      m_bottom = -m_top;

      m_topLeft.Set(m_left, m_near, m_top);
      m_topRight.Set(m_right, m_near, m_top);
      m_bottomLeft.Set(m_left, m_near, m_bottom);
      m_bottomRight.Set(m_right, m_near, m_bottom);
    }
    break;
  }

  m_frontNear.Set(0.0f, m_near, 0.0f);
  m_frontFar.Set(0.0f, m_far, 0.0f);
  m_projectionChanged = true;
}

void vkCamera::SetPerspective(float angle, float aspect)
{
  m_perspectiveAngle = angle;
  m_perspectiveAspect = aspect;
  m_projectionMode = ePM_Perspective;

  UpdateProjectionValues();
}

void vkCamera::SetOrthographic(const vkVector2f &viewport)
{
  m_orthographicViewport = viewport;
  m_projectionMode = ePM_Orthographic;

  UpdateProjectionValues();
}


vkClipper *vkCamera::GetClipper()
{
  m_clipper->Clear();
  switch (m_projectionMode)
  {
  case ePM_Perspective:
    {
      vkVector3f tl, tr, bl, br, l, r, b, t, n, f, dir;

      vkMatrix4f::Transform(m_cameraMatrixInv, m_topLeft, tl);
      vkMatrix4f::Transform(m_cameraMatrixInv, m_topRight, tr);
      vkMatrix4f::Transform(m_cameraMatrixInv, m_bottomLeft, bl);
      vkMatrix4f::Transform(m_cameraMatrixInv, m_bottomRight, br);
      vkMatrix4f::Transform(m_cameraMatrixInv, m_frontNear, n);
      vkMatrix4f::Transform(m_cameraMatrixInv, m_frontFar, f);
      vkVector3f::Sub(tl, m_eye, tl);
      vkVector3f::Sub(tr, m_eye, tr);
      vkVector3f::Sub(bl, m_eye, bl);
      vkVector3f::Sub(br, m_eye, br);
      vkVector3f::Sub(f, n, dir);
      vkVector3f::Cross(tl, tr, t);
      vkVector3f::Cross(bl, tl, l);
      vkVector3f::Cross(br, bl, b);
      vkVector3f::Cross(tr, br, r);
      m_clipper->AddPlane(vkPlane(m_eye, t));
      m_clipper->AddPlane(vkPlane(m_eye, l));
      m_clipper->AddPlane(vkPlane(m_eye, b));
      m_clipper->AddPlane(vkPlane(m_eye, r));
      m_clipper->AddPlane(vkPlane(n, dir));
    }
    break;

  case ePM_Orthographic:
    {
      vkVector3f tl, tr, bl, br, l, r, b, t, n, f, dir;
      vkMatrix4f::Transform(m_cameraMatrixInv, m_topLeft, tl);
      vkMatrix4f::Transform(m_cameraMatrixInv, m_topRight, tr);
      vkMatrix4f::Transform(m_cameraMatrixInv, m_bottomLeft, bl);
      vkMatrix4f::Transform(m_cameraMatrixInv, m_bottomRight, br);
      vkMatrix4f::Transform(m_cameraMatrixInv, m_frontNear, n);
      vkMatrix4f::Transform(m_cameraMatrixInv, m_frontFar, f);
      vkVector3f::Sub(tl, bl, l);
      vkVector3f::Sub(tr, tl, t);
      vkVector3f::Sub(br, tr, r);
      vkVector3f::Sub(bl, br, b);
      vkVector3f::Sub(f, n, dir);
      vkVector3f::Cross(dir, l, l);
      vkVector3f::Cross(dir, r, r);
      vkVector3f::Cross(dir, b, b);
      vkVector3f::Cross(dir, t, t);
      m_clipper->AddPlane(vkPlane(bl, l));
      m_clipper->AddPlane(vkPlane(tl, t));
      m_clipper->AddPlane(vkPlane(tr, r));
      m_clipper->AddPlane(vkPlane(br, b));



    }
    break;
  }
  return m_clipper;
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
    points[0].Set(-m_orthographicViewport.x, distance, -m_orthographicViewport.y);
    points[1].Set(m_orthographicViewport.x, distance, -m_orthographicViewport.y);
    points[2].Set(-m_orthographicViewport.x, distance, m_orthographicViewport.y);
    points[3].Set(m_orthographicViewport.x, distance, m_orthographicViewport.y);
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