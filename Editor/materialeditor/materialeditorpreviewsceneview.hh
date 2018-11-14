#pragma once


#include <materialeditor/materialeditorexport.hh>
#include <editor/components/preview/previewsceneview.hh>

class csMaterialWrapper;
class csStaticMeshState;
class MATERIALEDITOR_API MaterialEditorPreviewSceneView : public PreviewSceneView
{
public:
  MaterialEditorPreviewSceneView(QWidget *parent);
  virtual ~MaterialEditorPreviewSceneView();

  void SetMaterial(csMaterialWrapper *material);

protected:
  virtual void initializeGL();

private:
  csMaterialWrapper *m_material;
  csStaticMeshState *m_staticMeshState;
};
