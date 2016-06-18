#pragma once

#include <SceneWidget/SceneWidget.hh>

namespace scenewidget
{
class OrbitCamera;
}

class vkMaterial;
class vkDirectionalLight;
namespace materialinstance
{


class PreviewWidget: public scenewidget::SceneWidget
{
public:
  PreviewWidget(QWidget *parent = 0);
  ~PreviewWidget();

  virtual QSize sizeHint() const;

  void SetMaterialInstance(vkMaterialInstance *materialInstance);
protected:
  void initializeGL();
  virtual void	mouseMoveEvent(QMouseEvent *event);
  virtual void	mousePressEvent(QMouseEvent *event);
  virtual void	mouseReleaseEvent(QMouseEvent *event);
private:
  void UpdateLight();
  vkEntityScene *CreateScene();


private:
  scenewidget::OrbitCamera *m_orbitCamera;
  vkStaticMeshState *m_staticMeshState;
  vkMaterialInstance *m_materialInstance;

  vkDirectionalLight *m_light;

  bool m_lightRotating;
  float m_lightRotH;
  float m_lightRotV;
  QPoint m_lastPoint;
};

}
