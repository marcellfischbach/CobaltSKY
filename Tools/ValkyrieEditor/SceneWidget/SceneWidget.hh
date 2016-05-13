#pragma once

#include <GL/glew.h>
#include <qopenglwidget.h>

class vkCamera;
class vkEntityScene;
class vkMaterial;
class vkMaterialInstance;
class vkStaticMeshState;
class vkSubMesh;
class vkRenderTargetGL4;
struct IFrameProcessor;
struct IGraphics;
struct IRenderTarget;
struct ISampler;

namespace scenewidget
{

class EventListener;

class SceneWidget : public QOpenGLWidget
{
public:
  SceneWidget(QWidget *parent = 0);
  ~SceneWidget();

  void AddEventListener(EventListener *eventListener);
  void RemoveEventListener(EventListener *eventListener);

protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int w, int h);

  virtual void	keyPressEvent(QKeyEvent *event);
  virtual void	keyReleaseEvent(QKeyEvent *event);
  virtual void	mouseMoveEvent(QMouseEvent *event);
  virtual void	mousePressEvent(QMouseEvent *event);
  virtual void	mouseReleaseEvent(QMouseEvent *event);
  virtual void	wheelEvent(QWheelEvent *event);

  void SetScene(vkEntityScene *scene);

protected:
  IGraphics *m_graphics;
  vkCamera *m_camera;


private:
  QList<EventListener *> m_eventListeners;
  IFrameProcessor* m_frameProcessor;
  vkEntityScene *m_scene;
  IRenderTarget *m_renderTarget;
  ISampler *m_sampler;
  vkRenderTargetGL4 *m_onscreenTarget;
};

}