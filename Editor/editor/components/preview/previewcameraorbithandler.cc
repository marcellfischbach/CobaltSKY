
#include <editor/components/preview/previewcameraorbithandler.hh>
#include <cobalt/graphics/cscamera.hh>
#include <QMouseEvent>
#include <QWheelEvent>

PreviewCameraOrbitHandler::PreviewCameraOrbitHandler(csCamera *camera)
{
  CS_SET(m_camera, camera);
  UpdateCamera();
}

PreviewCameraOrbitHandler::~PreviewCameraOrbitHandler()
{

}

void PreviewCameraOrbitHandler::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton && !IsShiftDown() && !IsCtrlDown())
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
  if (m_rotationArmed && !IsShiftDown() && !IsCtrlDown())
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
  else
  {
    m_rotationArmed = false;
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
  csMatrix4f M, rH, rV;


  rH.SetRotationZ(m_rotH);
  rV.SetRotationX(m_rotV);
  csMatrix4f::Mult(rH, rV, M);


  csVector3f p(0.0f, 0.0f, 0.0f);
  csVector3f y;
  csVector3f e = csVector3f::MulAdd(p, M.GetYAxis(y), m_cameraDistance, e);
  csVector3f u(0.0f, 0.0f, 1.0f);

  m_camera->SetEye(e);
  m_camera->SetSpot(p);
  m_camera->SetUp(u);
  m_camera->UpdateCameraMatrices();
}

