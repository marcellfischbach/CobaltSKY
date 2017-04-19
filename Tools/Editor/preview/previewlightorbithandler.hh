#pragma once

#include <components/sceneviewinputhandler.hh>
#include <QPoint>

class vkDirectionalLight;
class PreviewLightOrbitHandler : public SceneViewInputHandler
{
public:
  PreviewLightOrbitHandler(vkDirectionalLight *light);
  virtual ~PreviewLightOrbitHandler();

  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);

  virtual void wheelEvent(QWheelEvent *event);

private:
  void UpdateLight();

private:
  bool m_rotationArmed = false;
  float m_rotH = 0.0f;
  float m_rotV = 0.0f;


  vkDirectionalLight *m_light= 0;
  QPoint m_mousePos;
};