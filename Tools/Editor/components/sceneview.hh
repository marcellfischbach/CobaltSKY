#pragma once

#include <GL/glew.h>
#include <QOpenGLWidget>

#include <vector>

struct iFrameProcessor;
struct iSampler;
class vkEntityScene;
class vkRenderTargetGL4;
class vkCamera;
class vkGraphicsGL4;
struct SceneViewInputHandler;
class SceneView : public QOpenGLWidget
{
public:
  SceneView(QWidget *parent = 0);
  virtual ~SceneView();

  const vkEntityScene *GetScene() const;
  vkEntityScene *GetScene();

  const vkCamera *GetCamera() const;
  vkCamera *GetCamera();

  void AddInputHandler(SceneViewInputHandler *handler);

protected:
  virtual void initializeGL();
  void paintGL();
  void resizeGL(int w, int h);

  void SetScene(vkEntityScene *scene);

  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);

private:
  vkCamera *m_camera = 0;
  vkEntityScene *m_scene = 0;
  vkGraphicsGL4 *m_graphics = 0;
  iFrameProcessor* m_frameProcessor = 0;
  vkRenderTargetGL4 *m_onscreenTarget = 0;

  std::vector<SceneViewInputHandler*> m_handlers;
};
