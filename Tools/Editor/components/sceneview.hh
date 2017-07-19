#pragma once

#include <QOpenGLWidget>

#include <vector>

struct iFrameProcessor;
struct iGraphics;
struct iSampler;
class csEntityScene;
class csCamera;
struct SceneViewInputHandler;
class QTCSOnscreenRenderTarget;
class QImage;
class SceneView : public QOpenGLWidget
{
public:
  SceneView(QWidget *parent = 0);
  virtual ~SceneView();

  const csEntityScene *GetScene() const;
  csEntityScene *GetScene();

  const csCamera *GetCamera() const;
  csCamera *GetCamera();

  QImage TakeScreenshot(unsigned width, unsigned height);

  void AddInputHandler(SceneViewInputHandler *handler);

protected:
  virtual void initializeGL();
  void paintGL();
  void resizeGL(int w, int h);

  void SetScene(csEntityScene *scene);

  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);

private:
  csCamera *m_camera = 0;
  csEntityScene *m_scene = 0;
  iGraphics *m_graphics = 0;
  iFrameProcessor* m_frameProcessor = 0;
  QTCSOnscreenRenderTarget *m_onscreenTarget = 0;

  std::vector<SceneViewInputHandler*> m_handlers;
};
