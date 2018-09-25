
#include <cobalt/graphics/cscamera.hh>
#include <cobalt/graphics/igraphics.hh>


csCamera::csCamera()
  : csObject()
  , m_near(1.0f)
  , m_far(1024.0f)
  , m_projectionChanged(false)
  , m_clipper(new csPlaneClipper())
{
  m_cameraMatrix.SetIdentity();
  m_cameraMatrixInv.SetIdentity();
  m_projectionMatrix.SetIdentity();
  m_projectionMatrixInv.SetIdentity();
}

csCamera::~csCamera()
{

}

void csCamera::Apply(iGraphics *renderer) 
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


void csCamera::UpdateCameraMatrices()
{
  m_cameraMatrix.SetLookAt(m_eye, m_spot, m_up);
  m_cameraMatrixInv.SetLookAtInv(m_eye, m_spot, m_up);
}

void csCamera::UpdateProjectionMatrices(iGraphics *renderer)
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

void csCamera::UpdateProjectionValues()
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

void csCamera::SetPerspective(float angle, float aspect)
{
  m_perspectiveAngle = angle;
  m_perspectiveAspect = aspect;
  m_projectionMode = ePM_Perspective;

  UpdateProjectionValues();
}

void csCamera::SetOrthographic(const csVector2f &viewport)
{
  m_orthographicViewport = viewport;
  m_projectionMode = ePM_Orthographic;

  UpdateProjectionValues();
}


csClipper *csCamera::GetClipper()
{
  m_clipper->Clear();
  switch (m_projectionMode)
  {
  case ePM_Perspective:
    {
      csVector3f tl, tr, bl, br, l, r, b, t, n, f, dir;

      csMatrix4f::Transform(m_cameraMatrixInv, m_topLeft, tl);
      csMatrix4f::Transform(m_cameraMatrixInv, m_topRight, tr);
      csMatrix4f::Transform(m_cameraMatrixInv, m_bottomLeft, bl);
      csMatrix4f::Transform(m_cameraMatrixInv, m_bottomRight, br);
      csMatrix4f::Transform(m_cameraMatrixInv, m_frontNear, n);
      csMatrix4f::Transform(m_cameraMatrixInv, m_frontFar, f);
      csVector3f::Sub(tl, m_eye, tl);
      csVector3f::Sub(tr, m_eye, tr);
      csVector3f::Sub(bl, m_eye, bl);
      csVector3f::Sub(br, m_eye, br);
      csVector3f::Sub(f, n, dir);
      csVector3f::Cross(tl, tr, t);
      csVector3f::Cross(bl, tl, l);
      csVector3f::Cross(br, bl, b);
      csVector3f::Cross(tr, br, r);
      m_clipper->AddPlane(csPlane(m_eye, t));
      m_clipper->AddPlane(csPlane(m_eye, l));
      m_clipper->AddPlane(csPlane(m_eye, b));
      m_clipper->AddPlane(csPlane(m_eye, r));
      m_clipper->AddPlane(csPlane(n, dir));
    }
    break;

  case ePM_Orthographic:
    {
      csVector3f tl, tr, bl, br, l, r, b, t, n, f, dir;
      csMatrix4f::Transform(m_cameraMatrixInv, m_topLeft, tl);
      csMatrix4f::Transform(m_cameraMatrixInv, m_topRight, tr);
      csMatrix4f::Transform(m_cameraMatrixInv, m_bottomLeft, bl);
      csMatrix4f::Transform(m_cameraMatrixInv, m_bottomRight, br);
      csMatrix4f::Transform(m_cameraMatrixInv, m_frontNear, n);
      csMatrix4f::Transform(m_cameraMatrixInv, m_frontFar, f);
      csVector3f::Sub(tl, bl, l);
      csVector3f::Sub(tr, tl, t);
      csVector3f::Sub(br, tr, r);
      csVector3f::Sub(bl, br, b);
      csVector3f::Sub(f, n, dir);
      csVector3f::Cross(dir, l, l);
      csVector3f::Cross(dir, r, r);
      csVector3f::Cross(dir, b, b);
      csVector3f::Cross(dir, t, t);
      m_clipper->AddPlane(csPlane(bl, l));
      m_clipper->AddPlane(csPlane(tl, t));
      m_clipper->AddPlane(csPlane(tr, r));
      m_clipper->AddPlane(csPlane(br, b));



    }
    break;
  }
  return m_clipper;
}

void csCamera::GetPlanePoints(float distance, csVector3f *points) const
{
  switch (m_projectionMode)
  {
  case ePM_Perspective:
    csVector3f::Mul(m_bottomLeft, distance, points[0]);
    csVector3f::Mul(m_bottomRight, distance, points[1]);
    csVector3f::Mul(m_topLeft, distance, points[2]);
    csVector3f::Mul(m_topRight, distance, points[3]);
    break;
  case ePM_Orthographic:
    points[0].Set(-m_orthographicViewport.x, distance, -m_orthographicViewport.y);
    points[1].Set(m_orthographicViewport.x, distance, -m_orthographicViewport.y);
    points[2].Set(-m_orthographicViewport.x, distance, m_orthographicViewport.y);
    points[3].Set(m_orthographicViewport.x, distance, m_orthographicViewport.y);
    break;
  }

  csMatrix4f::Transform(m_cameraMatrixInv, points[0], points[0]);
  csMatrix4f::Transform(m_cameraMatrixInv, points[1], points[1]);
  csMatrix4f::Transform(m_cameraMatrixInv, points[2], points[2]);
  csMatrix4f::Transform(m_cameraMatrixInv, points[3], points[3]);

  /*
  printf("GetPlanePoints: %f <%.2f %.2f %.2f> <%.2f %.2f %.2f>\n", distance, m_eye.x, m_eye.y, m_eye.z, m_spot.x, m_spot.y, m_spot.z);
  printf("    <%.2f %.2f %.2f>\n", points[0].x, points[0].y, points[0].z);
  printf("    <%.2f %.2f %.2f>\n", points[1].x, points[1].y, points[1].z);
  printf("    <%.2f %.2f %.2f>\n", points[2].x, points[2].y, points[2].z);
  printf("    <%.2f %.2f %.2f>\n", points[3].x, points[3].y, points[3].z);
  */
}