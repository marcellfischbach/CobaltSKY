#pragma once


#include <editor/editorexport.hh>
#include <editor/abstractasseteditor.hh>
#include <editor/assetdescriptor.hh>

#include <editor/editors/staticmesheditor/staticmesheditor.refl.hh>
#include <cobalt/csenums.hh>

class StaticMeshEditorWidget;
CS_CLASS()
class EDITOR_API StaticMeshEditor : public AbstractAssetEditor
{
  CS_CLASS_GEN_OBJECT;

public:
  StaticMeshEditor();
  virtual ~StaticMeshEditor();

protected:
  void UpdateAsset();

private:
  StaticMeshEditorWidget *m_widget;
  //TextureEditorProperties *m_properties;
};
