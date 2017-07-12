#pragma once


#include <abstractasseteditor.hh>
#include <assetdescriptor.hh>

#include <textureeditor/textureeditor.refl.hh>
#include <cobalt/csenums.hh>

class TextureEditorProperties;
class TextureEditorWidget;
CS_CLASS()
class TextureEditor : public AbstractAssetEditor
{
  CS_CLASS_GEN_OBJECT;

public:
  TextureEditor();
  ~TextureEditor();

  virtual void PopulateDockItems();

protected:
  void UpdateAsset();

private:
  TextureEditorWidget *m_widget;
  TextureEditorProperties *m_properties;
};
