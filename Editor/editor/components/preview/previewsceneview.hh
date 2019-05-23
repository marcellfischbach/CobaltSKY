#pragma once

#include <editor/editorexport.hh>
#include <editor/components/sceneview.hh>

namespace cs
{
class DirectionalLight;
class Entity;
class LightState;
class MaterialWrapper;

}
class PreviewLightOrbitHandler;
class EDITOR_API PreviewSceneView : public SceneView
{
public:
  PreviewSceneView(QWidget *parent = 0);
  virtual ~PreviewSceneView();

protected:
  cs::Entity *CreateSphere(float radius, unsigned numR, unsigned numH, cs::MaterialWrapper *materialInstance);

private:
  cs::DirectionalLight *m_light = 0;
  cs::LightState *m_lightState = 0;
  cs::Entity *m_lightEntity = 0;

  PreviewLightOrbitHandler *m_lightHandler = 0;
};
