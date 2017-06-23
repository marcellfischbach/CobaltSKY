#pragma once


#include <abstractasseteditor.hh>
#include <assetdescriptor.hh>

#include <materialeditor/materialeditor.refl.hh>
#include <cobalt/csenums.hh>

#include <QDomDocument>
#include <QDomElement>

class csMaterial;
class MaterialEditorProperties;
class MaterialEditorWidget;
CS_CLASS()
class MaterialEditor : public AbstractAssetEditor
{
  CS_CLASS_GEN_OBJECT;
public:
  MaterialEditor();
  virtual ~MaterialEditor();

  void Save();

protected:
  void OpenAsset();
  void PopulateDockItems();
  void ReplaceFileContent();

private:
  csMaterial *m_material;
  MaterialEditorWidget *m_widget;

  MaterialEditorProperties *m_properties;
};

