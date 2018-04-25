#pragma once


#include <preview/previewsceneview.hh>

class csEntity;
class csStaticMeshState;
class StaticMeshEditorPreviewSceneView : public PreviewSceneView
{
public:
  StaticMeshEditorPreviewSceneView(QWidget *parent);
  virtual ~StaticMeshEditorPreviewSceneView();

  void SetStaticMeshState(csStaticMeshState *staticMeshState);

protected:
  virtual void initializeGL();

private:
  csEntity *m_entity;
  csStaticMeshState *m_staticMeshState;
};