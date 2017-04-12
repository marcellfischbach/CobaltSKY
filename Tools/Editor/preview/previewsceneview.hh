#pragma once

#include <components/sceneview.hh>

class vkDirectionalLight;
class vkEntity;
class vkLightState;
class vkMaterialInstance;
class PreviewSceneView : public SceneView
{
public:
  PreviewSceneView(QWidget *parent = 0);
  virtual ~PreviewSceneView();

protected:
  vkEntity *CreateSphere(float radius, unsigned numR, unsigned numH, vkMaterialInstance *materialInstance);

private:
  vkDirectionalLight *m_light;
  vkLightState *m_lightState;
  vkEntity *m_lightEntity;

};
