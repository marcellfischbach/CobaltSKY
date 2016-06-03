#pragma once

#include <SceneWidget/SceneWidget.hh>

namespace scenewidget
{
class OrbitCamera;
}

class vkMaterial;
class vkDirectionalLight;
namespace shadergraph
{


class PreviewWidget: public scenewidget::SceneWidget
{
public:
  PreviewWidget(QWidget *parent = 0);
  ~PreviewWidget();

  virtual QSize sizeHint() const;

  void SetMaterial(vkMaterial *material);
protected:
  void initializeGL();
  virtual void	mouseMoveEvent(QMouseEvent *event);
  virtual void	mousePressEvent(QMouseEvent *event);
  virtual void	mouseReleaseEvent(QMouseEvent *event);
private:
  void UpdateLight();
  vkEntityScene *CreateScene();
  vkSubMesh *CreatePlaneMesh(float size, float height);
  vkSubMesh *CreateSphereMesh(float radius, int numH, int numV);

private:
  scenewidget::OrbitCamera *m_orbitCamera;
  vkStaticMeshState *m_staticMeshState;
  vkMaterial *m_material;
  vkMaterialInstance *m_materialInstance;

  vkDirectionalLight *m_light;

  bool m_lightRotating;
  float m_lightRotH;
  float m_lightRotV;
  QPoint m_lastPoint;
};

}
