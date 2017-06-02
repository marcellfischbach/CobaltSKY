
#include <preview/previewlightorbithandler.hh>
#include <valkyrie/graphics/vkdirectionallight.hh>
#include <valkyrie/math/vkmatrix4f.hh>
#include <QMouseEvent>
#include <QWheelEvent>

PreviewLightOrbitHandler::PreviewLightOrbitHandler(vkDirectionalLight *light)
{
  VK_SET(m_light, light);
  UpdateLight();
}

PreviewLightOrbitHandler::~PreviewLightOrbitHandler()
{

}

void PreviewLightOrbitHandler::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::RightButton)
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
  if (m_rotationArmed)
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
  UpdateLight();
}

void PreviewLightOrbitHandler::wheelEvent(QWheelEvent *event)
{
  
}


void PreviewLightOrbitHandler::UpdateLight()
{
  vkMatrix4f M, rH, rV;


  rH.SetRotationZ(m_rotH);
  rV.SetRotationX(m_rotV);
  vkMatrix4f::Mult(rH, rV, M);

  vkVector3f d = M.GetYAxis(d);
  vkVector3f::Mul(d, -1.0f, d);
  m_light->SetDirection(d);
}
