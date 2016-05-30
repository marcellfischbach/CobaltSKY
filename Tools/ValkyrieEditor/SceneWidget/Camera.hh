#pragma once

#include <Valkyrie/Core/Object.hh>
#include <SceneWidget/EventListener.hh>
#include <Valkyrie/Graphics/Camera.hh>

namespace scenewidget
{

class Camera : public QObject, 
               public EventListener
{
  Q_OBJECT;
public:
  Camera();
  ~Camera();

  void SetCamera(vkCamera *camera);
  const vkCamera *GetCamera() const;

  void SetWidget(QWidget *widget);

protected:
  vkCamera *m_camera;
  QWidget *m_widget;
};

class OrbitCamera : public Camera
{
  Q_OBJECT;
public:
  OrbitCamera(vkCamera *camera, float rotH = 0.0f, float rotV = 0.0f, float distance = 20.0f, const vkVector3f &center = vkVector3f (0.0f, 0.0f, 0.0f));
  ~OrbitCamera();

  virtual bool mouseMoveEvent(QMouseEvent *event);
  virtual bool mousePressEvent(QMouseEvent *event);
  virtual bool mouseReleaseEvent(QMouseEvent *event);
  virtual bool wheelEvent(QWheelEvent *event);

private:
  void UpdateMatrices();

private:
  vkVector3f m_center;
  float m_rotH;
  float m_rotV;
  float m_distance;
  bool m_rotating;
  QPoint m_lastPoint;
};


class FreeCamera : public Camera
{
  Q_OBJECT;
public:
  FreeCamera(vkCamera *camera, float rotH = 0.0f, float rotV = 0.0f, const vkVector3f &position = vkVector3f(0.0f, 0.0f, 0.0f));
  ~FreeCamera();

  virtual bool mouseMoveEvent(QMouseEvent *event);
  virtual bool mousePressEvent(QMouseEvent *event);
  virtual bool mouseReleaseEvent(QMouseEvent *event);
  virtual bool keyPressEvent(QKeyEvent *event);
  virtual bool keyReleaseEvent(QKeyEvent *event);

private:
  void UpdateMatrices();

private:
  enum State
  {
    eS_Idle,
    eS_Walk,
    eS_Free,
  };
  vkVector3f m_position;
  float m_rotH;
  float m_rotV;
  vkMatrix4f m_matrix;

  State m_state;
  QPoint m_lastPoint;

  bool m_w, m_a, m_s, m_d, m_shift;
  QTimer *m_keyTimer;

private slots:
void keyTimerTimeout();

};

}