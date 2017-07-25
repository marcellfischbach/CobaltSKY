#pragma once


#include <abstractasseteditor.hh>
#include <assetdescriptor.hh>

#include <shadergrapheditor/shadergrapheditor.refl.hh>
#include <cobalt/csenums.hh>

class ShaderGraphEditorOutliner;
class ShaderGraphEditorPreview;
class ShaderGraphEditorProperties;
class ShaderGraphEditorToolbox;
class ShaderGraphEditorWidget;

CS_CLASS()
class ShaderGraphEditor : public AbstractAssetEditor
{
  CS_CLASS_GEN_OBJECT;

public:
  ShaderGraphEditor();
  ~ShaderGraphEditor();

  virtual void PopulateDockItems();
  void UpdatePreview();
  QImage TakeScreenshot(unsigned width, unsigned height);

protected:
  void UpdateAsset();

private:
  ShaderGraphEditorOutliner *m_outliner;
  ShaderGraphEditorPreview *m_preview;
  ShaderGraphEditorProperties *m_properties;
  ShaderGraphEditorToolbox *m_toolbox;
  ShaderGraphEditorWidget *m_widget;
};
