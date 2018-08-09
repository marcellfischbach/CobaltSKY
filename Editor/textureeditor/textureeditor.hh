#pragma once

#include <textureeditor/textureeditorexport.hh>
#include <editor/abstractasseteditor.hh>
#include <editor/assetdescriptor.hh>

#include <textureeditor/textureeditor.refl.hh>
#include <cobalt/csenums.hh>

class QSplitter;
class TextureEditorProperties;
class TextureEditorWidget;
CS_CLASS()
class TEXTUREEDITOR_API TextureEditor : public AbstractAssetEditor
{
  CS_CLASS_GEN_OBJECT;

public:
  TextureEditor();
  virtual ~TextureEditor();


protected:
  void UpdateAsset();

private:
  QSplitter *m_splitter;
  TextureEditorWidget *m_widget;
  TextureEditorProperties *m_properties;
};
