#pragma once

#include <editor/editorexport.hh>
#include <editor/components/sceneview.hh>

class csDirectionalLight;
class csEntity;
class csLightState;
class csMaterialWrapper;
class PreviewLightOrbitHandler;
class EDITOR_API PreviewSceneView : public SceneView
{
public:
  PreviewSceneView(QWidget *parent = 0);
  virtual ~PreviewSceneView();

protected:
  csEntity *CreateSphere(float radius, unsigned numR, unsigned numH, csMaterialWrapper *materialInstance);

private:
  csDirectionalLight *m_light = 0;
  csLightState *m_lightState = 0;
  csEntity *m_lightEntity = 0;

  PreviewLightOrbitHandler *m_lightHandler = 0;
};
