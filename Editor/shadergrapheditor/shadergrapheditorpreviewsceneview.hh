#pragma once


#include <shadergrapheditor/shadergrapheditorexport.hh>
#include <editor/components/preview/previewsceneview.hh>

class csMaterialDefWrapper;
class csMaterialWrapper;
class ShaderGraphEditorPreviewSceneView : public PreviewSceneView
{
public:
  ShaderGraphEditorPreviewSceneView(QWidget *parent);
  virtual ~ShaderGraphEditorPreviewSceneView();

  void SetMaterial(csMaterialDefWrapper *material);
  void UpdateMaterial();

protected:
  virtual void initializeGL();

private:
  csMaterialWrapper *m_materialInstance;
};
