#pragma once


#include <abstractasseteditor.hh>
#include <assetdescriptor.hh>

#include <editors/staticmesheditor/staticmesheditor.refl.hh>
#include <cobalt/csenums.hh>

class StaticMeshEditorWidget;
CS_CLASS()
class StaticMeshEditor : public AbstractAssetEditor
{
  CS_CLASS_GEN_OBJECT;

public:
  StaticMeshEditor();
  ~StaticMeshEditor();

protected:
  void UpdateAsset();

private:
  StaticMeshEditorWidget *m_widget;
  //TextureEditorProperties *m_properties;
};
