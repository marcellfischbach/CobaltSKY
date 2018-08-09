
#include <editor/preview/previewlightorbithandler.hh>
#include <cobalt/graphics/csdirectionallight.hh>
#include <cobalt/math/csmatrix4f.hh>
#include <QMouseEvent>
#include <QWheelEvent>

PreviewLightOrbitHandler::PreviewLightOrbitHandler(csDirectionalLight *light)
{
  CS_SET(m_light, light);
  UpdateLight();
}

PreviewLightOrbitHandler::~PreviewLightOrbitHandler()
{

}

void PreviewLightOrbitHandler::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton && IsShiftDown() && !IsCtrlDown())
  {
    m_rotationArmed = true;
    m_mousePos = event->globalPos();
  }
}

void PreviewLightOrbitHandler::mouseReleaseEvent(QMouseEvent *event)
{
  if (event->button() == Qt::RightButton)
  {
    m_rotationArmed = false;
  }
}

void PreviewLightOrbitHandler::mouseMoveEvent(QMouseEvent *event)
{
  if (m_rotationArmed && IsShiftDown() && !IsCtrlDown())
  {
    QPoint d = event->globalPos() - m_mousePos;
    m_rotH += (float)d.x() / 100.0f;
    m_rotV -= (float)d.y() / 100.0f;
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
  UpdateLight();
}

void PreviewLightOrbitHandler::wheelEvent(QWheelEvent *event)
{
  
}


void PreviewLightOrbitHandler::UpdateLight()
{
  csMatrix4f M, rH, rV;


  rH.SetRotationZ(m_rotH);
  rV.SetRotationX(m_rotV);
  csMatrix4f::Mult(rH, rV, M);

  csVector3f d = M.GetYAxis(d);
  csVector3f::Mul(d, -1.0f, d);
  m_light->SetDirection(d);
}

