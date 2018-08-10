#pragma once


#include <materialeditor/materialeditorexport.hh>
#include <editor/preview/previewsceneview.hh>

class csMaterial;
class csStaticMeshState;
class MATERIALEDITOR_API MaterialEditorPreviewSceneView : public PreviewSceneView
{
public:
  MaterialEditorPreviewSceneView(QWidget *parent);
  virtual ~MaterialEditorPreviewSceneView();

  void SetMaterial(csMaterial *material);

protected:
  virtual void initializeGL();

private:
  csMaterial *m_material;
  csStaticMeshState *m_staticMeshState;
};