

#include <SceneWidget/Camera.hh>
#include <Valkyrie/Math/Matrix.hh>


namespace scenewidget
{

Camera::Camera()
  : m_camera (0)
{
}

Camera::~Camera()
{
  VK_RELEASE(m_camera);
}


void Camera::SetCamera(vkCamera *camera)
{
  VK_SET(m_camera, camera);
}

const vkCamera *Camera::GetCamera() const
{
  return m_camera;
}

OrbitCamera::OrbitCamera(vkCamera *camera, float rotH, float rotV, float distance, const vkVector3f &center)
  : Camera()
  , m_rotH(rotH)
  , m_rotV(rotV)
  , m_distance(distance)
  , m_center (center)
  , m_rotating(false)
{
  SetCamera(camera);

  UpdateMatrices();

}

OrbitCamera::~OrbitCamera()
{

}


void OrbitCamera::UpdateMatrices()
{

  vkMatrix4f MH, MV, M;
  MH.SetRotationZ(m_rotH);
  MV.SetRotationX(m_rotV);

  vkMatrix4f::Mult(MH, MV, M);

  vkVector3f eye;
  vkMatrix4f::Mult(M, vkVector3f(0.0f, -1.0f, 0.0f), eye);
  vkVector3f::Mul(eye, m_distance, eye);
  vkVector3f::Add(eye, m_center, eye);
  m_camera->SetEye(eye);
  m_camera->SetSpot(m_center);
  m_camera->SetUp(vkVector3f(0.0f, 0.0f, 1.0f));

  m_camera->UpdateCameraMatrices();
}



bool OrbitCamera::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
  {
    m_rotating = true;
    m_lastPoint = event->pos();
  }
  return false;
}


bool OrbitCamera::mouseReleaseEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
  {
    m_rotating = false;
  }
  return false;
}

bool OrbitCamera::mouseMoveEvent(QMouseEvent *event)
{
  if (m_rotating)
  {
    QPoint diff = event->pos() - m_lastPoint;
    m_lastPoint = event->pos();

    
    m_rotH -= (float)(diff.x()) / 100.0f;
    m_rotV -= (float)(diff.y()) / 100.0f;
    UpdateMatrices();
    return true;
  }
  return false;
}

bool OrbitCamera::wheelEvent(QWheelEvent *event)
{
  int scroll = event->angleDelta().y();
  if (!scroll)
  {
    return false;
  }
  if (scroll < 0)
  {
    m_distance *= 1.0f / 0.9f;
  }
  else
  {
    m_distance *= 0.9f;
  }
  UpdateMatrices();
  return true;
}



}