#pragma once


#include <abstractasseteditor.hh>
#include <assetdescriptor.hh>

#include <textureeditor/textureeditor.refl.hh>
#include <valkyrie/vkenums.hh>

class TextureEditorProperties;
class TextureEditorWidget;
VK_CLASS()
class TextureEditor : public AbstractAssetEditor
{
  VK_CLASS_GEN_OBJECT;

public:
  TextureEditor();
  ~TextureEditor();

  virtual void PopulateDockItems();

protected:
  void OpenAsset();

private:
  TextureEditorWidget *m_widget;
  TextureEditorProperties *m_properties;
};
