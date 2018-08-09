#pragma once


#include <abstractasseteditor.hh>
#include <assetdescriptor.hh>

#include <editors/textureeditor/textureeditor.refl.hh>
#include <cobalt/csenums.hh>

class QSplitter;
class TextureEditorProperties;
class TextureEditorWidget;
CS_CLASS()
class TextureEditor : public AbstractAssetEditor
{
  CS_CLASS_GEN_OBJECT;

public:
  TextureEditor();
  ~TextureEditor();


protected:
  void UpdateAsset();

private:
  QSplitter *m_splitter;
  TextureEditorWidget *m_widget;
  TextureEditorProperties *m_properties;
};
