#pragma once


#include <abstractasseteditor.hh>
#include <assetdescriptor.hh>

#include <shadergrapheditor/shadergrapheditor.refl.hh>
#include <valkyrie/vkenums.hh>

class ShaderGraphEditorWidget;

VK_CLASS()
class ShaderGraphEditor : public AbstractAssetEditor
{
  VK_CLASS_GEN_OBJECT;

public:
  ShaderGraphEditor();
  ~ShaderGraphEditor();


protected:
  void OpenAsset();

private:
  ShaderGraphEditorWidget *m_widget;
};
