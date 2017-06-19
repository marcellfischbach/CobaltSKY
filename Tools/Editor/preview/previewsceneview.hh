#pragma once

#include <components/sceneview.hh>

class csDirectionalLight;
class csEntity;
class csLightState;
class csMaterial;
class PreviewLightOrbitHandler;
class PreviewSceneView : public SceneView
{
public:
  PreviewSceneView(QWidget *parent = 0);
  virtual ~PreviewSceneView();

protected:
  csEntity *CreateSphere(float radius, unsigned numR, unsigned numH, csMaterial *materialInstance);

private:
  csDirectionalLight *m_light = 0;
  csLightState *m_lightState = 0;
  csEntity *m_lightEntity = 0;

  PreviewLightOrbitHandler *m_lightHandler = 0;
};
