#pragma once

#include <Valkyrie/Core/Object.hh>
#include <SceneWidget/EventListener.hh>
#include <Valkyrie/Graphics/Camera.hh>

namespace scenewidget
{

class Camera : public EventListener
{
public:
  Camera();
  ~Camera();

  void SetCamera(vkCamera *camera);
  const vkCamera *GetCamera() const;

protected:
  vkCamera *m_camera;
};

class OrbitCamera : public Camera
{
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



}