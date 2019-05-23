#pragma once

#include <editor/editorexport.hh>
#include <editor/components/sceneviewbaseinputhandler.hh>
#include <QPoint>

namespace cs
{
class Camera;
}
class EDITOR_API PreviewCameraOrbitHandler : public SceneViewBaseInputHandler
{
public:
  PreviewCameraOrbitHandler(cs::Camera *camera);
  virtual ~PreviewCameraOrbitHandler();

  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);

  virtual void wheelEvent(QWheelEvent *event);


private:
  void UpdateCamera();

private:
  bool m_rotationArmed = false;
  float m_rotH = 0.0f;
  float m_rotV = 0.0f;

  float m_cameraDistance = 25.0f;

  cs::Camera *m_camera = 0;
  QPoint m_mousePos;
};