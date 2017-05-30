#pragma once

#include <components/sceneview.hh>

class vkDirectionalLight;
class vkEntity;
class vkLightState;
class vkMaterial;
class PreviewLightOrbitHandler;
class PreviewSceneView : public SceneView
{
public:
  PreviewSceneView(QWidget *parent = 0);
  virtual ~PreviewSceneView();

protected:
  vkEntity *CreateSphere(float radius, unsigned numR, unsigned numH, vkMaterial *materialInstance);

private:
  vkDirectionalLight *m_light = 0;
  vkLightState *m_lightState = 0;
  vkEntity *m_lightEntity = 0;

  PreviewLightOrbitHandler *m_lightHandler = 0;
};
