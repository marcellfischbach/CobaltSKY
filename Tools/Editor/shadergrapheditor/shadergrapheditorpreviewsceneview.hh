#pragma once


#include <preview/previewsceneview.hh>

class vkMaterialDef;
class vkMaterial;
class ShaderGraphEditorPreviewSceneView : public PreviewSceneView
{
public:
  ShaderGraphEditorPreviewSceneView(QWidget *parent);
  virtual ~ShaderGraphEditorPreviewSceneView();

  void SetMaterial(vkMaterialDef *material);
  void UpdateMaterial();

protected:
  virtual void initializeGL();

private:
  vkMaterial *m_materialInstance;
};