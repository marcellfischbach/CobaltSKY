#pragma once

#include <SceneWidget/SceneWidget.hh>

namespace scenewidget
{
class FreeCamera;
}

class vkEntity;
class vkMaterial;
class vkMaterialInstance;
class vkMesh;
class vkStaticMeshState;
namespace staticmesh
{


class PreviewWidget : public scenewidget::SceneWidget
{
public:
  PreviewWidget(QWidget *parent = 0);
  ~PreviewWidget();

  virtual QSize sizeHint() const;

  void SetStaticMeshState(vkStaticMeshState *staticMeshState);

  bool HasGeometry() const;
  void SetRenderGeometry(bool renderGeometry);

  bool HasCollision() const;
  void SetRenderCollision(bool renderCollision);
protected:
  void initializeGL();

private:
  void CreateScene();

  bool InitCollisionStaticMesh();

private:
  scenewidget::FreeCamera *m_freeCamera;

  vkEntity *m_entity;
  vkStaticMeshState *m_staticMeshState;

  vkEntity *m_collisionEntity;
  vkStaticMeshState *m_collisionStaticMesh;

};

}