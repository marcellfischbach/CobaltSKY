
#include <editor/components/preview/previewcameraflowhandler.hh>
#include <cobalt/graphics/cscamera.hh>

#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>

PreviewCameraFlowHandler::PreviewCameraFlowHandler(cs::Camera *camera)
  : m_mode(Idle)
  , m_maxSpeed(6.0f)
  , m_moveAccel(6.0f)
  , m_moveDecel(32.0f)
  , m_moveSpeedForward(0.0f)
  , m_moveSpeedBackward(0.0f)
  , m_moveSpeedLeft(0.0f)
  , m_moveSpeedRight(0.0f)
  , m_w(false)
  , m_a(false)
  , m_s(false)
  , m_d(false)
  , m_up(false)
  , m_left(false)
  , m_down(false)
  , m_right(false)
{
  CS_SET(m_camera, camera);
  m_matrix = m_camera->GetCameraMatrixInv();
}

PreviewCameraFlowHandler::~PreviewCameraFlowHandler()
{

}

void PreviewCameraFlowHandler::mousePressEvent(QMouseEvent *event)
{
  if (m_mode != Idle)
  {
    return;
  }

  if (event->button() == Qt::LeftButton)
  {
    m_mode = CameraWalk;
  }
  else if (event->button() == Qt::RightButton)
  {
    m_mode = CameraFly;
  }
  else
  {
    return;
  }
  m_mousePos = event->globalPos();
}

void PreviewCameraFlowHandler::mouseReleaseEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton && m_mode == CameraWalk
    || event->button() == Qt::RightButton && m_mode == CameraFly)
  {
    m_mode = Idle;
  }
}

void PreviewCameraFlowHandler::mouseMoveEvent(QMouseEvent *event)
{
  if (m_mode == Idle)
  {
    return;
  }
  QPoint d = event->globalPos() - m_mousePos;
  if (m_mode == CameraFly)
  {
    float rotH = -(float)d.x() / 250.0f;
    float rotV = -(float)d.y() / 250.0f;

    cs::Matrix4f rH, rV;
    rH.SetRotationZ(rotH);
    rV.SetRotationX(rotV);

    cs::Vector3f t = m_matrix.GetTranslation(t);
    m_matrix.SetTranslation(cs::Vector3f(0.0f, 0.0f, 0.0f));

    cs::Matrix4f::Mult(rH, m_matrix, m_matrix);
    cs::Matrix4f::Mult(m_matrix, rV, m_matrix);

    m_matrix.SetTranslation(t);

  }
  m_mousePos = event->globalPos();

  UpdateCamera();
}

void PreviewCameraFlowHandler::keyPressEvent(QKeyEvent *event)
{
  SceneViewBaseInputHandler::keyPressEvent(event);

  if (event->key() == Qt::Key_A)
  {
    m_a = true;
  }
  else if (event->key() == Qt::Key_S)
  {
    m_s = true;
  }
  else if (event->key() == Qt::Key_D)
  {
    m_d = true;
  }
  else if (event->key() == Qt::Key_W)
  {
    m_w = true;
  }
  else if (event->key() == Qt::Key_Up)
  {
    m_up = true;
  }
  else if (event->key() == Qt::Key_Left)
  {
    m_left = true;
  }
  else if (event->key() == Qt::Key_Right)
  {
    m_right = true;
  }
  else if (event->key() == Qt::Key_Down)
  {
    m_down = true;
  }

}

void PreviewCameraFlowHandler::keyReleaseEvent(QKeyEvent *event)
{
  SceneViewBaseInputHandler::keyReleaseEvent(event);

  if (event->key() == Qt::Key_A)
  {
    m_a = false;
  }
  else if (event->key() == Qt::Key_S)
  {
    m_s = false;
  }
  else if (event->key() == Qt::Key_D)
  {
    m_d = false;
  }
  else if (event->key() == Qt::Key_W)
  {
    m_w = false;
  }
  else if (event->key() == Qt::Key_Up)
  {
    m_up = false;
  }
  else if (event->key() == Qt::Key_Left)
  {
    m_left = false;
  }
  else if (event->key() == Qt::Key_Right)
  {
    m_right = false;
  }
  else if (event->key() == Qt::Key_Down)
  {
    m_down = false;
  }
}

namespace
{
  float UpdateSpeed(bool set, float currentSpeed, float deltaT, float accel, float decel, float max)
  {
    float result = currentSpeed;
    if (set)
    {
      result += deltaT * accel;
      if (result > max)
      {
        result = max;
      }
    }
    else
    {
      result -= deltaT * decel;
      if (result < 0.0f)
      {
        result = 0.0;
      }
    }
    return result;
  }
}

void PreviewCameraFlowHandler::timedUpdate(float deltaT)
{
  if (m_mode == CameraFly)
  {
    m_moveSpeedForward = ::UpdateSpeed(m_w || m_up, m_moveSpeedForward, deltaT, m_moveAccel, m_moveDecel, m_maxSpeed);
    m_moveSpeedBackward = ::UpdateSpeed(m_s || m_down, m_moveSpeedBackward, deltaT, m_moveAccel, m_moveDecel, m_maxSpeed);
    m_moveSpeedLeft = ::UpdateSpeed(m_a || m_left, m_moveSpeedLeft, deltaT, m_moveAccel, m_moveDecel, m_maxSpeed);
    m_moveSpeedRight = ::UpdateSpeed(m_d || m_right, m_moveSpeedRight, deltaT, m_moveAccel, m_moveDecel, m_maxSpeed);

    printf("DeltaT: W: %f   A: %f   S: %f  D: %f\n", m_moveSpeedForward, m_moveSpeedLeft, m_moveSpeedBackward, m_moveSpeedRight);

    cs::Vector3f e = m_matrix.GetTranslation(e);
    cs::Vector3f d = m_matrix.GetYAxis(d);
    cs::Vector3f r = m_matrix.GetXAxis(r);

    cs::Vector3f::MulAdd(e, d, (m_moveSpeedForward - m_moveSpeedBackward) * deltaT, e);
    cs::Vector3f::MulAdd(e, r, (m_moveSpeedRight - m_moveSpeedLeft) * deltaT, e);

    m_matrix.SetTranslation(e);
    UpdateCamera();
  }
}


void PreviewCameraFlowHandler::UpdateCamera()
{

  cs::Vector3f e = m_matrix.GetTranslation(e);
  cs::Vector3f d = m_matrix.GetYAxis(d);
  cs::Vector3f s = cs::Vector3f::MulAdd(e, d, 1.0f, s);
  cs::Vector3f u = m_matrix.GetZAxis(u);



  m_camera->SetEye(e);
  m_camera->SetSpot(s);
  m_camera->SetUp(u);
  m_camera->UpdateCameraMatrices();
}

