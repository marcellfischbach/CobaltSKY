#pragma once

#include <SceneWidget/SceneWidget.hh>

namespace scenewidget
{
class OrbitCamera;
}

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

private:
  vkEntityScene *CreateScene();
  vkSubMesh *CreatePlaneMesh(float size, float height);

private:
  scenewidget::OrbitCamera *m_orbitCamera;
  vkStaticMeshState *m_staticMeshState;
  vkMaterial *m_material;
  vkMaterialInstance *m_materialInstance;
};

}