#pragma once

#include <GL/glew.h>
#include <qopenglwidget.h>

class vkCamera;
class vkEntityScene;
class vkMaterial;
class vkMaterialInstance;
class vkStaticMeshState;
class vkSubMesh;
struct IFrameProcessor;
struct IGraphics;
struct IRenderTarget;
struct ISampler;

namespace shadergraph
{

class PreviewWidget: public QOpenGLWidget
{
public:
  PreviewWidget(QWidget *parent = 0);
  ~PreviewWidget();

  virtual QSize sizeHint() const;

  void SetMaterial(vkMaterial *material);
protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int w, int h);



private:
  vkEntityScene *CreateScene();
  vkSubMesh *CreatePlaneMesh(float size, float height);
private:
  IGraphics *m_graphics;
  vkCamera *m_camera;
  IFrameProcessor* m_frameProcessor;
  vkEntityScene *m_scene;
  IRenderTarget *m_renderTarget;
  ISampler *m_sampler;
  vkStaticMeshState *m_staticMeshState;
  vkMaterialInstance *m_materialInstance;
};

}