#pragma once


#include <preview/previewsceneview.hh>

class vkMaterial;
class vkMaterialInstance;
class ShaderGraphEditorPreviewSceneView : public PreviewSceneView
{
public:
  ShaderGraphEditorPreviewSceneView(QWidget *parent);
  virtual ~ShaderGraphEditorPreviewSceneView();

  void SetMaterial(vkMaterial *material);

protected:
  virtual void initializeGL();

private:
  vkMaterialInstance *m_materialInstance;
};