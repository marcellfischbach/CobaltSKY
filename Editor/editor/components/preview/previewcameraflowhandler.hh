#pragma once

#include <editor/editorexport.hh>
#include <editor/components/sceneviewbaseinputhandler.hh>
#include <cobalt/math/csmatrix4f.hh>
#include <QPoint>

namespace cs
{
class Camera;
}
class EDITOR_API PreviewCameraFlowHandler : public SceneViewBaseInputHandler
{
public:
  PreviewCameraFlowHandler(cs::Camera *camera);
  virtual ~PreviewCameraFlowHandler();

  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);

  virtual void wheelEvent(QWheelEvent *event) { }

  virtual void keyPressEvent(QKeyEvent *event);
  virtual void keyReleaseEvent(QKeyEvent *event);

  virtual void timedUpdate(float deltaT);

private:
  void UpdateCamera();

private:
  enum CameraMode
  {
    Idle,
    CameraFly,
    CameraWalk,
  };


  CameraMode m_mode;

  cs::Matrix4f m_matrix;
  bool m_shiftDown;
  bool m_ctrlDown;

  bool m_a;
  bool m_s;
  bool m_d;
  bool m_w;

  bool m_left;
  bool m_right;
  bool m_up;
  bool m_down;

  float m_moveSpeedForward;
  float m_moveSpeedBackward;
  float m_moveSpeedLeft;
  float m_moveSpeedRight;

  float m_moveAccel;
  float m_moveDecel;
  float m_maxSpeed;

  cs::Camera *m_camera = 0;
  QPoint m_mousePos;
};