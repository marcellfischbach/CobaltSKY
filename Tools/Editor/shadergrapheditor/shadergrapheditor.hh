#pragma once


#include <abstractasseteditor.hh>
#include <assetdescriptor.hh>

#include <shadergrapheditor/shadergrapheditor.refl.hh>
#include <valkyrie/vkenums.hh>

class ShaderGraphEditorToolbox;
class ShaderGraphEditorWidget;

VK_CLASS()
class ShaderGraphEditor : public AbstractAssetEditor
{
  VK_CLASS_GEN_OBJECT;

public:
  ShaderGraphEditor();
  ~ShaderGraphEditor();

  virtual void PopulateDockItems();

protected:
  void OpenAsset();

private:
  ShaderGraphEditorToolbox *m_toolbox;
  ShaderGraphEditorWidget *m_widget;
};
