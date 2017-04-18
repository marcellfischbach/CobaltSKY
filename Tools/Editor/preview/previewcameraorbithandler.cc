
#include <preview/previewcameraorbithandler.hh>
#include <valkyrie/graphics/vkcamera.hh>
#include <QMouseEvent>
#include <QWheelEvent>

PreviewCameraOrbitHandler::PreviewCameraOrbitHandler(vkCamera *camera)
{
  VK_SET(m_camera, camera);
  UpdateCamera();
}

PreviewCameraOrbitHandler::~PreviewCameraOrbitHandler()
{

}

void PreviewCameraOrbitHandler::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
  {
    m_rotationArmed = true;
    m_mousePos = event->globalPos();
  }
}

void PreviewCameraOrbitHandler::mouseReleaseEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
  {
    m_rotationArmed = false;
  }
}

void PreviewCameraOrbitHandler::mouseMoveEvent(QMouseEvent *event)
{
  if (m_rotationArmed)
  {
    QPoint d = event->globalPos() - m_mousePos;
    m_rotH -= (float)d.x() / 100.0f;
    m_rotV += (float)d.y() / 100.0f;
    if (m_rotV < -1.5f)
    {
      m_rotV = -1.5f;
    }
    if (m_rotV > 1.5f)
    {
      m_rotV = 1.5f;
    }

    m_mousePos = event->globalPos();
  }
  UpdateCamera();
}

void PreviewCameraOrbitHandler::wheelEvent(QWheelEvent *event)
{
  if (event->angleDelta().y() > 0)
  {
    if (m_cameraDistance > 1.0)
    {
      m_cameraDistance = m_cameraDistance / 1.1f;
    }
  }
  else if (event->angleDelta().y() < 0)
  {
    m_cameraDistance = m_cameraDistance * 1.1f;
  }

  UpdateCamera();
}


void PreviewCameraOrbitHandler::UpdateCamera()
{
  vkMatrix4f M, rH, rV;


  rH.SetRotationZ(m_rotH);
  rV.SetRotationX(m_rotV);
  vkMatrix4f::Mult(rH, rV, M);


  vkVector3f p(0.0f, 0.0f, 0.0f);
  vkVector3f y;
  vkVector3f e = vkVector3f::MulAdd(p, M.GetYAxis(y), m_cameraDistance, e);
  vkVector3f u(0.0f, 0.0f, 1.0f);

  m_camera->SetEye(e);
  m_camera->SetSpot(p);
  m_camera->SetUp(u);
  m_camera->UpdateCameraMatrices();
}

