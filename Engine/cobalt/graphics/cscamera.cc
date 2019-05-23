
#include <cobalt/graphics/cscamera.hh>
#include <cobalt/graphics/igraphics.hh>


cs::Camera::Camera()
  : cs::Object()
  , m_near(1.0f)
  , m_far(1024.0f)
  , m_projectionChanged(false)
  , m_clipper(new cs::PlaneClipper())
{
  m_cameraMatrix.SetIdentity();
  m_cameraMatrixInv.SetIdentity();
  m_projectionMatrix.SetIdentity();
  m_projectionMatrixInv.SetIdentity();
}

cs::Camera::~Camera()
{

}

void cs::Camera::Apply(cs::iGraphics *renderer) 
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


void cs::Camera::UpdateCameraMatrices()
{
  m_cameraMatrix.SetLookAt(m_eye, m_spot, m_up);
  m_cameraMatrixInv.SetLookAtInv(m_eye, m_spot, m_up);
}

void cs::Camera::UpdateProjectionMatrices(cs::iGraphics *renderer)
{
  switch (m_projectionMode)
  {
    case cs::ePM_Perspective:
      renderer->GetPerspectiveProjection(m_left, m_right, m_bottom, m_top, m_near, m_far, m_projectionMatrix);
      renderer->GetPerspectiveProjectionInv(m_left, m_right, m_bottom, m_top, m_near, m_far, m_projectionMatrixInv);
      break;
    case cs::ePM_Orthographic:
      renderer->GetOrthographicProjection(m_left, m_right, m_bottom, m_top, m_near, m_far, m_projectionMatrix);
      renderer->GetOrthographicProjectionInv(m_left, m_right, m_bottom, m_top, m_near, m_far, m_projectionMatrixInv);
      break;
  }
  m_projectionChanged = false;
}

void cs::Camera::UpdateProjectionValues()
{
  switch (m_projectionMode)
  {
  case cs::ePM_Perspective:
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
  case cs::ePM_Orthographic:
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

void cs::Camera::SetPerspective(float angle, float aspect)
{
  m_perspectiveAngle = angle;
  m_perspectiveAspect = aspect;
  m_projectionMode = cs::ePM_Perspective;

  UpdateProjectionValues();
}

void cs::Camera::SetOrthographic(const cs::Vector2f &viewport)
{
  m_orthographicViewport = viewport;
  m_projectionMode = cs::ePM_Orthographic;

  UpdateProjectionValues();
}


cs::Clipper *cs::Camera::GetClipper()
{
  m_clipper->Clear();
  switch (m_projectionMode)
  {
  case cs::ePM_Perspective:
    {
      cs::Vector3f tl, tr, bl, br, l, r, b, t, n, f, dir;

      cs::Matrix4f::Transform(m_cameraMatrixInv, m_topLeft, tl);
      cs::Matrix4f::Transform(m_cameraMatrixInv, m_topRight, tr);
      cs::Matrix4f::Transform(m_cameraMatrixInv, m_bottomLeft, bl);
      cs::Matrix4f::Transform(m_cameraMatrixInv, m_bottomRight, br);
      cs::Matrix4f::Transform(m_cameraMatrixInv, m_frontNear, n);
      cs::Matrix4f::Transform(m_cameraMatrixInv, m_frontFar, f);
      cs::Vector3f::Sub(tl, m_eye, tl);
      cs::Vector3f::Sub(tr, m_eye, tr);
      cs::Vector3f::Sub(bl, m_eye, bl);
      cs::Vector3f::Sub(br, m_eye, br);
      cs::Vector3f::Sub(f, n, dir);
      cs::Vector3f::Cross(tl, tr, t);
      cs::Vector3f::Cross(bl, tl, l);
      cs::Vector3f::Cross(br, bl, b);
      cs::Vector3f::Cross(tr, br, r);
      m_clipper->AddPlane(cs::Plane(m_eye, t));
      m_clipper->AddPlane(cs::Plane(m_eye, l));
      m_clipper->AddPlane(cs::Plane(m_eye, b));
      m_clipper->AddPlane(cs::Plane(m_eye, r));
      m_clipper->AddPlane(cs::Plane(n, dir));
    }
    break;

  case cs::ePM_Orthographic:
    {
      cs::Vector3f tl, tr, bl, br, l, r, b, t, n, f, dir;
      cs::Matrix4f::Transform(m_cameraMatrixInv, m_topLeft, tl);
      cs::Matrix4f::Transform(m_cameraMatrixInv, m_topRight, tr);
      cs::Matrix4f::Transform(m_cameraMatrixInv, m_bottomLeft, bl);
      cs::Matrix4f::Transform(m_cameraMatrixInv, m_bottomRight, br);
      cs::Matrix4f::Transform(m_cameraMatrixInv, m_frontNear, n);
      cs::Matrix4f::Transform(m_cameraMatrixInv, m_frontFar, f);
      cs::Vector3f::Sub(tl, bl, l);
      cs::Vector3f::Sub(tr, tl, t);
      cs::Vector3f::Sub(br, tr, r);
      cs::Vector3f::Sub(bl, br, b);
      cs::Vector3f::Sub(f, n, dir);
      cs::Vector3f::Cross(dir, l, l);
      cs::Vector3f::Cross(dir, r, r);
      cs::Vector3f::Cross(dir, b, b);
      cs::Vector3f::Cross(dir, t, t);
      m_clipper->AddPlane(cs::Plane(bl, l));
      m_clipper->AddPlane(cs::Plane(tl, t));
      m_clipper->AddPlane(cs::Plane(tr, r));
      m_clipper->AddPlane(cs::Plane(br, b));



    }
    break;
  }
  return m_clipper;
}

void cs::Camera::GetPlanePoints(float distance, cs::Vector3f *points) const
{
  switch (m_projectionMode)
  {
  case cs::ePM_Perspective:
    cs::Vector3f::Mul(m_bottomLeft, distance, points[0]);
    cs::Vector3f::Mul(m_bottomRight, distance, points[1]);
    cs::Vector3f::Mul(m_topLeft, distance, points[2]);
    cs::Vector3f::Mul(m_topRight, distance, points[3]);
    break;
  case cs::ePM_Orthographic:
    points[0].Set(-m_orthographicViewport.x, distance, -m_orthographicViewport.y);
    points[1].Set(m_orthographicViewport.x, distance, -m_orthographicViewport.y);
    points[2].Set(-m_orthographicViewport.x, distance, m_orthographicViewport.y);
    points[3].Set(m_orthographicViewport.x, distance, m_orthographicViewport.y);
    break;
  }

  cs::Matrix4f::Transform(m_cameraMatrixInv, points[0], points[0]);
  cs::Matrix4f::Transform(m_cameraMatrixInv, points[1], points[1]);
  cs::Matrix4f::Transform(m_cameraMatrixInv, points[2], points[2]);
  cs::Matrix4f::Transform(m_cameraMatrixInv, points[3], points[3]);

  /*
  printf("GetPlanePoints: %f <%.2f %.2f %.2f> <%.2f %.2f %.2f>\n", distance, m_eye.x, m_eye.y, m_eye.z, m_spot.x, m_spot.y, m_spot.z);
  printf("    <%.2f %.2f %.2f>\n", points[0].x, points[0].y, points[0].z);
  printf("    <%.2f %.2f %.2f>\n", points[1].x, points[1].y, points[1].z);
  printf("    <%.2f %.2f %.2f>\n", points[2].x, points[2].y, points[2].z);
  printf("    <%.2f %.2f %.2f>\n", points[3].x, points[3].y, points[3].z);
  */
}