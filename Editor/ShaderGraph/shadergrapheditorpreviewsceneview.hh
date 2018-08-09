#pragma once


#include <preview/previewsceneview.hh>

class csMaterialDef;
class csMaterial;
class ShaderGraphEditorPreviewSceneView : public PreviewSceneView
{
public:
  ShaderGraphEditorPreviewSceneView(QWidget *parent);
  virtual ~ShaderGraphEditorPreviewSceneView();

  void SetMaterial(csMaterialDef *material);
  void UpdateMaterial();

protected:
  virtual void initializeGL();

private:
  csMaterial *m_materialInstance;
};