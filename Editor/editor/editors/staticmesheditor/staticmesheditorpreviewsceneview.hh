#pragma once


#include <editor/editorexport.hh>
#include <editor/components/preview/previewsceneview.hh>

namespace cs
{
class Entity;
class StaticMeshState;
}
class StaticMeshEditorPreviewSceneView : public PreviewSceneView
{
public:
  StaticMeshEditorPreviewSceneView(QWidget *parent);
  virtual ~StaticMeshEditorPreviewSceneView();

  void SetStaticMeshState(cs::StaticMeshState *staticMeshState);

protected:
  virtual void initializeGL();

private:
  cs::Entity *m_entity;
  cs::StaticMeshState *m_staticMeshState;
};