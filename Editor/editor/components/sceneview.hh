#pragma once

#include <editor/editorexport.hh>
#include <editor/renderwidget.hh>

#include <vector>

struct SceneViewInputHandler;
class QTCSOnscreenRenderTarget;
class QImage;
class QTimer;

namespace cs
{
struct iGraphics;
struct iSampler;
class EntityScene;
struct iFrameProcessor;
class Camera;
}

class EDITOR_API SceneView : public RenderWidget
{
  Q_OBJECT
public:
  SceneView(QWidget *parent = 0);
  virtual ~SceneView();

  const cs::EntityScene *GetScene() const;
  cs::EntityScene *GetScene();

  const cs::Camera *GetCamera() const;
  cs::Camera *GetCamera();

  QImage TakeScreenshot(unsigned width, unsigned height);

  void AddInputHandler(SceneViewInputHandler *handler);

protected:
  virtual void initializeGL();
  void paintGL();
  void resizeGL(int w, int h);

  void SetScene(cs::EntityScene *scene);

  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);

  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);

private slots:
void Timeout();
private:
  cs::Camera *m_camera = 0;
  cs::EntityScene *m_scene = 0;
  cs::iGraphics *m_graphics = 0;
  cs::iFrameProcessor* m_frameProcessor = 0;
  QTCSOnscreenRenderTarget *m_onscreenTarget = 0;
  QTimer *m_timer;

  std::vector<SceneViewInputHandler*> m_handlers;
};
