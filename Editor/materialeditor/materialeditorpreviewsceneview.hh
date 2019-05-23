#pragma once


#include <materialeditor/materialeditorexport.hh>
#include <editor/components/preview/previewsceneview.hh>

namespace cs
{
class MaterialWrapper;
class StaticMeshState;
}

class MATERIALEDITOR_API MaterialEditorPreviewSceneView : public PreviewSceneView
{
public:
  MaterialEditorPreviewSceneView(QWidget *parent);
  virtual ~MaterialEditorPreviewSceneView();

  void SetMaterial(cs::MaterialWrapper *material);

protected:
  virtual void initializeGL();

private:
  cs::MaterialWrapper *m_material;
  cs::StaticMeshState *m_staticMeshState;
};
