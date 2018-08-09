

#include <SceneWidget/Camera.hh>
#include <Valkyrie/Math/Matrix.hh>
#include <qtimer.h>
#include <qwidget.h>


namespace scenewidget
{

Camera::Camera()
  : m_camera (0)
  , m_widget(0)
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

void Camera::SetWidget(QWidget *widget)
{
  m_widget = widget;
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








FreeCamera::FreeCamera(vkCamera *camera, float rotH, float rotV, const vkVector3f &position)
  : Camera()
  , m_rotH(rotH)
  , m_rotV(rotV)
  , m_position(position)
  , m_state(eS_Idle)
  , m_w(false)
  , m_a(false)
  , m_s(false)
  , m_d(false)
  , m_shift(false)
{
  SetCamera(camera);

  UpdateMatrices();

  m_keyTimer = new QTimer();
  m_keyTimer->setSingleShot(false);
  m_keyTimer->setInterval(1000 / 30); // update 30 fps
  connect(m_keyTimer, SIGNAL(timeout()), this, SLOT(keyTimerTimeout()));
  m_keyTimer->start();
}

FreeCamera::~FreeCamera()
{

}


void FreeCamera::UpdateMatrices()
{

  vkMatrix4f MH, MV;
  MH.SetRotationZ(m_rotH);
  MV.SetRotationX(m_rotV);

  vkMatrix4f::Mult(MH, MV, m_matrix);

  vkVector3f spot, yAxis;
  vkVector3f::Add(m_position, m_matrix.GetYAxis(yAxis), spot);
  m_camera->SetEye(m_position);
  m_camera->SetSpot(spot);
  m_camera->SetUp(vkVector3f(0.0f, 0.0f, 1.0f));

  m_camera->UpdateCameraMatrices();
}



bool FreeCamera::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
  {
    m_state = eS_Walk;
    m_lastPoint = event->pos();
  }
  else if (event->button() == Qt::RightButton)
  {
    m_state = eS_Free;
    m_lastPoint = event->pos();
  }
  return false; 
}


bool FreeCamera::mouseReleaseEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
  {
    m_state = eS_Idle;
  }
  return false;
}

bool FreeCamera::mouseMoveEvent(QMouseEvent *event)
{
  QPoint diff = event->pos() - m_lastPoint;
  m_lastPoint = event->pos();

  switch (m_state)
  {
  case eS_Idle:
    return false;
  case eS_Walk:
    {
      vkVector3f yAxis = m_matrix.GetYAxis(yAxis);
      yAxis.z = 0.0f;
      vkVector3f::Mul(yAxis, (float)diff.y() / 20.0f, yAxis);
      vkVector3f::Sub(m_position, yAxis, m_position);
    }
    m_rotH -= (float)(diff.x()) / 400.0f;
    break;

  case eS_Free:
    m_rotH -= (float)(diff.x()) / 400.0f;
    m_rotV -= (float)(diff.y()) / 400.0f;
    break;

  }


  UpdateMatrices();
  return true;
}

bool FreeCamera::keyPressEvent(QKeyEvent *event)
{
  switch (event->key())
  {
  case Qt::Key_W:
    m_w = true;
    break;
  case Qt::Key_A:
    m_a = true;
    break;
  case Qt::Key_S:
    m_s = true;
    break;
  case Qt::Key_D:
    m_d = true;
    break;
  case Qt::Key_Shift:
    m_shift = true;
    break;
  }

  return false;
}

bool FreeCamera::keyReleaseEvent(QKeyEvent *event)
{
  switch (event->key())
  {
  case Qt::Key_W:
    m_w = false;
    break;
  case Qt::Key_A:
    m_a = false;
    break;
  case Qt::Key_S:
    m_s = false;
    break;
  case Qt::Key_D:
    m_d = false;
    break;
  case Qt::Key_Shift:
    m_shift = false;
    break;
  }
  return false;
}


void FreeCamera::keyTimerTimeout()
{
  if (m_state != eS_Idle)
  {
    vkVector3f dir(0.0f, 0.0f, 0.0f);
    if (m_w)
    {
      dir.y += 1.0f;
    }
    if (m_a)
    {
      dir.x -= 1.0f;
    }
    if (m_s)
    {
      dir.y -= 1.0f;
    }
    if (m_d)
    {
      dir.x += 1.0f;
    }

    float speed = 0.1f;
    if (m_shift)
    {
      speed += speed;
    }

    vkVector3f::Mul(dir, speed, dir);
    vkMatrix4f::Mult(m_matrix, dir, dir);
    vkVector3f::Add(m_position, dir, m_position);
    UpdateMatrices();

    if (m_widget)
    {
      m_widget->repaint();
    }
  }


}

}