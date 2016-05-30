#pragma once

#include <SceneWidget/SceneWidget.hh>

namespace scenewidget
{
class OrbitCamera;
}

class vkEntity;
class vkMaterial;
class vkMaterialInstance;
class vkMesh;
class vkStaticMeshState;
namespace entity
{


class PreviewWidget : public scenewidget::SceneWidget
{
public:
  PreviewWidget(QWidget *parent = 0);
  ~PreviewWidget();

  virtual QSize sizeHint() const;

  void SetMesh(vkMesh *mesh);
protected:
  void initializeGL();

private:
  void CreateScene();
  vkMaterialInstance *CreateDefaultMaterial();


private:
  scenewidget::OrbitCamera *m_orbitCamera;

  vkEntity *m_entity;
  vkStaticMeshState *m_staticMeshState;


  vkMaterial *m_material;
  vkMaterialInstance *m_materialInstance;
};

}