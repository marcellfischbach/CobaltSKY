#pragma once


#include <shadergrapheditor/shadergrapheditorexport.hh>
#include <editor/components/preview/previewsceneview.hh>

namespace cs
{
class MaterialDefWrapper;
class MaterialWrapper;
}

class ShaderGraphEditorPreviewSceneView : public PreviewSceneView
{
public:
  ShaderGraphEditorPreviewSceneView(QWidget *parent);
  virtual ~ShaderGraphEditorPreviewSceneView();

  void SetMaterial(cs::MaterialDefWrapper *material);
  void UpdateMaterial();

protected:
  virtual void initializeGL();

private:
  cs::MaterialWrapper *m_materialInstance;
};
