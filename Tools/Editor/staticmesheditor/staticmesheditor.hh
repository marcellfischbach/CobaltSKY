#pragma once


#include <abstractasseteditor.hh>
#include <assetdescriptor.hh>

#include <staticmesheditor/staticmesheditor.refl.hh>
#include <cobalt/csenums.hh>

class StaticMeshEditorWidget;
CS_CLASS()
class StaticMeshEditor : public AbstractAssetEditor
{
  CS_CLASS_GEN_OBJECT;

public:
  StaticMeshEditor();
  ~StaticMeshEditor();

  virtual void PopulateDockItems();

protected:
  void UpdateAsset();

private:
  StaticMeshEditorWidget *m_widget;
  //TextureEditorProperties *m_properties;
};
